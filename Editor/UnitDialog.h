#ifndef UNITDIALOG_H
#define UNITDIALOG_H

/**
 * Return an Unit*.
 */
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <unit.h>
#include <map_access.h>
#include <UnitListener.h>

class UnitDialog : public wxFrame
{
public:
    UnitDialog( wxWindow* parent, MapAccess* map_access,
                       UnitListener* listener);

    /** Attributs */
    wxWindow* _parent;
    MapAccess* _map_access;
    UnitListener* _listener;
    wxStaticText* _unit_text;
    wxTextCtrl* _unit_def;

    /** Events */
    void on_unitdef_update( wxCommandEvent& event );
    void OnOk( wxCommandEvent& event );
    void OnCancel( wxCommandEvent& event);
    void OnClose(wxCloseEvent& event);

private:
    Unit* _unit;
    Object*  _obj;
    std::string unit_str(Unit* unit);

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
    // Events
    DECLARE_EVENT_TABLE()
};

#endif // UNITDIALOG_H
