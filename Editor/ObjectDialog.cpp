/**
 * Find an Object type.
 */
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <ObjectDialog.h>
#include <ObjectListener.h>

BEGIN_EVENT_TABLE(ObjectDialog, wxFrame)
    EVT_CLOSE(ObjectDialog::OnClose)
END_EVENT_TABLE()

ObjectDialog::ObjectDialog( wxWindow* parent, MapAccess* map_access,
                            ARegion* reg, ObjectListener* listener)
     : wxFrame( parent, wxID_ANY, "Add Object", wxDefaultPosition, wxSize(500,180) ),
     _parent(parent), _map_access(map_access), _listener(listener)
{
    wxBoxSizer* main_vbox = new wxBoxSizer( wxVERTICAL );
    //
    _reg_text = new wxStaticText( this, -1, region_str(reg));
    main_vbox->Add( _reg_text, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 2 );
    wxBoxSizer* reg_hbox = new wxBoxSizer( wxHORIZONTAL );
    main_vbox->Add( reg_hbox, 0, wxALIGN_CENTER_VERTICAL, 2);
    //
    wxBoxSizer* obj_hbox = new wxBoxSizer( wxHORIZONTAL );
    mk_title( this, obj_hbox, "Type Objet: ");
    _obj_type = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxSize(300,-1) );
    for( auto& item : _map_access->_object_types ) {
        _obj_type->Append( item.first );
    }
    _obj_type->SetSelection(0);
    obj_hbox->Add( _obj_type, 0, wxLEFT | wxALIGN_CENTER_VERTICAL, 0);
    main_vbox->Add( obj_hbox, 0, wxALIGN_CENTER_VERTICAL, 2);

    // Buttons
    wxBoxSizer* btn_hbox = new wxBoxSizer( wxHORIZONTAL );
    wxButton* ok_btn = new wxButton(this, wxOK, "Ok");
    ok_btn->Bind( wxEVT_BUTTON, &ObjectDialog::OnOk, this);
    btn_hbox->Add( ok_btn, 0, wxRIGHT|wxLEFT, 20);
    wxButton* cancel_btn = new wxButton(this, wxCANCEL, "Cancel");
    cancel_btn->Bind( wxEVT_BUTTON, &ObjectDialog::OnCancel, this);
    btn_hbox->Add( cancel_btn, 0, wxRIGHT|wxLEFT, 20);
    main_vbox->Add( btn_hbox, 0, wxALIGN_CENTER_VERTICAL, 2);

    this->SetSizer( main_vbox );
}

std::string ObjectDialog::region_str(ARegion* region)
{
    std::stringstream loc;
    loc << "Dans Region num:" << region->num;
    loc << " [" << region->xloc << " " << region->yloc << " " << region->zloc << "]";
    return loc.str();
}
/** Events */
void ObjectDialog::OnOk( wxCommandEvent& event )
{
    int selid = _obj_type->GetSelection();
    int type = _map_access->_object_types[selid].second;
    if( type > 0 ) {
        _listener->receive_object( type );
        Destroy();
    }
    else {
        wxMessageBox( "Peut pas faire des Objets de type 'None'", "Add Object");
    }
}
void ObjectDialog::OnCancel( wxCommandEvent& event )
{
    // Warn parent
    _listener->receive_object( -1 );
    Destroy();
}
void ObjectDialog::OnClose( wxCloseEvent& event )
{
    _listener->receive_object( -1 );
    Destroy();
}
