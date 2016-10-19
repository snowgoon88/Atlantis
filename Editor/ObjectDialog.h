#ifndef OBJECTDIALOG_H
#define OBJECTDIALOG_H

/**
 * Return a int (type d'Object)>.
 */
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <aregion.h>
#include <object.h>
#include <map_access.h>
#include <vector>
#include <ObjectListener.h>

class ObjectDialog : public wxFrame
{
public:
    ObjectDialog( wxWindow* parent, MapAccess* map_access, ARegion* reg,
                       ObjectListener* listener);

    /** Attributs */
    wxWindow* _parent;
    MapAccess* _map_access;
    ObjectListener* _listener;
    wxStaticText* _reg_text;
    wxChoice* _obj_type;

    /** Events */
    void OnOk( wxCommandEvent& event );
    void OnCancel( wxCommandEvent& event);
    void OnClose(wxCloseEvent& event);

private:
    /** Region to text */
    std::string region_str(ARegion* reg);

    /** Make GUI building easier */
    void mk_title(wxWindow *parent, wxBoxSizer *sizer, std::string title)
    {
        wxStaticText *label = new wxStaticText( parent, -1, _(title));
        sizer->Add( label, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 2 );
    }    // Events
    DECLARE_EVENT_TABLE()
};

#endif // OBJECTDIALOG_H
