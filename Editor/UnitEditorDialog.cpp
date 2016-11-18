
/**
 * Edit a Unit.
 */
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <UnitEditorDialog.h>
#include <EditorMain.h>
#include <ListElement.h>
#include <astring.h>
#include <faction.h>

BEGIN_EVENT_TABLE(UnitEditorDialog, wxFrame)
    EVT_CLOSE(UnitEditorDialog::OnClose)
END_EVENT_TABLE()

UnitEditorDialog::UnitEditorDialog( wxWindow* parent, Unit* unit, MapAccess* map_access)
     : wxFrame( parent, wxID_ANY, "Unit Editor", wxDefaultPosition, wxSize(650,650) ),
     _parent((EditorFrame*)parent), _unit(unit), _map_access(map_access)
{
    wxBoxSizer* main_vbox = new wxBoxSizer( wxVERTICAL );
    // Num
    wxString unit_id;
    unit_id << "Unit no: " << unit->num;
    mk_title( this, main_vbox, unit_id.ToStdString());
    // Name
    wxBoxSizer* name_hbox = new wxBoxSizer( wxHORIZONTAL );
    mk_field( this, wxID_ANY, name_hbox, "Nom:", _name, 1, 250);
    // remove unit number in parenthesis
    std::string fullname(unit->name->Str());
    auto pospar = fullname.find_first_of( "(" );

    _name->SetValue( wxString( fullname.substr(0,pospar-1) ));
    main_vbox->Add( name_hbox, 0, wxALIGN_CENTER_VERTICAL, 2);
    // Faction
    wxBoxSizer* fac_hbox = new wxBoxSizer( wxHORIZONTAL );
    mk_title( this, fac_hbox, "Faction: ");
    _faction = new wxChoice( this, wxID_ANY );
    for( auto& item : _map_access->_faction_enum ){
        _faction->Append( item.first );
    }
    int fnum = find_selfaction_bynum( _unit->faction->num );
    if( fnum >= 0 ) {
        _faction->SetSelection( fnum );
    }
    else {
        wxMessageBox( "Oups, pas de faction connue pour cette unit. J'met 1...", "Edit Unit");
    }
    fac_hbox->Add( _faction, 0, wxLEFT | wxALIGN_CENTER_VERTICAL, 0);
    main_vbox->Add( fac_hbox, 0, wxALIGN_CENTER_VERTICAL, 2);
    // Localisation
    wxBoxSizer* reg_hbox = new wxBoxSizer( wxHORIZONTAL );
    mk_title( this, reg_hbox, getUnitLocation(_unit));
//    _reg_btn = new wxButton( this, wxID_ANY, "Modifier");
//    reg_hbox->Add( _reg_btn, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 0);
    //mk_spin( this, wxID_ANY, reg_hbox, "Region:", _reg_num, 0, 20000, 100);
    main_vbox->Add( reg_hbox, 0, wxALIGN_CENTER_VERTICAL, 2);
    // Type
    wxBoxSizer* type_hbox = new wxBoxSizer( wxHORIZONTAL );
    mk_title( this, type_hbox, "Type: ");
    _type = new wxChoice( this, wxID_ANY );
    for( auto& item : _map_access->_unit_types ) {
        _type->Append( item.first );
    }
    int tnum = find_seltype_bynum( _unit->type );
    if( tnum >= 0 ) {
        _type->SetSelection( tnum );
    }
    else {
        wxMessageBox( "Oups, pas de type connu pour cette unit. J'met 0...", "Edit Unit");
    }
    //_type->SetSelection(0);
    type_hbox->Add( _type, 0, wxLEFT | wxALIGN_CENTER_VERTICAL, 0);
    main_vbox->Add( type_hbox, 0, wxALIGN_CENTER_VERTICAL, 2);
    // Items
    _list_items = new ListChooser(this, "Items x number", _map_access->_item_names);
    if( _unit->items.Num() > 0) {
        Item *it = (Item *) _unit->items.First();
        do {
            _list_items->AddElement( it->type, it->num );
            it = (Item *) _unit->items.Next( it );
        } while( it );
    }
    main_vbox->Add( _list_items, 1, wxEXPAND | wxRIGHT | wxALIGN_CENTER_VERTICAL, 2);
    // Skills
    _list_skills = new ListChooser(this, "Skills x days", _map_access->_skill_names);
    if( _unit->skills.Num() > 0) {
        Skill *it = (Skill *) _unit->skills.First();
        do {
            _list_skills->AddElement( it->type, it->days / _unit->GetMen());
            it = (Skill *) _unit->skills.Next( it );
        } while( it );
    }
    main_vbox->Add( _list_skills, 1, wxEXPAND | wxRIGHT | wxALIGN_CENTER_VERTICAL, 2);

    // Buttons
    wxBoxSizer* btn_hbox = new wxBoxSizer( wxHORIZONTAL );
    wxButton* save_btn = new wxButton(this, wxOK, "Save");
    save_btn->Bind( wxEVT_BUTTON, &UnitEditorDialog::OnSave, this);
    btn_hbox->Add( save_btn, 0, wxRIGHT|wxLEFT, 20);
    wxButton* cancel_btn = new wxButton(this, wxCANCEL, "Cancel");
    cancel_btn->Bind( wxEVT_BUTTON, &UnitEditorDialog::OnCancel, this);
    btn_hbox->Add( cancel_btn, 0, wxRIGHT|wxLEFT, 20);
    main_vbox->Add( btn_hbox, 0, wxALIGN_CENTER_VERTICAL, 2);


    this->SetSizer( main_vbox );
}

/****************************************** UnitEditorDialog::getUnitLocation */
std::string UnitEditorDialog::getUnitLocation(Unit* u)
{
    std::stringstream loc;
    loc << "Emplacement => R:" << u->object->region->num << " O:" << u->object->name->Str();
    return loc.str();
}
/****************************************** UnitEditorDialog::getUnitLocation */
/************************************ UnitEditorDialog::find_selfaction_bynum */
int UnitEditorDialog::find_selfaction_bynum( int fnum )
{
    for( int id=0; id < _map_access->_faction_enum.size(); ++id ) {
        if( _map_access->_faction_enum[id].second == fnum ) {
            return id;
        }
    }
    return -1;
}
int UnitEditorDialog::find_seltype_bynum( int tnum )
{
    for( int id=0; id < _map_access->_unit_types.size(); ++id ) {
        if( _map_access->_unit_types[id].second == tnum ) {
            return id;
        }
    }
    return -1;
}
/*************************************************** UnitEditorDialog::Events */
void UnitEditorDialog::OnSave( wxCommandEvent& event)
{
    // Display everything that is set "anew"
    std::cout << "UNIT_name    : " << _name->GetValue().ToStdString() << std::endl;
    AString* newname = new AString( _name->GetValue().ToAscii() );
    _unit->SetName( newname );

    std::cout << "UNIT_faction : " << _map_access->_faction_enum[_faction->GetSelection()].first;
    std::cout << " num=" << _map_access->_faction_enum[_faction->GetSelection()].second << std::endl;
    Faction* fac = _map_access->get_faction( _map_access->_faction_enum[_faction->GetSelection()].second );
    //Faction *fac = {Game::}GetFaction(&factions, fnum);
    if (fac) _unit->faction = fac;
    //else Awrite("Cannot Find Faction");
    std::cout << "UNIT_type    : " << _map_access->_unit_types[_type->GetSelection()].first;
    std::cout << " num=" << _map_access->_unit_types[_type->GetSelection()].second << std::endl;
    _unit->type = _map_access->_unit_types[_type->GetSelection()].second;
    //if (newtype<0 || newtype>NUNITTYPES-1) {
	//					Awrite("Invalid Type");
	//					break;
	//				}
	//				pUnit->type = newtype;
    std::cout << "UNIT_Region  : TODO " << std::endl;
    std::cout << "UNIT_Items ***" << std::endl;
    _unit->items.DeleteAll();
    for( auto& item : _list_items->_elements ) {
        int inum = item->get_type_int();
        int number = item->get_number();
        //std::cout << "     " << _map_access->_item_names[item->get_type_int()].first;
//        std::cout << " num=" << inum;
//        std::cout << " #=" << number << std::endl;
        _unit->items.SetNum( inum, number );
        _unit->faction->items.SetNum( inum, 1);
    }
//    int itemNum = ParseAllItems(pToken);
//				if (itemNum == -1) {
//					Awrite("No such item.");
//pUnit->items.SetNum(itemNum, num);
//				/* Mark it as known about for 'shows' */
//				pUnit->faction->items.SetNum(itemNum, 1);
    std::cout << "UNIT_Skills **" << std::endl;
    _unit->skills.DeleteAll();
    for( auto& item : _list_skills->_elements ) {
        int snum = item->get_type_int();
        int days = item->get_number();
//        std::cout << "     " << _map_access->_skill_names[item->get_type_int()].first;
//        std::cout << " num=" << _map_access->_skill_names[item->get_type_int()].second;
//        std::cout << " #=" << item->get_number() << std::endl;
        //int skillnum = _map_access->_skill_names[item->get_type_int()].second;
        if( (SkillDefs[snum].flags & SkillType::MAGIC) &&
            (_unit->type != U_MAGE)) {
            _unit->type = U_MAGE;
        }
        if( (SkillDefs[snum].flags & SkillType::APPRENTICE) &&
            (_unit->type == U_NORMAL)) {
            _unit->type = U_APPRENTICE;
        }
        _unit->skills.SetDays(snum, days * _unit->GetMen());
        int lvl = _unit->GetRealSkill(snum);
        if (lvl > _unit->faction->skills.GetDays(snum)) {
            _unit->faction->skills.SetDays(snum, lvl);
        }
    }
//    				int skillNum = ParseSkill(pToken);
//    						if ((SkillDefs[skillNum].flags & SkillType::MAGIC) &&
//						(pUnit->type != U_MAGE)) {
//					pUnit->type = U_MAGE;
//				}
//				if ((SkillDefs[skillNum].flags & SkillType::APPRENTICE) &&
//						(pUnit->type == U_NORMAL)) {
//					pUnit->type = U_APPRENTICE;
//				}
//				pUnit->skills.SetDays(skillNum, days * pUnit->GetMen());
//				int lvl = pUnit->GetRealSkill(skillNum);
//				if (lvl > pUnit->faction->skills.GetDays(skillNum)) {
//					pUnit->faction->skills.SetDays(skillNum, lvl);
//				}
    // _chooser->_ref[_type->GetSelection()].second;
    _parent->enable_treeMenu( true );
    Destroy();
}
void UnitEditorDialog::OnCancel( wxCommandEvent& event)
{
    _parent->enable_treeMenu( true );
    Destroy();
}
void UnitEditorDialog::OnClose( wxCloseEvent& event )
{
    _parent->enable_treeMenu( true );
    Destroy();
}
/****************************************** UnitEditorDialog::getUnitLocation */
/****************************************** UnitEditorDialog::getUnitLocations */
