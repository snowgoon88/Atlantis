#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <ListChooser.h>
#include <ListElement.h>

ListElement::ListElement( wxWindow* parent, ListChooser* chooser)
  : wxPanel(parent), _chooser(chooser)
{
        _hbox = new wxBoxSizer( wxHORIZONTAL );
        _type = new wxChoice( this, wxID_ANY );
        for( auto& item : _chooser->_ref )
        {
            _type->Append( item.first );
        }
        _type->SetSelection(0);
        _hbox->Add( _type, 0, wxLEFT | wxALIGN_CENTER_VERTICAL, 20);
        mk_spin( this, wxID_ANY, _hbox, "", _number, 0, 1000000, 150);
        _del_btn = new wxButton( this, wxID_ANY, "DEL");
        _del_btn->Bind( wxEVT_BUTTON, &ListElement::OnDel, this);
        _hbox->Add( _del_btn, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 2);
        this->SetSizer( _hbox );
}

void ListElement::OnDel( wxCommandEvent& event )
{
  _chooser->RemoveElement( this );
}
/** Set Selection */
void ListElement::set_type_bynum( int num )
{
    // look for num
    for( int id=0; id < _chooser->_ref.size(); ++id ) {
        if( _chooser->_ref[id].second == num ) {
            _type->SetSelection(id);
            return;
        }
    }
    _type->SetSelection(0);
}
void ListElement::set_number( int number )
{
    _number->SetValue( number );
}
int ListElement::get_type_int()
{
    return _chooser->_ref[_type->GetSelection()].second;
}
int ListElement::get_number()
{
    return _number->GetValue();
}

