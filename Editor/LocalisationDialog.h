#ifndef LOCALISATIONDIALOG_H
#define LOCALISATIONIDIALOG_H

/**
 * Return a std::pair<ARegion*,Objet*>.
 */
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <aregion.h>
#include <object.h>
#include <map_access.h>
#include <vector>
#include <LocalisationListener.h>

using Localisation= std::pair<ARegion*,Object*>;

class LocalisationDialog : public wxFrame
{
public:
    LocalisationDialog( wxWindow* parent, MapAccess* map_access,
                       LocalisationListener* listener);

    /** Attributs */
    wxWindow* _parent;
    MapAccess* _map_access;
    LocalisationListener* _listener;
    wxStaticText* _reg_text;
    wxTextCtrl* _reg_def;
    wxChoice* _obj_def;
    std::vector<Object*> _coll_obj;

    /** Events */
    void on_regdef_update( wxCommandEvent& event );
    void OnOk( wxCommandEvent& event );
    void OnCancel( wxCommandEvent& event);
    void OnClose(wxCloseEvent& event);

private:
    ARegion* _reg;
    Object*  _obj;
    /** Region to text */
    std::string region_str(ARegion* reg);

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

#endif // LOCALISATIONDIALOG_H
