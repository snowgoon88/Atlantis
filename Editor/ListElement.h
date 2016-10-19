#ifndef LISTELEMENT_H
#define LISTELEMENT_H

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <vector>
#include <list>
#include <wx/spinctrl.h>

class ListChooser;
/**
 * List Element displays ItemChooser : Nb : Del.
 *  - ItemChooser is base on a collection of (wxString,int) (as "I_ITEM",valof(I_ITEM)).
 *  - nb : spin to choose the number of items
 *  - Del : a button to ask for the removal/deletion of ListElement
 */
class ListElement : public wxPanel
{
public:
  ListElement( wxWindow* parent, ListChooser* chooser);
  /** SetSelection */
  void set_type_bynum( int num );
  void set_number( int number );
  /** Get Values */
  int get_type_int();
  int get_number();

private:
    /** Attributs */
    ListChooser* _chooser;
    wxChoice* _type;
    wxSpinCtrl* _number;
    wxButton* _del_btn;
    wxBoxSizer* _hbox;
    //std::vector<RefItem> _coll_item = {{"UN",1}, {"TROIS",3}, {"SEPT",7}};

    void OnDel( wxCommandEvent& event );

    /** Easy maker of SpinCtrl */
    void mk_spin(wxWindow *parent, int id, wxBoxSizer* sizer, std::string title,
             wxSpinCtrl*& wxspin, int rmin, int rmax, int width)
    {
        //wxStaticText *label = new wxStaticText( parent, -1, _(title));
        //sizer->Add( label, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 2);
        wxspin = new wxSpinCtrl( parent, id, "", wxDefaultPosition, wxSize(width,-1), wxTE_PROCESS_ENTER);
        wxspin->SetRange(rmin,rmax);
        wxspin->SetValue( 0 );
        sizer->Add( wxspin, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 2);
    }
};
#endif // LISTELEMENT
