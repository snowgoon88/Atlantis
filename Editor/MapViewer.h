#ifndef MAPVIEWER_H
#define MAPVIEWER_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <list>
#include <algorithm>

#include <aregion.h>
#include <RegViewer.h>

#define SCALE_MIN 0.25
#define SCALE_MAX 10.0
#define SCALE_DELTA 10.0
#define HEXSIZE 30
#define SELSIZE 27
/**
 * View a map
 */
class MapViewer : public wxPanel
{
public:
    /** Creation */
    MapViewer( wxWindow *parent );

    /** Attach model */
    void attach_regviewer( RegViewer* regview );
    void attach( ARegionArray* pArr );

private:
    /** Model */
    ARegionArray* _pArr;
    RegViewer* _reg_viewer;
    /** Paint */
    void render(wxDC&  dc);

    /** coordinate */
    wxPoint _origin;
    double _scale;
    wxFont _hexfont;
    wxPoint* _cursor_pos;
    std::list<ARegion*> _selected_list;

    /** info about hexagons */
    wxCoord _hexwidth, _hexheight;
    wxPoint _hex[6], _selhex[6];

    /** movements */
    enum class Action { NONE, TRANSLATE };
    Action _action;
    wxPoint _old_pos;

    /** utility */
    void draw_cursor( wxDC& dc, const wxPoint& hexpos );
    void draw_region( wxDC& dc, ARegion* reg );
    void draw_hex( wxDC& dc, const wxPoint& hexpos, bool select=false );
    wxPoint hex_corner( int size, int index );
    wxPoint hex_coord( int xhex, int yhex );
    wxPoint find_hexcoord( wxPoint& pt );

    /** terrain */
    std::map<int,wxBrush> _color_terrain;
    wxPen* _wall_pen;

    /** Event gestion */
    void paint_event(wxPaintEvent & evt);
    void on_leftclick( wxMouseEvent& event );
    void on_rightdown( wxMouseEvent& event );
    void on_rightup( wxMouseEvent& event );
    void on_mousemotion( wxMouseEvent& event );
    void on_mousewheel( wxMouseEvent& event );
    void on_keydown( wxKeyEvent& event );
    wxDECLARE_EVENT_TABLE();
};


#endif // MAPVIEWER_H
