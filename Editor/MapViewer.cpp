#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include <MapViewer.h>
#include <math.h>
#include <sstream>

wxBEGIN_EVENT_TABLE(MapViewer, wxPanel)
//    EVT_COMBOBOX(idAbbrCombo, MonsterView::on_cbox_update)
//    EVT_TEXT(idAbbrCombo, MonsterView::on_cbox_update)
//    EVT_TEXT_ENTER(idAbbrCombo, MonsterView::on_cbox_update)
    // catch paint events
    EVT_PAINT(MapViewer::paint_event)
    // some useful events
/*
 EVT_MOTION(BasicDrawPane::mouseMoved)
 EVT_LEFT_DOWN(BasicDrawPane::mouseDown)
 EVT_LEFT_UP(BasicDrawPane::mouseReleased)
 EVT_RIGHT_DOWN(BasicDrawPane::rightClick)
 EVT_LEAVE_WINDOW(BasicDrawPane::mouseLeftWindow)
 EVT_KEY_DOWN(BasicDrawPane::keyPressed)
 EVT_KEY_UP(BasicDrawPane::keyReleased)
 EVT_MOUSEWHEEL(BasicDrawPane::mouseWheelMoved)
 */
    EVT_LEFT_DOWN(MapViewer::on_leftclick)
    EVT_RIGHT_DOWN(MapViewer::on_rightdown)
    EVT_RIGHT_UP(MapViewer::on_rightup)
    EVT_MOTION(MapViewer::on_mousemotion)
    EVT_MOUSEWHEEL(MapViewer::on_mousewheel)
    EVT_KEY_DOWN(MapViewer::on_keydown)
wxEND_EVENT_TABLE()

// ****************************************************************************
// ******************************************************** MapViewer::creation
MapViewer::MapViewer(wxWindow* parent) :
    wxPanel(parent), _pArr(nullptr), _reg_viewer(nullptr),
    _origin(100,100), _scale(1.0), _cursor_pos(nullptr),
    _action(Action::NONE)
{
    _hexwidth = 2 * HEXSIZE;
    _hexheight = sqrt(3) / 2 * _hexwidth;

    for( int i=0; i<6; ++i ) {
        _hex[i] = hex_corner( HEXSIZE, i );
        _selhex[i] = hex_corner( SELSIZE, i);
        std::cout << _hex[i].x << ", " << _hex[i].y << std::endl;
    }

    // Selection
    _selected_list.clear();

    // look for proper text scaling
    wxPaintDC dc(this);
    wxSize text_size = dc.GetTextExtent( wxString("[99,99]"));
    std::cout << "text_size=" << text_size.GetWidth() << "x" << text_size.GetHeight() << std::endl;
    float text_scale = (float) HEXSIZE / (float) text_size.GetWidth();
    _hexfont = dc.GetFont();
    _hexfont = _hexfont.Scaled( text_scale );

    // Set the colors
    _color_terrain[0] = wxBrush(wxColor(102,178,255)); //OCEAN
    _color_terrain[1] = wxBrush(wxColor(255,204,153)); //PLAIN
    _color_terrain[2] = wxBrush(wxColor(0,204,0)); //FOREST,
	_color_terrain[3] = wxBrush(wxColor(224,224,224));//_MOUNTAIN,
	_color_terrain[4] = wxBrush(wxColor(0,153,153));//R_SWAM,
	_color_terrain[5] = wxBrush(wxColor(76,153,0));//R_JUNGLE,
	_color_terrain[6] = wxBrush(wxColor(255,255,153));//R_DESERT,
	_color_terrain[7] = wxBrush(wxColor(255,204,153));//R_TUNDRA,
	_color_terrain[8] = wxBrush(wxColor(153,153,255));//R_CAVERN,
	_color_terrain[9] = wxBrush(wxColor(0,204,0));//R_UFOREST,
	_color_terrain[10] = wxBrush(wxColor(178,102,255));//R_TUNNELS,
	_color_terrain[60] = wxBrush(wxColor(1,128,255));//R_LAKE
}
// ********************************************************** MapViewer::attach
void MapViewer::attach_regviewer( RegViewer* regview )
{
    _reg_viewer = regview;
}
void MapViewer::attach( ARegionArray* pArr )
{
    wxPaintDC dc(this);
//    wxBufferedDC dc(this);
    _pArr = pArr;
    if( _pArr->strName )
        std::cout << "ATTACHED name=" << *(pArr->strName) << std::endl;
    _selected_list.clear();
    if( _cursor_pos ) { _cursor_pos->x = 0; _cursor_pos->y = 0;}
    else { _cursor_pos = new wxPoint(0,0);}
    render(dc);
}
// ****************************************************************************
// ***************************************************** MapViewer::paint_event
/**
 * Called by the system of by wxWidgets when the panel needs
 * to be redrawn. You can also trigger this call by
 * calling Refresh()/Update().
 */
void MapViewer::paint_event(wxPaintEvent & evt)
{
    wxPaintDC dc(this);
//    wxAutoBufferedPaintDC dc(this);
//    wxSize dc_size = dc.GetSize();
//    wxPoint dc_ori = dc.GetDeviceOrigin();
//    double dummy;
//    dc.GetUserScale( &dummy, &dummy );
//    std::cout << "Geom ";
//    std::cout << " (" << dc_ori.x << ", " << dc_ori.y << ") ";
//    std::cout << dc_size.GetWidth() << "x" << dc_size.GetHeight();
//    std::cout << " scale=" << _scale << ", [" << dummy << "]" << std::endl;

    render(dc);
}
// ****************************************************************************
// ********************************************************** MapViewer::render
/**
 * Here we do the actual rendering. I put it in a separate
 * method so that it can work no matter what type of DC
 * (e.g. wxPaintDC or wxClientDC) is used.
 */
void MapViewer::render(wxDC&  dc)
{
    dc.Clear();
    dc.SetFont( _hexfont );
    dc.SetUserScale( _scale, _scale );

    // Draw RegionArr
    if( _pArr != nullptr) {
//        std::cout << "DRAW REGION" << std::endl;
        dc.SetBrush( wxNullBrush );
        dc.SetPen( wxPen( wxColor(0,0,0), 1 ) );
        for( int i=0; i<(_pArr->x * _pArr->y /2); i++ ) {
            ARegion* reg = _pArr->regions[i];
//            std::cout << i << "=[" << reg->xloc << "," << reg->yloc << "," << reg->zloc << "] ";
            draw_region( dc, reg );
        }
//        std::cout << std::endl;

        if( _cursor_pos ) {
//        std::cout << "DRAW _selected" << std::endl;
            dc.SetPen( wxPen( wxColor(255,0,0), 2 ) ); // 5-pixels-thick red outline
//        wxPoint center = hex_coord( _selected->x, _selected->y );
//        dc.DrawPolygon( 6, _hex, _origin.x+center.x, _origin.y+center.y, wxODDEVEN_RULE);*
            draw_cursor( dc, *_cursor_pos );
        }
        dc.SetPen( wxPen( wxColor(255,0,0), 5 ) );
        for( auto& region : _selected_list ) {
            draw_hex( dc, wxPoint( region->xloc, region->yloc), true);
        }
    }
    else {
        dc.DrawText( wxString("Pas de region"), _origin );
    }
//    // draw some text
//    dc.DrawText(wxT("Testing"), 40, 60);
//
//    // draw a circle
//    dc.SetBrush(*wxGREEN_BRUSH); // green filling
//    dc.SetPen( wxPen( wxColor(255,0,0), 5 ) ); // 5-pixels-thick red outline
//    dc.DrawCircle( wxPoint(200,100), 25 /* radius */ );
//
//    // draw a rectangle
//    dc.SetBrush(*wxBLUE_BRUSH); // blue filling
//    dc.SetPen( wxPen( wxColor(255,175,175), 10 ) ); // 10-pixels-thick pink outline
//    dc.DrawRectangle( 300, 100, 400, 200 );
//
//    // draw a line
//    dc.SetPen( wxPen( wxColor(0,0,0), 3 ) ); // black line, 3 pixels thick
//    dc.DrawLine( 300, 100, 700, 300 ); // draw line across the rectangle

    // Draw hexes
//    std::cout << "_hexwidth=" << _hexwidth << "  _hexheight=" << _hexheight << std::endl;
//    dc.SetBrush( wxNullBrush );
//    dc.SetPen( wxPen( wxColor(0,0,0), 1 ) );
//    int nb_x = 5;
//    int nb_y = 10;
//    for( int idy=0; idy<nb_y; ++idy ) {
//        int y = idy * _hexheight/2;
//        for( int idx=0; idx<nb_x; ++idx ) {
//            int x = idx * (_hexwidth * 3/2) + (idy%2)*(_hexwidth * 3/4);
////            std::cout << "plot at " << x << ", " << y << std::endl;
//            dc.DrawPolygon( 6, _hex, _origin.x+x, _origin.y+y, wxODDEVEN_RULE);
//        }
//    }
//
//    dc.SetPen( wxPen( wxColor(0,0,255), 2 ));
//    for( int idy=0; idy<5; ++idy) {
//        for( int idx=(idy%2); idx<11; idx+=2 ) {
////            wxPoint center = hex_coord( idx, idy );
////            dc.DrawPolygon( 6, _hex, _origin.x+center.x, _origin.y+center.y, wxODDEVEN_RULE);
//            draw_region( dc, wxPoint(idx,idy));
//        }
//    }



    // Look at the wxDC docs to learn how to draw other stuff
}
// ***************************************************** MapViewer::on_leftxxxx
void MapViewer::on_leftclick( wxMouseEvent& event )
{
    wxPaintDC dc(this);
//    wxAutoBufferedPaintDC dc(this);
    wxPoint pos = event.GetLogicalPosition(dc);

    std::cout << "LEFT_CLICK pos=" << pos.x << ", " << pos.y << std::endl;
    pos.x = round((double) pos.x / _scale);
    pos.y = round((double) pos.y / _scale);
    std::cout << "          _pos=" << pos.x << ", " << pos.y << std::endl;
    wxPoint coord = pos - _origin;
    int x = coord.x / (_hexwidth * 3/4);
    int y = coord.y / (_hexheight / 2 );
    std::cout << "  coord=" << coord.x << ", " << coord.y << "  -> x=" << x << " y=" << y << std::endl;
    wxPoint res = find_hexcoord( coord );
    if( _cursor_pos ) { _cursor_pos->x = res.x; _cursor_pos->y = res.y;}
    else { _cursor_pos = new wxPoint(res);}
    std::cout << "  _cursor_pos=" << _cursor_pos->x << ", " << _cursor_pos->y << std::endl;

    // Find associated hex
    if( _pArr != nullptr) {
        // If not CTRL -> clear selection
        if( event.ControlDown() == false ) {
            _selected_list.clear();
        }
        for( int i=0; i<(_pArr->x * _pArr->y /2); i++ ) {
            ARegion* reg = _pArr->regions[i];
            if( reg->xloc == _cursor_pos->x and reg->yloc == _cursor_pos->y ) {
                // ARegion
                auto reg_it = std::find( _selected_list.begin(), _selected_list.end(), reg);
                if( reg_it == _selected_list.end() ) { // not found
                    _selected_list.push_back( reg );
                }
                else {
                    _selected_list.erase(reg_it);
                }
                if( _reg_viewer ) _reg_viewer->attach( reg );
                break;
            }
        }
    }

    render(dc);
}
// **************************************************** MapViewer::on_rightxxxx
void MapViewer::on_rightdown( wxMouseEvent& event )
{
    wxPaintDC dc(this);
    wxPoint pos = event.GetLogicalPosition(dc);
    std::cout << "RIGHT_DOWN pos=" << pos.x << ", " << pos.y << std::endl;
    _action = Action::TRANSLATE;
    _old_pos = pos;
}
void MapViewer::on_rightup( wxMouseEvent& event )
{
    wxPaintDC dc(this);
    wxPoint pos = event.GetLogicalPosition(dc);
    std::cout << "RIGHT_UP pos=" << pos.x << ", " << pos.y << std::endl;
    _action = Action::NONE;
//    _origin += (pos - _old_pos);
//    dc.SetDeviceOrigin( _origin.x, _origin.y );
//    render(dc);
}
// ************************************************** MapViewer::on_mousemotion
void MapViewer::on_mousemotion( wxMouseEvent& event )
{
    if( _action == Action::TRANSLATE ) {
        if( event.Dragging() ) {
            wxPaintDC dc(this);
            wxPoint pos = event.GetLogicalPosition(dc);
            _origin.x += round((pos.x - _old_pos.x)/_scale);
            _origin.y += round((pos.y - _old_pos.y)/_scale);
            _old_pos = pos;
            //dc.SetDeviceOrigin( _origin.x, _origin.y );
            render(dc);
        }
    }
}
// *************************************************** MapViewer::on_mousewheel
void MapViewer::on_mousewheel( wxMouseEvent& event )
{
    wxPaintDC dc(this);
//    wxAutoBufferedPaintDC dc(this);


    std::cout << "MOUSE_WHEEL";
    std::cout << " delta=" << event.GetWheelDelta();
    std::cout << " rotation=" << event.GetWheelRotation();
    std::cout << std::endl;

    wxPoint pos = event.GetLogicalPosition(dc);
    double old_scale = _scale;
//    wxPoint op = pos - _origin;
//    std::cout << "  V(o,pos) =(" << op.x/_scale << ", " << op.y /_scale << ")";
//    std::cout << " O=(" << _origin.x << ", " << _origin.y << ")";
//    std::cout << " P=(" << pos.x << ", " << pos.y << ")";
//    std::cout << std::endl;

    _scale = _scale * (1.0 + (double) event.GetWheelRotation() / (event.GetWheelDelta() * SCALE_DELTA) );
    if( _scale < SCALE_MIN ) _scale = SCALE_MIN;
    if( _scale > SCALE_MAX ) _scale = SCALE_MAX;

    // move origin to center zooming on actual mouse position
    // vector( _origin -> (pos/scale)) should be constant
    _origin.x = round(pos.x * ((1.0/_scale)-(1.0/old_scale))) +_origin.x;
    _origin.y = round(pos.y * ((1.0/_scale)-(1.0/old_scale))) +_origin.y;

//    op = pos - _origin;
//    std::cout << "  NEW V(o,pos) =(" << op.x/_scale << ", " << op.y /_scale << ")";
//    std::cout << " O=(" << _origin.x << ", " << _origin.y << ")";
//    std::cout << " P=(" << pos.x << ", " << pos.y << ")";
//    std::cout << std::endl;

    //dc.SetUserScale( _scale, _scale );
    render(dc);
}
// ****************************************************** MapViewer::on_keydown
void MapViewer::on_keydown( wxKeyEvent& event )
{
    std::cout << "KEY code=" << event.GetKeyCode() << std::endl;
    bool arrow_pressed = false;
    switch( event.GetKeyCode() ) {
    case 315: //UP
        arrow_pressed = true;
        _cursor_pos->y -= 2;
        if( ((_cursor_pos->x)%2 == 0) and _cursor_pos->y < 0) _cursor_pos->y = 0;
        if( ((_cursor_pos->x)%2 == 1) and _cursor_pos->y < 1) _cursor_pos->y = 1;
        break;
    case 316: //RIGHT
        arrow_pressed = true;
        if( _cursor_pos->x < (_pArr->x-1)) {
            if( _cursor_pos->x % 2 == 0) {
                _cursor_pos->y += 1;
            }
            else {
                _cursor_pos->y -= 1;
            }
            _cursor_pos->x += 1;
        }
        break;
    case 317: //DOWN
        arrow_pressed = true;
        if( _cursor_pos->y < (_pArr->y-2) ) {
            _cursor_pos->y += 2;
        }
        break;
    case 314: //LEFT
        arrow_pressed = true;
        if( _cursor_pos->x > 0 ) {
            if( _cursor_pos->x % 2 == 0 and _cursor_pos->y <(_pArr->y) ) {
                _cursor_pos->y += 1;
            }
            else {
                _cursor_pos->y -= 1;
            }
            _cursor_pos->x -= 1;
        }
        break;
    case 32://SPACE
        _selected_list.clear();
        if( _pArr ) {
            for( int i=0; i<(_pArr->x * _pArr->y /2); i++ ) {
                ARegion* reg = _pArr->regions[i];
                if( reg->xloc == _cursor_pos->x and reg->yloc == _cursor_pos->y ) {
                    _selected_list.push_back( reg );
                    if( _reg_viewer ) _reg_viewer->attach( reg );
                    break;
                }
            }
        }
        break;
    default:
        std::cout << "unknown code=" << event.GetKeyCode() << std::endl;
    }

    // Add to seleection
    if( event.ControlDown() == true and _pArr and arrow_pressed ) {
        for( int i=0; i<(_pArr->x * _pArr->y /2); i++ ) {
            ARegion* reg = _pArr->regions[i];
            if( reg->xloc == _cursor_pos->x and reg->yloc == _cursor_pos->y ) {
                // ARegion
                auto reg_it = std::find( _selected_list.begin(), _selected_list.end(), reg);
                if( reg_it == _selected_list.end() ) { // not found
                    _selected_list.push_back( reg );
                }
                else {
                    _selected_list.erase(reg_it);
                }
                if( _reg_viewer ) _reg_viewer->attach( reg );
                break;
            }
        }
    }

    wxPaintDC dc(this);
    render(dc);
}
// ****************************************************************************
// ***************************************************** MapViewer::draw_region
void MapViewer::draw_cursor( wxDC& dc, const wxPoint& hexpos )
{
    dc.SetBrush( wxNullBrush );
    wxPoint center = hex_coord( hexpos.x, hexpos.y );
    dc.DrawCircle( _origin+center, HEXSIZE/3 );
}
void MapViewer::draw_hex( wxDC& dc, const wxPoint& hexpos, bool select )
{
    dc.SetBrush( wxNullBrush );
    wxPoint center = hex_coord( hexpos.x, hexpos.y );
    if( select ) {
        dc.DrawPolygon( 6, _selhex, _origin.x+center.x, _origin.y+center.y, wxODDEVEN_RULE);
    }
    else {
        dc.DrawPolygon( 6, _hex, _origin.x+center.x, _origin.y+center.y, wxODDEVEN_RULE);
    }
}
void MapViewer::draw_region( wxDC& dc, ARegion* reg )
{
    // Color
    auto brush_it = _color_terrain.find( reg->type );
    if( brush_it != _color_terrain.end()) {
        dc.SetBrush( brush_it->second );
    }
    else {
        dc.SetBrush( wxNullBrush );
        std::cout << "BRUSH not found for " << reg->type << " " << TerrainDefs[reg->type].name << std::endl;
    }
    dc.SetPen( wxNullPen );
    wxPoint center = hex_coord( reg->xloc, reg->yloc );
    dc.DrawPolygon( 6, _hex, _origin.x+center.x, _origin.y+center.y, wxODDEVEN_RULE);

    std::stringstream msg;
    msg << "[" << reg->xloc << "," << reg->yloc << "]";
    wxPoint textpos = center + wxPoint( 4 -HEXSIZE/2, 4 -_hexheight/2);
    dc.DrawText(wxString(msg.str()), _origin + textpos );

    if( reg->HasShaft() ) {
        dc.SetPen( *wxRED_PEN );
        draw_hex( dc, wxPoint( reg->xloc, reg->yloc));
        dc.SetBrush( *wxRED_BRUSH );
        dc.DrawCircle( _origin+center+wxPoint(-HEXSIZE/3,_hexheight/4), 5);
    }
    if( reg->gate > 0 ) {
        dc.SetPen( *wxBLUE_PEN );
        draw_hex( dc, wxPoint( reg->xloc, reg->yloc));
        dc.SetBrush( *wxBLUE_BRUSH );
        dc.DrawCircle( _origin+center+wxPoint(+HEXSIZE/3,_hexheight/4), 5);
    }
    if( reg->town ) {
        dc.SetPen( *wxBLACK_PEN );
        draw_hex( dc, wxPoint( reg->xloc, reg->yloc));
        dc.SetBrush( *wxBLACK_BRUSH );
        dc.DrawCircle( _origin+center+wxPoint(0,_hexheight/4), 5);
    }
}
// ***************************************************************** hex_corner
wxPoint MapViewer::hex_corner( int size, int index )
{
    float ang = M_PI / 3.f * index;
    return wxPoint( size * cosf(ang), size * sinf(ang) );
}
wxPoint MapViewer::hex_coord( int xhex, int yhex )
{
    int x = xhex * (_hexwidth * 3/4);
    int y = yhex * _hexheight/2;
    return wxPoint( x, y );
}
wxPoint MapViewer::find_hexcoord( wxPoint& pt )
{
    int x = pt.x / (_hexwidth * 3/4);
    int y = pt.y / (_hexheight / 2 );

//    std::cout << "find among ";
    if( (x%2) == (y%2) ) {
//        std::cout << "  (" << x << ", " << y << "):" << hex_coord( x, y ).x << ", " << hex_coord( x, y ).y;
//        std::cout << "  (" << x+1 << ", " << y+1 << "):"<< hex_coord( x+1, y+1 ).x << ", " << hex_coord( x+1, y+1 ).y;;
//        std::cout << std::endl;
        wxPoint to1 = hex_coord( x, y ) - pt;
        wxPoint to2 = hex_coord( x+1, y+1 ) - pt;
//        std::cout << "to1:(" << to1.x << ", " << to1.y << ") = " << ((to1.x)*(to1.x) + (to1.y)*(to1.y)) << std::endl;
//        std::cout << "to2:(" << to2.x << ", " << to2.y << ") = " << ((to2.x)*(to2.x) + (to2.y)*(to2.y)) << std::endl;
        if( ((to1.x)*(to1.x) + (to1.y)*(to1.y)) < ((to2.x)*(to2.x) + (to2.y)*(to2.y)) ) {
//            std::cout << std::endl << "  =>   (" << x << ", " << y << ")"<< std::endl;
            return wxPoint(x,y);
        }
        else {
//            std::cout << std::endl << "  =>   (" << x+1 << ", " << y+1 << ")"<< std::endl;
            return wxPoint(x+1,y+1);
        }
    }
    else {
//        std::cout << "  (" << x << ", " << y+1 << "):" << hex_coord( x, y+1 ).x << ", " << hex_coord( x, y+1 ).y;
//        std::cout << "  (" << x+1 << ", " << y << "):" << hex_coord( x+1, y ).x << ", " << hex_coord( x+1, y ).y;
//        std::cout << std::endl;
        wxPoint to1 = hex_coord( x, y+1 ) - pt;
        wxPoint to2 = hex_coord( x+1, y ) - pt;
//        std::cout << "to1:(" << to1.x << ", " << to1.y << ") = " << ((to1.x)*(to1.x) + (to1.y)*(to1.y)) << std::endl;
//        std::cout << "to2:(" << to2.x << ", " << to2.y << ") = " << ((to2.x)*(to2.x) + (to2.y)*(to2.y)) << std::endl;
        if( ((to1.x)*(to1.x) + (to1.y)*(to1.y)) < ((to2.x)*(to2.x) + (to2.y)*(to2.y)) ) {
//            std::cout << std::endl << "  =>   (" << x << ", " << y+1 << ")"<< std::endl;
            return wxPoint(x,y+1);
        }
        else {
//            std::cout << std::endl << "  =>   (" << x+1 << ", " << y << ")"<< std::endl;
            return wxPoint(x+1,y);
        }
    }
//    std::cout << std::endl;
}
