/**
 * Find a Unit.
 */
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <UnitDialog.h>
#include <UnitListener.h>

BEGIN_EVENT_TABLE(UnitDialog, wxFrame)
    EVT_CLOSE(UnitDialog::OnClose)
END_EVENT_TABLE()

UnitDialog::UnitDialog( wxWindow* parent, MapAccess* map_access,
                                        UnitListener* listener)
     : wxFrame( parent, wxID_ANY, "Unit", wxDefaultPosition, wxSize(500,180) ),
     _parent(parent), _map_access(map_access), _listener(listener), _unit(nullptr)
{
    wxBoxSizer* main_vbox = new wxBoxSizer( wxVERTICAL );
    //
    _unit_text = new wxStaticText( this, -1, "Unit indeterminee");
    main_vbox->Add( _unit_text, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 2 );
    wxBoxSizer* unit_hbox = new wxBoxSizer( wxHORIZONTAL );
    mk_field( this, wxID_ANY, unit_hbox, "Unit: num", _unit_def, 1, 200);
    _unit_def->Bind( wxEVT_TEXT_ENTER, &UnitDialog::on_unitdef_update, this );
    main_vbox->Add( unit_hbox, 0, wxALIGN_CENTER_VERTICAL, 2);
    //

    // Buttons
    wxBoxSizer* btn_hbox = new wxBoxSizer( wxHORIZONTAL );
    wxButton* ok_btn = new wxButton(this, wxOK, "Ok");
    ok_btn->Bind( wxEVT_BUTTON, &UnitDialog::OnOk, this);
    btn_hbox->Add( ok_btn, 0, wxRIGHT|wxLEFT, 20);
    wxButton* cancel_btn = new wxButton(this, wxCANCEL, "Cancel");
    cancel_btn->Bind( wxEVT_BUTTON, &UnitDialog::OnCancel, this);
    btn_hbox->Add( cancel_btn, 0, wxRIGHT|wxLEFT, 20);
    main_vbox->Add( btn_hbox, 0, wxALIGN_CENTER_VERTICAL, 2);

    this->SetSizer( main_vbox );
}
/******* UnitDialog::on_unitdef_update */
void UnitDialog::on_unitdef_update( wxCommandEvent& event )
{
    std::string text = _unit_def->GetValue().ToStdString();

    // Split with spaces
    std::string buf;
    std::stringstream sstr( text );
    std::vector<std::string> tokens;
    while (sstr >> buf ) {
        tokens.push_back(buf);
    }

    _unit = nullptr;
    if( tokens.size() == 1 ) { // num}
        std::stringstream t0(tokens[0]);
        int unitnum; t0 >> unitnum;
        //int regnum = std::stoi( tokens[0] );
        _unit = _map_access->get_unit( unitnum );
    }
    else {
        wxString msg("J'arrive pas a trouver l'Unit ");
        msg << text;
        wxMessageBox( msg, "Mauvaise Unit");
        _unit_def->SetValue("");
        _unit_text->SetLabelText( "Unit non determinee");
        return;
    }
    if( _unit == nullptr ) {
        wxString msg("J'arrive pas a trouver l'Unit ");
        msg << text;
        wxMessageBox( msg, "Mauvaise Unit");
        _unit_def->SetValue("");
        _unit_text->SetLabelText( "Unit non determinee");
        return;
    }
    _unit_text->SetLabelText( unit_str(_unit) );

}
std::string UnitDialog::unit_str(Unit* unit)
{
    std::stringstream unitstr;
    unitstr << "unit:" << unit->name->Str();
    if( unit->faction->name)
        unitstr  << "(" << unit->faction->name->Str() << ")";// (" << u->faction->num << ")";
    else
        unitstr  << " (????)";
    return unitstr.str();
}
/** Events */
void UnitDialog::OnOk( wxCommandEvent& event )
{
    if( !_unit ) {
         wxMessageBox( "Pas de Unit choisie");
         return;
    }
    // Warn parent
    _listener->receive_unit( _unit );
    Destroy();
}
void UnitDialog::OnCancel( wxCommandEvent& event )
{
    // Warn parent
    _unit = nullptr;
    _listener->receive_unit( _unit );
    Destroy();
}
void UnitDialog::OnClose( wxCloseEvent& event )
{
    // Warn parent
    _unit = nullptr;
    _listener->receive_unit( _unit );
    Destroy();
}
