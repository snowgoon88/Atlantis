#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include <MapViewer.h>
#include <math.h>

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
wxEND_EVENT_TABLE()

// ****************************************************************************
// ******************************************************** MapViewer::creation
MapViewer::MapViewer(wxWindow* parent) :
    wxPanel(parent), _origin(100,100), _scale(1.0), _selected(nullptr),
    _hexsize(30), _action(Action::NONE)
{
    _hexwidth = 2 * _hexsize;
    _hexheight = sqrt(3) / 2 * _hexwidth;

    for( int i=0; i<6; ++i ) {
        _hex[i] = hex_corner( _hexsize, i );
        std::cout << _hex[i].x << ", " << _hex[i].y << std::endl;
    }
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

    wxSize dc_size = dc.GetSize();
    wxPoint dc_ori = dc.GetDeviceOrigin();
    double dummy;
    dc.GetUserScale( &dummy, &dummy );
    std::cout << "Geom ";
    std::cout << " (" << dc_ori.x << ", " << dc_ori.y << ") ";
    std::cout << dc_size.GetWidth() << "x" << dc_size.GetHeight();
    std::cout << " scale=" << _scale << ", [" << dummy << "]" << std::endl;


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
    dc.SetUserScale( _scale, _scale );
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
    dc.SetBrush( wxNullBrush );
    dc.SetPen( wxPen( wxColor(0,0,0), 1 ) );
    int nb_x = 5;
    int nb_y = 10;
    for( int idy=0; idy<nb_y; ++idy ) {
        int y = idy * _hexheight/2;
        for( int idx=0; idx<nb_x; ++idx ) {
            int x = idx * (_hexwidth * 3/2) + (idy%2)*(_hexwidth * 3/4);
//            std::cout << "plot at " << x << ", " << y << std::endl;
            dc.DrawPolygon( 6, _hex, _origin.x+x, _origin.y+y, wxODDEVEN_RULE);
        }
    }

    dc.SetPen( wxPen( wxColor(0,0,255), 2 ));
    for( int idy=0; idy<5; ++idy) {
        for( int idx=(idy%2); idx<11; idx+=2 ) {
            wxPoint center = hex_coord( idx, idy );
            dc.DrawPolygon( 6, _hex, _origin.x+center.x, _origin.y+center.y, wxODDEVEN_RULE);
        }
    }

    if( _selected ) {
//        std::cout << "DRAW _selected" << std::endl;
        dc.SetPen( wxPen( wxColor(255,0,0), 5 ) ); // 5-pixels-thick red outline
        wxPoint center = hex_coord( _selected->x, _selected->y );
        dc.DrawPolygon( 6, _hex, _origin.x+center.x, _origin.y+center.y, wxODDEVEN_RULE);
    }

    // Look at the wxDC docs to learn how to draw other stuff
}
// ***************************************************** MapViewer::on_leftxxxx
void MapViewer::on_leftclick( wxMouseEvent& event )
{
    wxPaintDC dc(this);
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
    _selected = new wxPoint( res );
    std::cout << "  _selected=" << _selected->x << ", " << _selected->y << std::endl;

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
            _origin += (pos - _old_pos);
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

    std::cout << "MOUSE_WHEEL";
    std::cout << " delta=" << event.GetWheelDelta();
    std::cout << " rotation=" << event.GetWheelRotation();
    std::cout << std::endl;

    _scale = _scale * (1.0 + (double) event.GetWheelRotation() / (event.GetWheelDelta() * SCALE_DELTA) );
    if( _scale < SCALE_MIN ) _scale = SCALE_MIN;
    if( _scale > SCALE_MAX ) _scale = SCALE_MAX;

    //dc.SetUserScale( _scale, _scale );
    render(dc);
}
// ****************************************************************************
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
