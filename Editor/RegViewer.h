#ifndef REGVIEWER_H
#define REGVIEWER_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <aregion.h>

/**
 * View a ARegion
 */
class RegViewer : public wxPanel
{
public:
    /** Creation */
    RegViewer( wxWindow *parent );

    /** Attach model */
    void attach( ARegion* reg);

private:
    /** Model */
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
    wxDECLARE_EVENT_TABLE();
};

void mk_title(wxWindow *parent, wxBoxSizer *sizer, std::string title,
              wxStaticText*& wxtext);
#endif // REGVIEWER_H
