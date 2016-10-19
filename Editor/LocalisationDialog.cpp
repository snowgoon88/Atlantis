/**
 * Find a localisation (object in region).
 */
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <LocalisationDialog.h>
#include <LocalisationListener.h>

BEGIN_EVENT_TABLE(LocalisationDialog, wxFrame)
    EVT_CLOSE(LocalisationDialog::OnClose)
END_EVENT_TABLE()

LocalisationDialog::LocalisationDialog( wxWindow* parent, MapAccess* map_access,
                                        LocalisationListener* listener)
     : wxFrame( parent, wxID_ANY, "Localisation", wxDefaultPosition, wxSize(500,180) ),
     _parent(parent), _map_access(map_access), _listener(listener), _reg(nullptr), _obj(nullptr)
{
    wxBoxSizer* main_vbox = new wxBoxSizer( wxVERTICAL );
    //
    _reg_text = new wxStaticText( this, -1, "Region indeterminee");
    main_vbox->Add( _reg_text, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 2 );
    wxBoxSizer* reg_hbox = new wxBoxSizer( wxHORIZONTAL );
    mk_field( this, wxID_ANY, reg_hbox, "Region: x y z OR num", _reg_def, 1, 200);
    _reg_def->Bind( wxEVT_TEXT_ENTER, &LocalisationDialog::on_regdef_update, this );
    main_vbox->Add( reg_hbox, 0, wxALIGN_CENTER_VERTICAL, 2);
    //
    wxBoxSizer* obj_hbox = new wxBoxSizer( wxHORIZONTAL );
    mk_title( this, obj_hbox, "Objet: ");
    _obj_def = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxSize(300,-1) );
    obj_hbox->Add( _obj_def, 0, wxLEFT | wxALIGN_CENTER_VERTICAL, 0);
    main_vbox->Add( obj_hbox, 0, wxALIGN_CENTER_VERTICAL, 2);

    // Buttons
    wxBoxSizer* btn_hbox = new wxBoxSizer( wxHORIZONTAL );
    wxButton* ok_btn = new wxButton(this, wxOK, "Ok");
    ok_btn->Bind( wxEVT_BUTTON, &LocalisationDialog::OnOk, this);
    btn_hbox->Add( ok_btn, 0, wxRIGHT|wxLEFT, 20);
    wxButton* cancel_btn = new wxButton(this, wxCANCEL, "Cancel");
    cancel_btn->Bind( wxEVT_BUTTON, &LocalisationDialog::OnCancel, this);
    btn_hbox->Add( cancel_btn, 0, wxRIGHT|wxLEFT, 20);
    main_vbox->Add( btn_hbox, 0, wxALIGN_CENTER_VERTICAL, 2);

    this->SetSizer( main_vbox );
}
/******* LocalisationDialog::on_regdef_update */
void LocalisationDialog::on_regdef_update( wxCommandEvent& event )
{
    std::string text = _reg_def->GetValue().ToStdString();

    // Split with spaces
    std::string buf;
    std::stringstream sstr( text );
    std::vector<std::string> tokens;
    while (sstr >> buf ) {
        tokens.push_back(buf);
    }

    _reg = nullptr;
    _obj = nullptr;
    if( tokens.size() == 3) { //x, y, z
        int xloc = std::stoi( tokens[0] );
        int yloc = std::stoi( tokens[1] );
        int zloc = std::stoi( tokens[2] );
        _reg = _map_access->regions()->GetRegion( xloc, yloc, zloc );
    }
    else if( tokens.size() == 1 ) { // num}
        int regnum = std::stoi( tokens[0] );
        _reg = _map_access->regions()->GetRegion( regnum );
    }
    else {
        wxString msg("J'arrive pas a trouver la region ");
        msg << text;
        wxMessageBox( msg, "Mauvaise region");
        _reg_def->SetValue("");
        _reg_text->SetLabelText( "Region non determinee");
        _coll_obj.clear();
        _obj_def->Clear();
        return;
    }
    if( _reg == nullptr ) {
        wxString msg("J'arrive pas a trouver la region ");
        msg << text;
        wxMessageBox( msg, "Mauvaise region");
        _reg_def->SetValue("");
        _reg_text->SetLabelText( "Region non determinee");
        _coll_obj.clear();
        _obj_def->Clear();
        return;
    }
    _reg_text->SetLabelText( region_str(_reg) );

    // All the objects
    _coll_obj.clear();
    _obj_def->Clear();
    forlist (&(_reg->objects)) {
        Object *o = (Object *) elem;
        wxString obj_str;
        if( o->name )
            obj_str << "O:" << o->name->Str();
        else
            obj_str << "O:---";
        if( o->type != -1)
            obj_str << "[" << ObjectDefs[o->type].name << "]";
        else
            obj_str << "[???]";
        _obj_def->Append( obj_str );
        _coll_obj.push_back( o );
    }
    _obj_def->SetSelection(0);

}
std::string LocalisationDialog::region_str(ARegion* region)
{
    std::stringstream loc;
    loc << "region num:" << region->num;
    loc << " [" << region->xloc << " " << region->yloc << " " << region->zloc << "]";
    return loc.str();
}
/** Events */
void LocalisationDialog::OnOk( wxCommandEvent& event )
{
    if( !_reg ) {
         wxMessageBox( "Pas de region choisie");
         return;
    }
    if( _coll_obj.size() == 0) {
        wxMessageBox( "Bizarre, il n'y a pas d'Objects" );
        return;
    }
    _obj = _coll_obj[_obj_def->GetSelection()];
    std::cout << "LocalisationDialog R=" << _reg->num << " O" << *(_obj->name) << std::endl;
    // Warn parent
    Localisation loc {_reg,_obj};
    _listener->receive_localisation( loc );
    Destroy();
}
void LocalisationDialog::OnCancel( wxCommandEvent& event )
{
    // Warn parent
    Localisation loc {nullptr,nullptr};
    _listener->receive_localisation( loc );
    Destroy();
}
void LocalisationDialog::OnClose( wxCloseEvent& event )
{
    Localisation loc {nullptr,nullptr};
    _listener->receive_localisation( loc );
    Destroy();
}
