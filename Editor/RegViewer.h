#ifndef REGVIEWER_H
#define REGVIEWER_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <aregion.h>
#include <observer.h>
#include <RegionData.h>
#include <wx/treelist.h>

/**
 * View a ARegion
 */
class RegViewer : public wxPanel, public Observer
{
public:
    /** Creation */
    RegViewer( wxWindow *parent, RegionData& model );

    /** Attach model */
    void attach( ARegion* reg);
    void reset();

    /** Override as Observer */
    void update( int signal=0 );

    /** Tree with selection */
    wxTreeListCtrl* _obj_tree;
    /** Get selected Unit* or nullptr */
    Unit* getSelUnit();
    Object* getSelObject();
private:
    /** Model */
    RegionData& _model;
    ARegion* _reg;

    /** Widgets */
    wxStaticText* _info_text;
    wxStaticText* _name_text;
    wxStaticText* _terrain_text;
    wxStaticText* _gate_text;
    wxStaticText* _shaft_text;
    wxStaticText* _town_text;
    wxStaticText* _pop_text;
    wxStaticText* _obj_text;



    /** Event gestion */
    void update_values();
    wxDECLARE_EVENT_TABLE();
};

void mk_title(wxWindow *parent, wxBoxSizer *sizer, std::string title,
              wxStaticText*& wxtext);
#endif // REGVIEWER_H
