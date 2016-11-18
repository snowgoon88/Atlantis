#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <ListElement.h>
#include <ListChooser.h>

ListChooser::ListChooser( wxWindow* parent, const std::string& title, std::vector<RefItem>& ref)
  : wxPanel(parent,wxID_ANY, wxDefaultPosition, wxDefaultSize,
                    wxTAB_TRAVERSAL|wxBORDER_SIMPLE),
   _parent(parent), _ref(ref)
{
    this->SetMaxSize( wxSize(-1, 300) );
        // GUI
        wxBoxSizer* main_vbox = new wxBoxSizer( wxVERTICAL );
        // Title and others
        wxBoxSizer* title_hbox = new wxBoxSizer( wxHORIZONTAL );
        mk_title( this, title_hbox, title);
        _add_btn = new wxButton( this, wxID_ANY, "ADD");
        _add_btn->Bind( wxEVT_BUTTON, &ListChooser::OnAddElement, this);
        title_hbox->Add( _add_btn, 0, wxLEFT | wxALIGN_CENTER_VERTICAL| wxALIGN_RIGHT, 20);
        main_vbox->Add( title_hbox, 0, wxALIGN_CENTER_VERTICAL, 2);
        // Elements
        _elem_scrollpane = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                    wxTAB_TRAVERSAL|wxVSCROLL);
        _elem_scrollpane->SetMinSize( wxSize(50,30));
        _elem_vbox = new wxBoxSizer( wxVERTICAL );
        _elem_scrollpane->SetSizer( _elem_vbox );
        _elem_scrollpane->SetScrollRate(5, 5);
        main_vbox->Add( _elem_scrollpane, 1, wxEXPAND | wxLEFT, 25);

        this->SetSizer( main_vbox );
        this->FitInside();
        //this->SetScrollRate(5, 5);
        // List
        _elements.clear();
}
void ListChooser::AddElement( int type, int number )
{
    ListElement* elem = new ListElement(_elem_scrollpane, this );
    elem->set_type_bynum( type );
    elem->set_number( number );
    _elem_vbox->Add( elem, 0, wxALIGN_CENTER_VERTICAL, 2);
    //_elem_vbox->Layout();
    this->Layout();
    this->FitInside();
    //_elem_vbox->Fit(this);
    _parent->PostSizeEvent();
    //_parent->Refresh(false);
    //_parent->Update();
    _elements.push_back( elem );
}
void ListChooser::RemoveElement( ListElement* elem )
{
  std::cout << "Remove ListElement id=" << elem->get_type_int() << " nb=" << elem->get_number() << std::endl;
        _elem_vbox->Detach( elem );
        this->Layout();
        _parent->PostSizeEvent();
        _elements.remove( elem );
        delete elem;
}

void ListChooser::OnAddElement( wxCommandEvent& event )
{
    std::cout << "Add ListElement " << std::endl;
    AddElement( 0, 0 );
}
