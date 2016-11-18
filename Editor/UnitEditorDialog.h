#ifndef UNITEDITORDIALOG_H
#define UNITEDITORDIALOG_H

/**
 * Edit a Unit.
 */
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/spinctrl.h>
#include <ListChooser.h>
#include <map_access.h>

class EditorFrame;
class UnitEditorDialog : public wxFrame
{
public:
    UnitEditorDialog( wxWindow* parent, Unit* unit, MapAccess* map_access);

    /** Attributs */
    EditorFrame* _parent;
    Unit* _unit;
    MapAccess* _map_access;
    wxTextCtrl* _name;
    wxChoice* _faction;
    wxChoice* _type;
    //wxButton* _reg_btn;
    ListChooser* _list_items;
    ListChooser* _list_skills;

    /** Events */
    void OnSave( wxCommandEvent& event );
    void OnCancel( wxCommandEvent& event);
    void OnClose(wxCloseEvent& event);

private:
    std::string getUnitLocation(Unit* u);
    int find_selfaction_bynum( int fnum );
    int find_seltype_bynum( int tnum );

    /** Make GUI building easier */
    void mk_title(wxWindow *parent, wxBoxSizer *sizer, std::string title)
    {
        wxStaticText *label = new wxStaticText( parent, -1, _(title));
        sizer->Add( label, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 2 );
    }
    void mk_field(wxWindow *parent, int id, wxBoxSizer *sizer, std::string title,
              wxTextCtrl*& wxtext, int proportionnal, int width )
    {
        wxStaticText *label = new wxStaticText( parent, wxID_ANY, _(title));
        sizer->Add( label, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 2 );
        wxtext = new wxTextCtrl( parent, id, "", wxDefaultPosition, wxSize(width,-1), wxTE_PROCESS_ENTER);
        sizer->Add( wxtext, proportionnal, wxEXPAND | wxRIGHT | wxALIGN_CENTER_VERTICAL, 20);
    }
    void mk_spin(wxWindow *parent, int id, wxBoxSizer* sizer, std::string title,
             wxSpinCtrl*& wxspin, int rmin, int rmax, int width)
    {
        wxStaticText *label = new wxStaticText( parent, -1, _(title));
        sizer->Add( label, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 2);
        wxspin = new wxSpinCtrl( parent, id, "", wxDefaultPosition, wxSize(width,-1), wxTE_PROCESS_ENTER);
        wxspin->SetRange(rmin,rmax);
        wxspin->SetValue( 0 );
        sizer->Add( wxspin, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 20);
    }

    // Events
    DECLARE_EVENT_TABLE()
};

#endif // UNITEDITORDIALOG_H
