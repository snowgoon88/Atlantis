#ifndef LISTCHOOSER_H
#define LISTCHOOSER_H

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <vector>
#include <list>
#include <wx/spinctrl.h>

using RefItem = std::pair<std::string,int>;
class ListElement;
class ListChooser : public wxPanel
{
public:
  ListChooser( wxWindow* parent, const std::string& title, std::vector<RefItem>& ref );
  void AddElement( int type, int number );
  void RemoveElement( ListElement* elem );

  /** Attributs */
  wxWindow* _parent;
  std::vector<RefItem>& _ref;
  wxStaticText* _title_label;
  wxButton* _add_btn;
  std::list<ListElement*> _elements;

  wxScrolledWindow* _elem_scrollpane;
  wxBoxSizer* _elem_vbox;

private:
    /** Add another ListElement to the list */
  void OnAddElement( wxCommandEvent& event );

  /** Easy Maker : add a static title */
  void mk_title(wxWindow *parent, wxBoxSizer *sizer, std::string title)
  {
	wxStaticText *label = new wxStaticText( parent, -1, _(title));
	sizer->Add( label, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 2 );
  }
};

#endif // LISTCHOOSER
