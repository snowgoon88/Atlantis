#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "ItemView.h"
#include <items.h>
#include <skills.h>
#include <algorithm>
#include <sstream>
#include <iomanip>

wxBEGIN_EVENT_TABLE(ItemView, wxPanel)
    EVT_COMBOBOX(idAbbrCombo, ItemView::on_cbox_update)
    EVT_TEXT(idAbbrCombo, ItemView::on_cbox_update)
    EVT_TEXT_ENTER(idAbbrCombo, ItemView::on_cbox_update)
wxEND_EVENT_TABLE()

ItemView::ItemView(wxWindow *parent, AllData& data)
    : wxPanel( parent ), _data(data), _item(nullptr)
{
    // main Vertical
    wxBoxSizer *main_vbox = new wxBoxSizer( wxVERTICAL );

    // Identification
    wxBoxSizer *id_hbox = new wxBoxSizer( wxHORIZONTAL );
    mk_title(this, id_hbox, "Identifiant : ");
    _abbr_combo = new wxComboBox(this, idAbbrCombo, _T(""), wxDefaultPosition, wxDefaultSize,
                                0, NULL, wxTE_PROCESS_ENTER | wxTE_PROCESS_TAB);

    // add all labels
    for( auto& item : _data._all_items ) {
        _abbr_combo->Append( wxString( item.second._item_enum ));
    }
    id_hbox->Add( _abbr_combo, 0, wxEXPAND | wxRIGHT | wxALIGN_CENTER_VERTICAL, 20);

    mk_check( this, wxID_ANY, id_hbox, "Edited", _edit_check );
    _edit_check->Bind( wxEVT_CHECKBOX, &ItemView::on_edit_update, this);

    main_vbox->Add( id_hbox, 0, wxEXPAND, 0 );
    main_vbox->AddSpacer( 20 );

    _item_panel = new wxPanel( this, wxID_ANY );
    _item_panel->Enable(false);
    wxBoxSizer *item_vbox = new wxBoxSizer( wxVERTICAL );

    wxBoxSizer *name_hbox = new wxBoxSizer( wxHORIZONTAL );
    mk_field( _item_panel, wxID_ANY, name_hbox, "abbr:", _abbr_text, 0, 80);
    _abbr_text->Bind( wxEVT_TEXT_ENTER, &ItemView::on_abbrtext_update, this );
    mk_field( _item_panel, wxID_ANY, name_hbox, "name:", _name_text, 1, 200);
    _name_text->Bind( wxEVT_TEXT_ENTER, &ItemView::on_nametext_update, this );
    mk_field( _item_panel, wxID_ANY, name_hbox, "names:", _names_text, 1, 200);
    _names_text->Bind( wxEVT_TEXT_ENTER, &ItemView::on_namestext_update, this );
    item_vbox->Add( name_hbox, 0, wxEXPAND, 0 );

    // flags
    wxBoxSizer *flags_hbox = new wxBoxSizer( wxHORIZONTAL );
    mk_title( _item_panel, flags_hbox, "Flags: ");
    mk_check( _item_panel, wxID_ANY, flags_hbox, "Disabled", _disabled_check );
    _disabled_check->Bind( wxEVT_CHECKBOX, &ItemView::on_disabled_update, this);
    mk_check( _item_panel, wxID_ANY, flags_hbox, "Can't give", _cantgive_check );
    _cantgive_check->Bind( wxEVT_CHECKBOX, &ItemView::on_cantgive_update, this);
    item_vbox->Add( flags_hbox, 0, wxEXPAND, 0 );

    // magical skill
    wxBoxSizer *magic_hbox = new wxBoxSizer( wxHORIZONTAL );
    mk_title( _item_panel, magic_hbox, "Magical Production: skill ");
    //mk_field( this, wxID_ANY, magic_hbox, "Skill:", _mSkill_text, 0, 80);
    _mSkill_combo = new wxComboBox(_item_panel, idAbbrCombo, _T(""), wxDefaultPosition, wxDefaultSize,
                                0, NULL, wxTE_PROCESS_ENTER | wxTE_PROCESS_TAB);
    _mSkill_combo->Append( wxString("NULL"));
    for( int i=0; i<NSKILLS; ++i) {
        if( SkillDefs[i].flags & SkillType::MAGIC ) {
            std::string mLabel = std::string( SkillDefs[i].name);
            mLabel += " [" + std::string( SkillDefs[i].abbr) + "]";
            _map_mSkill[mLabel] = i;
            _mSkill_combo->Append( wxString(mLabel));
            //std::cout << mLabel << "=>" << i << std::endl;
            }
    }
    _mSkill_combo->SetValue( wxString("NULL") );
    _mSkill_combo->Bind( wxEVT_COMBOBOX, &ItemView::on_mSkillcombo_update, this);
    magic_hbox->Add( _mSkill_combo, 0, wxEXPAND | wxRIGHT | wxALIGN_CENTER_VERTICAL, 20);
    mk_spin( _item_panel, wxID_ANY, magic_hbox, "Level:", _mLevel_spin, 0, 10, 50);
    _mLevel_spin->Bind( wxEVT_SPINCTRL, &ItemView::on_mLevelspin_update, this);
    _mLevel_spin->Bind( wxEVT_TEXT, &ItemView::on_mLevelspin_updateenter, this);
    _mLevel_spin->Bind( wxEVT_TEXT_ENTER, &ItemView::on_mLevelspin_updateenter, this);
    //mk_title( _monster_panel, magic_hbox, "Materials: TODO");
    item_vbox->Add( magic_hbox, 0, wxEXPAND, 0 );

    // weight, baseprice
    wxBoxSizer *weight_hbox = new wxBoxSizer( wxHORIZONTAL );
    mk_spin( _item_panel, wxID_ANY, weight_hbox, "Weight:", _weight_spin, 0, 1000, 200);
    _weight_spin->Bind( wxEVT_SPINCTRL, &ItemView::on_weightspin_update, this);
    _weight_spin->Bind( wxEVT_TEXT, &ItemView::on_weightspin_updateenter, this);
    _weight_spin->Bind( wxEVT_TEXT_ENTER, &ItemView::on_weightspin_updateenter, this);
    mk_spin( _item_panel, wxID_ANY, weight_hbox, "Baseprice:", _baseprice_spin, 0, 1000, 200);
    _baseprice_spin->Bind( wxEVT_SPINCTRL, &ItemView::on_basepricespin_update, this);
    _baseprice_spin->Bind( wxEVT_TEXT, &ItemView::on_basepricespin_updateenter, this);
    _baseprice_spin->Bind( wxEVT_TEXT_ENTER, &ItemView::on_basepricespin_updateenter, this);
    item_vbox->Add( weight_hbox, 0, wxEXPAND, 0 );

    // type IT_NORMAL, IT_ADVANCED, IT_TRADE, IT_MAGIC, IT_WEAPON, IT_ARMOR, IT_MOUNT
    //       IT_BATTLE, IT_SPECIAL, IT_TOOL, IT_FOOD, IT_SHIP, MAGEONLY, IT_NEVER_SPOIL
    wxBoxSizer *type_hbox = new wxBoxSizer( wxHORIZONTAL );
    mk_title( _item_panel, type_hbox, "Type:");
    mk_check( _item_panel, wxID_ANY, type_hbox, "Normal", _normal_check );
    _normal_check->Bind( wxEVT_CHECKBOX, &ItemView::on_normal_update, this);
    mk_check( _item_panel, wxID_ANY, type_hbox, "Advanced", _advanced_check );
    _advanced_check->Bind( wxEVT_CHECKBOX, &ItemView::on_advanced_update, this);
    mk_check( _item_panel, wxID_ANY, type_hbox, "Trade", _trade_check );
    _trade_check->Bind( wxEVT_CHECKBOX, &ItemView::on_trade_update, this);
    mk_check( _item_panel, wxID_ANY, type_hbox, "Magic", _magic_check );
    _magic_check->Bind( wxEVT_CHECKBOX, &ItemView::on_magic_update, this);
    mk_check( _item_panel, wxID_ANY, type_hbox, "Weapon", _weapon_check );
    _weapon_check->Bind( wxEVT_CHECKBOX, &ItemView::on_weapon_update, this);
    mk_check( _item_panel, wxID_ANY, type_hbox, "Armor", _armor_check );
    _armor_check->Bind( wxEVT_CHECKBOX, &ItemView::on_armor_update, this);
    mk_check( _item_panel, wxID_ANY, type_hbox, "Mount", _mount_check );
    _mount_check->Bind( wxEVT_CHECKBOX, &ItemView::on_mount_update, this);
    mk_check( _item_panel, wxID_ANY, type_hbox, "Battle", _battle_check );
    _battle_check->Bind( wxEVT_CHECKBOX, &ItemView::on_battle_update, this);
    mk_check( _item_panel, wxID_ANY, type_hbox, "Special", _special_check );
    _special_check->Bind( wxEVT_CHECKBOX, &ItemView::on_special_update, this);
    mk_check( _item_panel, wxID_ANY, type_hbox, "Tool", _tool_check );
    _tool_check->Bind( wxEVT_CHECKBOX, &ItemView::on_tool_update, this);
    mk_check( _item_panel, wxID_ANY, type_hbox, "Food", _food_check );
    _food_check->Bind( wxEVT_CHECKBOX, &ItemView::on_food_update, this);
    mk_check( _item_panel, wxID_ANY, type_hbox, "Ship", _ship_check );
    _ship_check->Bind( wxEVT_CHECKBOX, &ItemView::on_ship_update, this);
    mk_check( _item_panel, wxID_ANY, type_hbox, "Mage Only", _mageonly_check );
    _mageonly_check->Bind( wxEVT_CHECKBOX, &ItemView::on_mageonly_update, this);
    mk_check( _item_panel, wxID_ANY, type_hbox, "Never Spoil", _neverspoil_check );
    _neverspoil_check->Bind( wxEVT_CHECKBOX, &ItemView::on_neverspoil_update, this);
    item_vbox->Add( type_hbox, 0, wxEXPAND, 0 );

    // Movements
    wxBoxSizer *move_hbox = new wxBoxSizer( wxHORIZONTAL );
    mk_title( _item_panel, move_hbox, "Movements: ");
    mk_spin( _item_panel, wxID_ANY, move_hbox, "Walk", _walk_spin, 0, 1000, 100);
    _walk_spin->Bind( wxEVT_SPINCTRL, &ItemView::on_walkspin_update, this);
    _walk_spin->Bind( wxEVT_TEXT, &ItemView::on_walkspin_updateenter, this);
    _walk_spin->Bind( wxEVT_TEXT_ENTER, &ItemView::on_walkspin_updateenter, this);
    mk_spin( _item_panel, wxID_ANY, move_hbox, "Ride", _ride_spin, 0, 1000, 100);
    _ride_spin->Bind( wxEVT_SPINCTRL, &ItemView::on_ridespin_update, this);
    _ride_spin->Bind( wxEVT_TEXT, &ItemView::on_ridespin_updateenter, this);
    _ride_spin->Bind( wxEVT_TEXT_ENTER, &ItemView::on_ridespin_updateenter, this);
    mk_spin( _item_panel, wxID_ANY, move_hbox, "Fly", _fly_spin, 0, 1000, 100);
    _fly_spin->Bind( wxEVT_SPINCTRL, &ItemView::on_flyspin_update, this);
    _fly_spin->Bind( wxEVT_TEXT, &ItemView::on_flyspin_updateenter, this);
    _fly_spin->Bind( wxEVT_TEXT_ENTER, &ItemView::on_flyspin_updateenter, this);
    mk_spin( _item_panel, wxID_ANY, move_hbox, "Swim", _swim_spin, 0, 1000, 100);
    _swim_spin->Bind( wxEVT_SPINCTRL, &ItemView::on_swimspin_update, this);
    _swim_spin->Bind( wxEVT_TEXT, &ItemView::on_swimspin_updateenter, this);
    _swim_spin->Bind( wxEVT_TEXT_ENTER, &ItemView::on_swimspin_updateenter, this);
    mk_spin( _item_panel, wxID_ANY, move_hbox, "Speed", _speed_spin, 0, 20, 80);
    _speed_spin->Bind( wxEVT_SPINCTRL, &ItemView::on_speedspin_update, this);
    _speed_spin->Bind( wxEVT_TEXT, &ItemView::on_speedspin_updateenter, this);
    _speed_spin->Bind( wxEVT_TEXT_ENTER, &ItemView::on_speedspin_updateenter, this);
    item_vbox->Add( move_hbox, 0, wxEXPAND, 0 );

    // Max Inventory
    wxBoxSizer *inventory_hbox = new wxBoxSizer( wxHORIZONTAL );
    mk_spin( _item_panel, wxID_ANY, inventory_hbox, "Max Inventory:", _maxinventory_spin, 0, 1000, 150);
    _maxinventory_spin->Bind( wxEVT_SPINCTRL, &ItemView::on_maxinventoryspin_update, this);
    _maxinventory_spin->Bind( wxEVT_TEXT, &ItemView::on_maxinventoryspin_updateenter, this);
    _maxinventory_spin->Bind( wxEVT_TEXT_ENTER, &ItemView::on_maxinventoryspin_updateenter, this);
    item_vbox->Add( inventory_hbox, 0, wxEXPAND, 0 );

    _item_panel->SetSizer( item_vbox );

    main_vbox->Add( _item_panel, 0, wxEXPAND, 0);

    this->SetSizer( main_vbox );

}
// ****************************************************** ItemView::add_item
void ItemView::add_item( AItem* new_item )
{
    _abbr_combo->Append( wxString(new_item->_item_enum) );
    _abbr_combo->SetValue( wxString(new_item->_item_enum) );
    set_item( new_item );
}
// ************************************************ ItemView::on_cbox_update
void ItemView::on_cbox_update( wxCommandEvent& event )
{
    // Don't show messages for the log output window (it'll crash)
    if ( !event.GetEventObject()->IsKindOf(CLASSINFO(wxComboBox)) )
        return;

    if ( event.GetEventType() == wxEVT_COMBOBOX )
    {
        //std::cout << "EVT_COMBOBOX(id="<< event.GetId() << ",selection=" << event.GetSelection() << std::endl;
        //std::cout << "  => " << _list_abbr[event.GetSelection()] << std::endl;
        //std::cout << "  => " << _abbr_combo->GetStringSelection() << std::endl;
        set_item( _data.find_item( _abbr_combo->GetStringSelection().ToStdString()) );
    }
    else if ( event.GetEventType() == wxEVT_TEXT )
    {
        //std::cout << "EVT_TEXT(id="<< event.GetId() << ",selection=" << event.GetSelection() << std::endl;
    }
    else if ( event.GetEventType() == wxEVT_TEXT_ENTER )
    {
        //std::cout << "EVT_TEXT_ENTER(id="<< event.GetId() << ",string=" << event.GetString() << std::endl;
        //wxLogDebug("EVT_TEXT_ENTER(id=%i,string=\"%s\")",
        //            event.GetId(), event.GetString().c_str());
    }
}
void ItemView::on_edit_update( wxCommandEvent& event )
{
    if( _item->_fg_edited == false ) {
        int answer = wxMessageBox("Tu veux vraiment modifier cet item ?? ", "Editer", wxOK | wxCANCEL, this);
        if( answer == wxOK) {
            _item->_fg_edited = true;
            _item_panel->Enable( true );
        }
        else {
            _edit_check->SetValue( false );
        }
    }
    else {
        if( _edit_check->GetValue() == false ) {
            wxMessageBox("'tention, les modifs sont memorisees ", "Editer", wxOK, this);
            _item_panel->Enable( false );
        }
        else {
            _item_panel->Enable( _edit_check->GetValue() );
        }
    }
}

void ItemView::on_abbrtext_update( wxCommandEvent& event )
{
    std::string abbr = _abbr_text->GetValue().ToStdString();
    std::transform(abbr.begin(), abbr.end(), abbr.begin(), ::toupper);
    // check only Caps and no more than 4
    if( abbr.length() > 4) {
        wxMessageBox("Plus de 4 lettres !!", "Erreur", wxOK, this);
        _abbr_text->Clear();
        _abbr_text->AppendText( wxString( _item->_item->abr));
    }
    else {
        _item->_item->abr = (const char*) malloc( abbr.length()+1 );
        strcpy( (char *) _item->_item->abr, abbr.c_str() );
        _abbr_text->Clear();
        _abbr_text->AppendText( wxString( _item->_item->abr));
    }
}
void ItemView::on_nametext_update( wxCommandEvent& event )
{
    std::string name = _name_text->GetValue().ToStdString();
    _item->_item->name = (const char*) malloc( name.length()+1 );
    strcpy( (char *) _item->_item->name, name.c_str() );
    _name_text->Clear();
    _name_text->AppendText( wxString( _item->_item->name));
}
void ItemView::on_namestext_update( wxCommandEvent& event )
{
    std::string names = _names_text->GetValue().ToStdString();
    _item->_item->names = (const char*) malloc( names.length()+1 );
    strcpy( (char *) _item->_item->names, names.c_str() );
    _names_text->Clear();
    _names_text->AppendText( wxString( _item->_item->names));
}
void ItemView::on_disabled_update( wxCommandEvent& event )
{
    if( _disabled_check->GetValue() )
        _item->_item->flags = _item->_item->flags | ItemType::DISABLED;
    else
        _item->_item->flags = _item->_item->flags ^ ItemType::DISABLED;
}
void ItemView::on_cantgive_update( wxCommandEvent& event )
{
    if( _cantgive_check->GetValue() )
        _item->_item->flags = _item->_item->flags | ItemType::CANTGIVE;
    else
        _item->_item->flags = _item->_item->flags ^ ItemType::CANTGIVE;
}
void ItemView::on_mSkillcombo_update( wxCommandEvent& event )
{
    if ( event.GetEventType() == wxEVT_COMBOBOX )
    {
        std::cout << "EVT_COMBOBOX(mSkill), selection=" << _mSkill_combo->GetStringSelection().ToStdString() << std::endl;
        //std::cout << "  " << _mSkill_combo.GetStringSelection().ToStdString() << std::endl;
        std::string mLabel = _mSkill_combo->GetStringSelection().ToStdString();
        int id_skill = _map_mSkill[mLabel];
        //std::cout << "  => " << _list_abbr[event.GetSelection()] << std::endl;
        std::cout << "  => " << SkillDefs[id_skill].abbr << std::endl;

        _item->_item->mSkill = (const char*) malloc( strlen(SkillDefs[id_skill].abbr)+1 );
        strcpy( (char *) _item->_item->mSkill, SkillDefs[id_skill].abbr );
    }
}
void ItemView::on_mLevelspin_update( wxSpinEvent& event)
{
    _item->_item->mLevel = _mLevel_spin->GetValue();
}
void ItemView::on_mLevelspin_updateenter( wxCommandEvent& event)
{
    _item->_item->mLevel = _mLevel_spin->GetValue();
}
void ItemView::on_weightspin_update( wxSpinEvent& event)
{
    _item->_item->weight = _weight_spin->GetValue();
}
void ItemView::on_weightspin_updateenter( wxCommandEvent& event)
{
    _item->_item->weight = _weight_spin->GetValue();
}
void ItemView::on_basepricespin_update( wxSpinEvent& event)
{
    _item->_item->baseprice = _baseprice_spin->GetValue();
}
void ItemView::on_basepricespin_updateenter( wxCommandEvent& event)
{
    _item->_item->baseprice = _baseprice_spin->GetValue();
}
// type IT_NORMAL, IT_ADVANCED, IT_TRADE, IT_MAGIC, IT_WEAPON, IT_ARMOR, IT_MOUNT
//       IT_BATTLE, IT_SPECIAL, IT_TOOL, IT_FOOD, IT_SHIP, MAGEONLY, IT_NEVER_SPOIL
void ItemView::on_normal_update( wxCommandEvent& event )
{
    if( _normal_check->GetValue() )
        _item->_item->type = _item->_item->type | IT_NORMAL;
    else
        _item->_item->type = _item->_item->type ^ IT_NORMAL;
}
void ItemView::on_advanced_update( wxCommandEvent& event )
{
    if( _advanced_check->GetValue() )
        _item->_item->type = _item->_item->type | IT_ADVANCED;
    else
        _item->_item->type = _item->_item->type ^ IT_ADVANCED;
}
void ItemView::on_trade_update( wxCommandEvent& event )
{
    if( _trade_check->GetValue() )
        _item->_item->type = _item->_item->type | IT_TRADE;
    else
        _item->_item->type = _item->_item->type ^ IT_TRADE;
}
void ItemView::on_magic_update( wxCommandEvent& event )
{
    if( _magic_check->GetValue() )
        _item->_item->type = _item->_item->type | IT_MAGIC;
    else
        _item->_item->type = _item->_item->type ^ IT_MAGIC;
}
void ItemView::on_weapon_update( wxCommandEvent& event )
{
    if( _weapon_check->GetValue() )
        _item->_item->type = _item->_item->type | IT_WEAPON;
    else
        _item->_item->type = _item->_item->type ^ IT_WEAPON;
    _item->switch_weapon();
    _armor_check->SetValue(false);
    _mount_check->SetValue(false);
}
void ItemView::on_armor_update( wxCommandEvent& event )
{
    if( _armor_check->GetValue() )
        _item->_item->type = _item->_item->type | IT_ARMOR;
    else
        _item->_item->type = _item->_item->type ^ IT_ARMOR;
    _item->switch_armor();
    _weapon_check->SetValue(false);
    _mount_check->SetValue(false);
}
void ItemView::on_mount_update( wxCommandEvent& event )
{
    if( _mount_check->GetValue() )
        _item->_item->type = _item->_item->type | IT_MOUNT;
    else
        _item->_item->type = _item->_item->type ^ IT_MOUNT;
    _item->switch_mount();
    _weapon_check->SetValue(false);
    _armor_check->SetValue(false);
}
void ItemView::on_battle_update( wxCommandEvent& event )
{
    if( _battle_check->GetValue() ) {
        _item->_item->type = _item->_item->type | IT_BATTLE;
        _item->switch_battle( true );
    }
    else {
        _item->_item->type = _item->_item->type ^ IT_BATTLE;
        _item->switch_battle( false );
    }
}
void ItemView::on_special_update( wxCommandEvent& event )
{
    if( _special_check->GetValue() )
        _item->_item->type = _item->_item->type | IT_SPECIAL;
    else
        _item->_item->type = _item->_item->type ^ IT_SPECIAL;
}
void ItemView::on_tool_update( wxCommandEvent& event )
{
    if( _tool_check->GetValue() )
        _item->_item->type = _item->_item->type | IT_TOOL;
    else
        _item->_item->type = _item->_item->type ^ IT_TOOL;
}
void ItemView::on_food_update( wxCommandEvent& event )
{
    if( _food_check->GetValue() )
        _item->_item->type = _item->_item->type | IT_FOOD;
    else
        _item->_item->type = _item->_item->type ^ IT_FOOD;
}
void ItemView::on_ship_update( wxCommandEvent& event )
{
    if( _ship_check->GetValue() )
        _item->_item->type = _item->_item->type | IT_SHIP;
    else
        _item->_item->type = _item->_item->type ^ IT_SHIP;
}
void ItemView::on_mageonly_update( wxCommandEvent& event )
{
    if( _mageonly_check->GetValue() )
        _item->_item->type = _item->_item->type | IT_MAGEONLY;
    else
        _item->_item->type = _item->_item->type ^ IT_MAGEONLY;
}
void ItemView::on_neverspoil_update( wxCommandEvent& event )
{
    if( _neverspoil_check->GetValue() )
        _item->_item->type = _item->_item->type | IT_NEVER_SPOIL;
    else
        _item->_item->type = _item->_item->type ^ IT_NEVER_SPOIL;
}

void ItemView::on_walkspin_update( wxSpinEvent& event)
{
    _item->_item->walk = _walk_spin->GetValue();
}
void ItemView::on_walkspin_updateenter( wxCommandEvent& event)
{
    _item->_item->walk = _walk_spin->GetValue();
}
void ItemView::on_ridespin_update( wxSpinEvent& event)
{
    _item->_item->ride = _ride_spin->GetValue();
}
void ItemView::on_ridespin_updateenter( wxCommandEvent& event)
{
    _item->_item->ride = _ride_spin->GetValue();
}
void ItemView::on_flyspin_update( wxSpinEvent& event)
{
    _item->_item->fly = _fly_spin->GetValue();
}
void ItemView::on_flyspin_updateenter( wxCommandEvent& event)
{
    _item->_item->fly = _fly_spin->GetValue();
}
void ItemView::on_swimspin_update( wxSpinEvent& event)
{
    _item->_item->swim = _swim_spin->GetValue();
}
void ItemView::on_swimspin_updateenter( wxCommandEvent& event)
{
    _item->_item->swim = _swim_spin->GetValue();
}
void ItemView::on_speedspin_update( wxSpinEvent& event)
{
    _item->_item->speed = _speed_spin->GetValue();
}
void ItemView::on_speedspin_updateenter( wxCommandEvent& event)
{
    _item->_item->speed = _speed_spin->GetValue();
}
void ItemView::on_maxinventoryspin_update( wxSpinEvent& event)
{
    _item->_item->max_inventory = _maxinventory_spin->GetValue();
}
void ItemView::on_maxinventoryspin_updateenter( wxCommandEvent& event)
{
    _item->_item->max_inventory = _maxinventory_spin->GetValue();
}

// ****************************************************************************
void ItemView::set_item( AItem* item )
{
    _item = item;
    _edit_check->SetValue( false );
    _item_panel->Enable( false );
    // abr, name, names
    _abbr_text->Clear();
    _abbr_text->AppendText( wxString( item->_item->abr));
    _name_text->Clear();
    _name_text->AppendText( wxString( item->_item->name));
    _names_text->Clear();
    _names_text->AppendText( wxString( item->_item->names));

    // flags
    _disabled_check->SetValue( item->_item->flags & ItemType::DISABLED );
    _cantgive_check->SetValue( item->_item->flags & ItemType::CANTGIVE );

    // Magic production
    if( item->_item->mSkill != NULL ) {
        // Find proper Skill
        for( int i=0; i < NSKILLS; ++i ) {
            std::cout << "CMP " << SkillDefs[i].abbr << " with " << item->_item->mSkill << std::endl;
            if( strcmp( SkillDefs[i].abbr, item->_item->mSkill) == 0 ) {
                std::cout << "  => FOUND" << std::endl;
                std::string mLabel = std::string( SkillDefs[i].name);
                mLabel += " [" + std::string( SkillDefs[i].abbr) + "]";
                _map_mSkill[mLabel] = i;
                _mSkill_combo->SetValue( wxString(mLabel));
                break;
            }
        }
        _mLevel_spin->SetValue( item->_item->mLevel );
    }
    else {
//        _mSkill_text->Clear();
//        _mSkill_text->AppendText( _("-") );
        _mSkill_combo->SetValue( wxString("NULL") );
        _mLevel_spin->SetValue( 0 );
    }

    // weight
    _weight_spin->SetValue( item->_item->weight );
    _baseprice_spin->SetValue( item->_item->baseprice );

    // type IT_NORMAL, IT_ADVANCED, IT_TRADE, IT_MAGIC, IT_WEAPON, IT_ARMOR, IT_MOUNT
    //       IT_BATTLE, IT_SPECIAL, IT_TOOL, IT_FOOD, IT_SHIP, MAGEONLY, IT_NEVER_SPOIL
    _normal_check->SetValue( item->_item->type & IT_NORMAL );
    _advanced_check->SetValue( item->_item->type & IT_ADVANCED );
    _trade_check->SetValue( item->_item->type & IT_TRADE );
    _magic_check->SetValue( item->_item->type & IT_MAGIC );
    _weapon_check->SetValue( item->_item->type & IT_WEAPON );
    _armor_check->SetValue( item->_item->type & IT_ARMOR );
    _mount_check->SetValue( item->_item->type & IT_MOUNT );
    _battle_check->SetValue( item->_item->type & IT_BATTLE );
    _special_check->SetValue( item->_item->type & IT_SPECIAL );
    _tool_check->SetValue( item->_item->type & IT_TOOL );
    _food_check->SetValue( item->_item->type & IT_FOOD );
    _ship_check->SetValue( item->_item->type & IT_SHIP );
    _mageonly_check->SetValue( item->_item->type & IT_MAGEONLY );
    _neverspoil_check->SetValue( item->_item->type & IT_NEVER_SPOIL );

    // movements
    _walk_spin->SetValue( item->_item->walk );
    _ride_spin->SetValue( item->_item->ride );
    _fly_spin->SetValue( item->_item->fly );
    _swim_spin->SetValue( item->_item->swim );
    _speed_spin->SetValue( item->_item->speed );

    // max inventory
    _maxinventory_spin->SetValue( item->_item->max_inventory );
}
