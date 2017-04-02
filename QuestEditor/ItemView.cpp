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
#include <iostream>     // std::cout, std::fixed
#include <iomanip>

wxBEGIN_EVENT_TABLE(ItemView, wxPanel)
    EVT_COMBOBOX(idAbbrCombo, ItemView::on_cbox_update)
    EVT_TEXT(idAbbrCombo, ItemView::on_cbox_update)
    EVT_TEXT_ENTER(idAbbrCombo, ItemView::on_cbox_update)
wxEND_EVENT_TABLE()

ItemView::ItemView(wxWindow *parent, AllData& data)
    : wxPanel( parent ), _data(data), _item(nullptr)
{
    // Weapon Class
    enum {
	SLASHING,		// e.g. sword attack (This is default)
	PIERCING,		// e.g. spear or arrow attack
	CRUSHING,		// e.g. mace attack
	CLEAVING,		// e.g. axe attack
	ARMORPIERCING,		// e.g. crossbow double bow
	MAGIC_ENERGY,		// e.g. fire, dragon breath
	MAGIC_SPIRIT,		// e.g. black wind
	MAGIC_WEATHER,		// e.g. tornado
	NUM_WEAPON_CLASSES
};

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
    mk_check( _item_panel, wxID_ANY, flags_hbox, "NoMarket", _nomarket_check );
    _nomarket_check->Bind( wxEVT_CHECKBOX, &ItemView::on_nomarket_update, this);
    mk_check( _item_panel, wxID_ANY, flags_hbox, "OrInput", _orinput_check );
    _orinput_check->Bind( wxEVT_CHECKBOX, &ItemView::on_orinput_update, this);
    mk_check( _item_panel, wxID_ANY, flags_hbox, "SkillOut", _skillout_check );
    _skillout_check->Bind( wxEVT_CHECKBOX, &ItemView::on_skillout_update, this);
    mk_check( _item_panel, wxID_ANY, flags_hbox, "NoTransport", _notransport_check );
    _notransport_check->Bind( wxEVT_CHECKBOX, &ItemView::on_notransport_update, this);
//    mk_check( _item_panel, wxID_ANY, flags_hbox, "Can't give", _cantgive_check );
//    _cantgive_check->Bind( wxEVT_CHECKBOX, &ItemView::on_cantgive_update, this);
    item_vbox->Add( flags_hbox, 0, wxEXPAND, 0 );

    // production skill
    wxBoxSizer *prod_hbox = new wxBoxSizer( wxHORIZONTAL );
    mk_title( _item_panel, prod_hbox, "Production: skill ");
    //mk_field( this, wxID_ANY, magic_hbox, "Skill:", _mSkill_text, 0, 80);
    _pSkill_combo = new wxComboBox(_item_panel, idAbbrCombo, _T(""), wxDefaultPosition, wxSize(300,-1),
                                0, NULL, wxTE_PROCESS_ENTER | wxTE_PROCESS_TAB);
    _pSkill_combo->Append( wxString("NULL"));
    for( int i=0; i<NSKILLS; ++i) {
        if( !(SkillDefs[i].flags & SkillType::MAGIC) ) {
            std::string pLabel = std::string( SkillDefs[i].name);
            pLabel += " [" + std::string( SkillDefs[i].abbr) + "]";
            _map_pSkill[pLabel] = i;
            _pSkill_combo->Append( wxString(pLabel));
            //std::cout << mLabel << "=>" << i << std::endl;
            }
    }
    _pSkill_combo->SetValue( wxString("NULL") );
    _pSkill_combo->Bind( wxEVT_COMBOBOX, &ItemView::on_pSkillcombo_update, this);
    prod_hbox->Add( _pSkill_combo, 0, wxEXPAND | wxRIGHT | wxALIGN_CENTER_VERTICAL, 20);
    mk_spin( _item_panel, wxID_ANY, prod_hbox, "Level:", _pLevel_spin, 0, 10, 50);
    _pLevel_spin->Bind( wxEVT_SPINCTRL, &ItemView::on_pLevelspin_update, this);
    _pLevel_spin->Bind( wxEVT_TEXT, &ItemView::on_pLevelspin_updateenter, this);
    _pLevel_spin->Bind( wxEVT_TEXT_ENTER, &ItemView::on_pLevelspin_updateenter, this);
    mk_spin( _item_panel, wxID_ANY, prod_hbox, "Mois:", _pMonth_spin, 0, 10, 50);
    _pMonth_spin->Bind( wxEVT_SPINCTRL, &ItemView::on_pMonthspin_update, this);
    _pMonth_spin->Bind( wxEVT_TEXT, &ItemView::on_pMonthspin_updateenter, this);
    _pMonth_spin->Bind( wxEVT_TEXT_ENTER, &ItemView::on_pMonthspin_updateenter, this);
    mk_spin( _item_panel, wxID_ANY, prod_hbox, "Out:", _pOut_spin, 0, 10, 50);
    _pOut_spin->Bind( wxEVT_SPINCTRL, &ItemView::on_pOutspin_update, this);
    _pOut_spin->Bind( wxEVT_TEXT, &ItemView::on_pOutspin_updateenter, this);
    _pOut_spin->Bind( wxEVT_TEXT_ENTER, &ItemView::on_pOutspin_updateenter, this);
    //mk_title( _item_panel, prod_hbox, "Materials: TODO");
    item_vbox->Add( prod_hbox, 0, wxEXPAND, 0 );

    wxBoxSizer *pmat_hbox = new wxBoxSizer( wxHORIZONTAL );
    mk_title( _item_panel, pmat_hbox, "  Prod. Materials: ");
    for( int k=0; k<4; ++k ) {
        _pMat_combo[k] = new wxComboBox(_item_panel, idProdMat+k, _T(""), wxDefaultPosition, wxSize(200,-1),
                                    0, NULL, wxTE_PROCESS_ENTER | wxTE_PROCESS_TAB);
        _pMat_combo[k]->Append( wxString( "NULL" ));
        for( auto& item : _data._all_enumitems ) {
            _pMat_combo[k]->Append( wxString( item.first ));
        }
        _pMat_combo[k]->SetValue( wxString("NULL") );
        _pMat_combo[k]->Bind( wxEVT_COMBOBOX, &ItemView::on_pMatcombo_update, this);
        pmat_hbox->Add( _pMat_combo[k], 0, wxEXPAND | wxRIGHT | wxALIGN_CENTER_VERTICAL, 20);

        mk_spin( _item_panel, idProdMat+k, pmat_hbox, "Qty:", _pMat_spin[k], 0, 100, 80);
        _pMat_spin[k]->Bind( wxEVT_SPINCTRL, &ItemView::on_pMatspin_update, this);
        _pMat_spin[k]->Bind( wxEVT_TEXT, &ItemView::on_pMatspin_updateenter, this);
        _pMat_spin[k]->Bind( wxEVT_TEXT_ENTER, &ItemView::on_pMatspin_updateenter, this);
    }
    item_vbox->Add( pmat_hbox, 0, wxEXPAND, 0 );

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
    //mk_title( _item_panel, magic_hbox, "Materials: TODO");
    item_vbox->Add( magic_hbox, 0, wxEXPAND, 0 );

    wxBoxSizer *mmat_hbox = new wxBoxSizer( wxHORIZONTAL );
    mk_title( _item_panel, mmat_hbox, "  Mag. Materials: ");
    for( int k=0; k<4; ++k ) {
        _mMat_combo[k] = new wxComboBox(_item_panel, idProdMat+k, _T(""), wxDefaultPosition, wxSize(200,-1),
                                    0, NULL, wxTE_PROCESS_ENTER | wxTE_PROCESS_TAB);
        _mMat_combo[k]->Append( wxString( "NULL" ));
        for( auto& item : _data._all_enumitems ) {
            _mMat_combo[k]->Append( wxString( item.first ));
        }
        _mMat_combo[k]->SetValue( wxString("NULL") );
        _mMat_combo[k]->Bind( wxEVT_COMBOBOX, &ItemView::on_mMatcombo_update, this);
        mmat_hbox->Add( _mMat_combo[k], 0, wxEXPAND | wxRIGHT | wxALIGN_CENTER_VERTICAL, 20);

        mk_spin( _item_panel, idProdMat+k, mmat_hbox, "Qty:", _mMat_spin[k], 0, 100, 80);
        _mMat_spin[k]->Bind( wxEVT_SPINCTRL, &ItemView::on_mMatspin_update, this);
        _mMat_spin[k]->Bind( wxEVT_TEXT, &ItemView::on_mMatspin_updateenter, this);
        _mMat_spin[k]->Bind( wxEVT_TEXT_ENTER, &ItemView::on_mMatspin_updateenter, this);
    }
    item_vbox->Add( mmat_hbox, 0, wxEXPAND, 0 );

    // weight, baseprice, combat
    wxBoxSizer *weight_hbox = new wxBoxSizer( wxHORIZONTAL );
    mk_spin( _item_panel, wxID_ANY, weight_hbox, "Weight:", _weight_spin, 0, 1000, 200);
    _weight_spin->Bind( wxEVT_SPINCTRL, &ItemView::on_weightspin_update, this);
    _weight_spin->Bind( wxEVT_TEXT, &ItemView::on_weightspin_updateenter, this);
    _weight_spin->Bind( wxEVT_TEXT_ENTER, &ItemView::on_weightspin_updateenter, this);
    mk_spin( _item_panel, wxID_ANY, weight_hbox, "Baseprice:", _baseprice_spin, 0, 1000, 200);
    _baseprice_spin->Bind( wxEVT_SPINCTRL, &ItemView::on_basepricespin_update, this);
    _baseprice_spin->Bind( wxEVT_TEXT, &ItemView::on_basepricespin_updateenter, this);
    _baseprice_spin->Bind( wxEVT_TEXT_ENTER, &ItemView::on_basepricespin_updateenter, this);
    mk_spin( _item_panel, wxID_ANY, weight_hbox, "Combat:", _combat_spin, 0, 10, 80);
    _combat_spin->Bind( wxEVT_SPINCTRL, &ItemView::on_combatspin_update, this);
    _combat_spin->Bind( wxEVT_TEXT, &ItemView::on_combatspin_updateenter, this);
    _combat_spin->Bind( wxEVT_TEXT_ENTER, &ItemView::on_combatspin_updateenter, this);
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
    // weapon -> later
    // armor  -> later
    // mount -> later
    // battle -> later
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
    mk_spin( _item_panel, wxID_ANY, inventory_hbox, "Hitch Item:", _hitchitem_spin, 0, 1000, 150);
    _hitchitem_spin->Bind( wxEVT_SPINCTRL, &ItemView::on_hitchitemspin_update, this);
    _hitchitem_spin->Bind( wxEVT_TEXT, &ItemView::on_hitchitemspin_updateenter, this);
    _hitchitem_spin->Bind( wxEVT_TEXT_ENTER, &ItemView::on_hitchitemspin_updateenter, this);
    mk_spin( _item_panel, wxID_ANY, inventory_hbox, "Hitch Walk:", _hitchwalk_spin, 0, 1000, 150);
    _hitchwalk_spin->Bind( wxEVT_SPINCTRL, &ItemView::on_hitchwalkspin_update, this);
    _hitchwalk_spin->Bind( wxEVT_TEXT, &ItemView::on_hitchwalkspin_updateenter, this);
    _hitchwalk_spin->Bind( wxEVT_TEXT_ENTER, &ItemView::on_hitchwalkspin_updateenter, this);
    mk_spin( _item_panel, wxID_ANY, inventory_hbox, "Mult. Item:", _mult_item_spin, 0, 1000, 150);
    _mult_item_spin->Bind( wxEVT_SPINCTRL, &ItemView::on_mult_itemspin_update, this);
    _mult_item_spin->Bind( wxEVT_TEXT, &ItemView::on_mult_itemspin_updateenter, this);
    _mult_item_spin->Bind( wxEVT_TEXT_ENTER, &ItemView::on_mult_itemspin_updateenter, this);
    mk_spin( _item_panel, wxID_ANY, inventory_hbox, "Mult. Val:", _mult_val_spin, 0, 1000, 150);
    _mult_val_spin->Bind( wxEVT_SPINCTRL, &ItemView::on_mult_valspin_update, this);
    _mult_val_spin->Bind( wxEVT_TEXT, &ItemView::on_mult_valspin_updateenter, this);
    _mult_val_spin->Bind( wxEVT_TEXT_ENTER, &ItemView::on_mult_valspin_updateenter, this);
    mk_spin( _item_panel, wxID_ANY, inventory_hbox, "Max Inventory:", _maxinventory_spin, 0, 1000, 150);
    _maxinventory_spin->Bind( wxEVT_SPINCTRL, &ItemView::on_maxinventoryspin_update, this);
    _maxinventory_spin->Bind( wxEVT_TEXT, &ItemView::on_maxinventoryspin_updateenter, this);
    _maxinventory_spin->Bind( wxEVT_TEXT_ENTER, &ItemView::on_maxinventoryspin_updateenter, this);
    item_vbox->Add( inventory_hbox, 0, wxEXPAND, 0 );

    wxBoxSizer *grant_hbox = new wxBoxSizer( wxHORIZONTAL );
    mk_title( _item_panel, grant_hbox, "GrantSkill ");
    //mk_field( this, wxID_ANY, magic_hbox, "Skill:", _mSkill_text, 0, 80);
    _gSkill_combo = new wxComboBox(_item_panel, wxID_ANY, _T(""), wxDefaultPosition, wxSize(300,-1),
                                0, NULL, wxTE_PROCESS_ENTER | wxTE_PROCESS_TAB);
    _gSkill_combo->Append( wxString("NULL"));
    for( int i=0; i<NSKILLS; ++i) {
        std::string pLabel = std::string( SkillDefs[i].name);
        pLabel += " [" + std::string( SkillDefs[i].abbr) + "]";
        //_map_pSkill[pLabel] = i;
        _gSkill_combo->Append( wxString(pLabel));
        //std::cout << mLabel << "=>" << i << std::endl;
    }
    _gSkill_combo->SetValue( wxString("NULL") );
    _gSkill_combo->Bind( wxEVT_COMBOBOX, &ItemView::on_fSkillcombo_update, this);
    grant_hbox->Add( _gSkill_combo, 0, wxEXPAND | wxRIGHT | wxALIGN_CENTER_VERTICAL, 20);
    // spin
    mk_spin( _item_panel, wxID_ANY, grant_hbox, "min Lvl:", _min_grant_spin, 0, 1000, 150);
    _min_grant_spin->Bind( wxEVT_SPINCTRL, &ItemView::on_min_grantspin_update, this);
    _min_grant_spin->Bind( wxEVT_TEXT, &ItemView::on_min_grantspin_updateenter, this);
    _min_grant_spin->Bind( wxEVT_TEXT_ENTER, &ItemView::on_min_grantspin_updateenter, this);
    mk_spin( _item_panel, wxID_ANY, grant_hbox, "max Lvl:", _max_grant_spin, 0, 1000, 150);
    _max_grant_spin->Bind( wxEVT_SPINCTRL, &ItemView::on_max_grantspin_update, this);
    _max_grant_spin->Bind( wxEVT_TEXT, &ItemView::on_max_grantspin_updateenter, this);
    _max_grant_spin->Bind( wxEVT_TEXT_ENTER, &ItemView::on_max_grantspin_updateenter, this);
    item_vbox->Add( grant_hbox, 0, wxEXPAND, 0 );
    // from
    wxBoxSizer *from_hbox = new wxBoxSizer( wxHORIZONTAL );
    for( int k=0; k<4; ++k ) {
        mk_title( _item_panel, from_hbox, "from ");
        //mk_field( this, wxID_ANY, magic_hbox, "Skill:", _mSkill_text, 0, 80);
        _fSkill_combo[k] = new wxComboBox(_item_panel, idGrantSkill+k, _T(""), wxDefaultPosition, wxSize(300,-1),
                                0, NULL, wxTE_PROCESS_ENTER | wxTE_PROCESS_TAB);
        _fSkill_combo[k]->Append( wxString("NULL"));
        for( int i=0; i<NSKILLS; ++i) {
            std::string pLabel = std::string( SkillDefs[i].name);
            pLabel += " [" + std::string( SkillDefs[i].abbr) + "]";
            //_map_pSkill[pLabel] = i;
            _fSkill_combo[k]->Append( wxString(pLabel));
            //std::cout << mLabel << "=>" << i << std::endl;
        }
        _fSkill_combo[k]->SetValue( wxString("NULL") );
        _fSkill_combo[k]->Bind( wxEVT_COMBOBOX, &ItemView::on_fSkillcombo_update, this);
        from_hbox->Add( _fSkill_combo[k], 0, wxEXPAND | wxRIGHT | wxALIGN_CENTER_VERTICAL, 20);
    }
    item_vbox->Add( from_hbox, 0, wxEXPAND, 0 );

    // WEAPON
    wxBoxSizer *weapon_type_hbox = new wxBoxSizer( wxHORIZONTAL );
    mk_check( _item_panel, wxID_ANY, weapon_type_hbox, "Weapon", _weapon_check );
    _weapon_check->Bind( wxEVT_CHECKBOX, &ItemView::on_weapon_update, this);
    item_vbox->Add( weapon_type_hbox, 0, wxEXPAND, 0);

    _weapon_panel = new wxPanel( _item_panel, wxID_ANY );
    wxBoxSizer *weapon_vbox = new wxBoxSizer( wxVERTICAL );

    wxBoxSizer *weapon_flag_hbox = new wxBoxSizer( wxHORIZONTAL );
    weapon_flag_hbox->AddSpacer(30);
    mk_check( _weapon_panel, idWeapNeedSkill, weapon_flag_hbox, "NeedSkill", _needskill_ch );
    _needskill_ch->Bind( wxEVT_CHECKBOX, &ItemView::on_weaponcheck_update, this);
    mk_check( _weapon_panel, idWeapAlwaysReady, weapon_flag_hbox, "AlwaysReady", _alwaysready_ch );
    _alwaysready_ch->Bind( wxEVT_CHECKBOX, &ItemView::on_weaponcheck_update, this);
    mk_check( _weapon_panel, idWeapNoDefense, weapon_flag_hbox, "NoDefense", _nodefense_ch );
    _nodefense_ch->Bind( wxEVT_CHECKBOX, &ItemView::on_weaponcheck_update, this);
    mk_check( _weapon_panel, idWeapNoFoot, weapon_flag_hbox, "NoFoot", _nofoot_ch );
    _nofoot_ch->Bind( wxEVT_CHECKBOX, &ItemView::on_weaponcheck_update, this);
    mk_check( _weapon_panel, idWeapNoMount, weapon_flag_hbox, "NoMount", _nomount_ch );
    _nomount_ch->Bind( wxEVT_CHECKBOX, &ItemView::on_weaponcheck_update, this);
    mk_check( _weapon_panel, idWeapShort, weapon_flag_hbox, "Short", _short_ch );
    _short_ch->Bind( wxEVT_CHECKBOX, &ItemView::on_weaponcheck_update, this);
    mk_check( _weapon_panel, idWeapLong, weapon_flag_hbox, "Long", _long_ch);
    _long_ch->Bind( wxEVT_CHECKBOX, &ItemView::on_weaponcheck_update, this);
    mk_check( _weapon_panel, idWeapRanged, weapon_flag_hbox, "Ranged", _ranged_ch );
    _ranged_ch->Bind( wxEVT_CHECKBOX, &ItemView::on_weaponcheck_update, this);
    mk_check( _weapon_panel, idWeapNoAttackerSkill, weapon_flag_hbox, "NoAttackerSkill", _noattackerskill_ch );
    _noattackerskill_ch->Bind( wxEVT_CHECKBOX, &ItemView::on_weaponcheck_update, this);
    mk_check( _weapon_panel, idWeapRidingBonus, weapon_flag_hbox, "RidingBonus", _riding_bonus_ch );
    _riding_bonus_ch->Bind( wxEVT_CHECKBOX, &ItemView::on_weaponcheck_update, this);
    mk_check( _weapon_panel, idWeapRidingBonusDefense, weapon_flag_hbox, "RidingBonusDefense", _riding_bonus_defense_ch );
    _riding_bonus_defense_ch->Bind( wxEVT_CHECKBOX, &ItemView::on_weaponcheck_update, this);
    weapon_vbox->Add( weapon_flag_hbox, 0, wxEXPAND, 0);

//    Skills with SkillType::BATTLEREP
    wxBoxSizer *weapskill_hbox = new wxBoxSizer( wxHORIZONTAL );
    weapskill_hbox->AddSpacer(30);
    mk_title( _weapon_panel, weapskill_hbox, "Skill ");
    _bSkill_combo = new wxComboBox(_weapon_panel, idAbbrCombo, _T(""), wxDefaultPosition, wxDefaultSize,
                                    0, NULL, wxTE_PROCESS_ENTER | wxTE_PROCESS_TAB);
    _bSkill_combo->Append( wxString("NULL"));
    _bSkill_combo->SetValue( wxString("NULL") );
    _bSkill_combo->Bind( wxEVT_COMBOBOX, &ItemView::on_bSkillcombo_update, this);
    weapskill_hbox->Add( _bSkill_combo, 0, wxEXPAND | wxRIGHT | wxALIGN_CENTER_VERTICAL, 20);
    mk_title( _weapon_panel, weapskill_hbox, " or ");

    _oSkill_combo = new wxComboBox(_weapon_panel, idAbbrCombo, _T(""), wxDefaultPosition, wxDefaultSize,
                                    0, NULL, wxTE_PROCESS_ENTER | wxTE_PROCESS_TAB);
    _oSkill_combo->Append( wxString("NULL"));
    _oSkill_combo->SetValue( wxString("NULL") );
    _oSkill_combo->Bind( wxEVT_COMBOBOX, &ItemView::on_oSkillcombo_update, this);
    weapskill_hbox->Add( _oSkill_combo, 0, wxEXPAND | wxRIGHT | wxALIGN_CENTER_VERTICAL, 20);

    for( int i=0; i<NSKILLS; ++i) {
        if( SkillDefs[i].flags & SkillType::BATTLEREP ) {
            std::string bLabel = std::string( SkillDefs[i].name);
            bLabel += " [" + std::string( SkillDefs[i].abbr) + "]";
            _map_bSkill[bLabel] = i;
            _bSkill_combo->Append( wxString(bLabel));
            _oSkill_combo->Append( wxString(bLabel));
            //std::cout << mLabel << "=>" << i << std::endl;
            }
    }
    _bSkill_combo->SetValue( wxString("NULL") );
    _oSkill_combo->SetValue( wxString("NULL") );

    weapon_vbox->Add( weapskill_hbox, 0, wxEXPAND, 0 );

        // weight, baseprice
    wxBoxSizer *weapon_attack_hbox = new wxBoxSizer( wxHORIZONTAL );
    weapon_attack_hbox->AddSpacer(30);
    mk_title( _weapon_panel, weapon_attack_hbox, "WeaponClass ");
    _weap_class_combo = new wxComboBox(_weapon_panel, idAbbrCombo, _T(""), wxDefaultPosition, wxDefaultSize,
                                    0, NULL, wxTE_PROCESS_ENTER | wxTE_PROCESS_TAB);
    _weap_class_combo->Append( wxString("NULL"));
    std::string attack_enum[] = {"SLASHING","PIERCING","CRUSHING","CLEAVING","ARMORPIERCING","MAGIC_ENERGY","MAGIC_SPIRIT","MAGIC_WEATHER","NUM_WEAPON_CLASSES"};
    for( int i=0; i<NUM_WEAPON_CLASSES; ++i) {
        _map_wClass[attack_enum[i]] = i;
        _weap_class_combo->Append( wxString(attack_enum[i]) );
        //std::cout << mLabel << "=>" << i << std::endl;
    }
    _weap_class_combo->SetValue( wxString("NULL") );
    _weap_class_combo->Bind( wxEVT_COMBOBOX, &ItemView::on_weapClasscombo_update, this);
    weapon_attack_hbox->Add( _weap_class_combo, 0, wxEXPAND | wxRIGHT | wxALIGN_CENTER_VERTICAL, 20);

    // BEWARE WeaponType::NUM_ATTACKS_SKILL ===> see ???? (list ?)
    // Add Panel with explanation
    mk_spin( _weapon_panel, wxID_ANY, weapon_attack_hbox, "NumAttack:", _num_attack_spin, -150, 150, 200);
    _num_attack_spin->Bind( wxEVT_SPINCTRL, &ItemView::on_numattackspin_update, this);
    _num_attack_spin->Bind( wxEVT_TEXT, &ItemView::on_numattackspin_updateenter, this);
    _num_attack_spin->Bind( wxEVT_TEXT_ENTER, &ItemView::on_numattackspin_updateenter, this);
    _num_attack_text = new wxStaticText( _weapon_panel, wxID_ANY, wxString(compute_numattack()));
    weapon_attack_hbox->Add( _num_attack_text, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 2 );
    weapon_vbox->Add( weapon_attack_hbox, 0, wxEXPAND, 0 );

    wxBoxSizer *weapon_bonus_hbox = new wxBoxSizer( wxHORIZONTAL );
    weapon_bonus_hbox->AddSpacer(30);
    mk_spin( _weapon_panel, wxID_ANY, weapon_bonus_hbox, "AttackBonus:", _atk_bonus_spin, 0, 20, 200);
    _atk_bonus_spin->Bind( wxEVT_SPINCTRL, &ItemView::on_atk_bonusspin_update, this);
    _atk_bonus_spin->Bind( wxEVT_TEXT, &ItemView::on_atk_bonusspin_updateenter, this);
    _atk_bonus_spin->Bind( wxEVT_TEXT_ENTER, &ItemView::on_atk_bonusspin_updateenter, this);
    mk_spin( _weapon_panel, wxID_ANY, weapon_bonus_hbox, "DefenseBonus:", _def_bonus_spin, 0, 20, 200);
    _def_bonus_spin->Bind( wxEVT_SPINCTRL, &ItemView::on_def_bonusspin_update, this);
    _def_bonus_spin->Bind( wxEVT_TEXT, &ItemView::on_def_bonusspin_updateenter, this);
    _def_bonus_spin->Bind( wxEVT_TEXT_ENTER, &ItemView::on_def_bonusspin_updateenter, this);
    mk_spin( _weapon_panel, wxID_ANY, weapon_bonus_hbox, "MountBonus:", _mount_bonus_spin, 0, 20, 200);
    _mount_bonus_spin->Bind( wxEVT_SPINCTRL, &ItemView::on_mount_bonusspin_update, this);
    _mount_bonus_spin->Bind( wxEVT_TEXT, &ItemView::on_mount_bonusspin_updateenter, this);
    _mount_bonus_spin->Bind( wxEVT_TEXT_ENTER, &ItemView::on_mount_bonusspin_updateenter, this);
    weapon_vbox->Add( weapon_bonus_hbox, 0, wxEXPAND, 0 );

    _weapon_panel->Enable( false );
    _weapon_panel->SetSizer( weapon_vbox );
    item_vbox->Add( _weapon_panel, 0, wxEXPAND, 0 );

    // ARMOR
    wxBoxSizer *armor_type_hbox = new wxBoxSizer( wxHORIZONTAL );
    mk_check( _item_panel, wxID_ANY, armor_type_hbox, "Armor", _armor_check );
    _armor_check->Bind( wxEVT_CHECKBOX, &ItemView::on_armor_update, this);
    item_vbox->Add( armor_type_hbox, 0, wxEXPAND, 0);

    _armor_panel = new wxPanel( _item_panel, wxID_ANY );
    wxBoxSizer *armor_vbox = new wxBoxSizer( wxVERTICAL );

    wxBoxSizer *armor_flag_hbox = new wxBoxSizer( wxHORIZONTAL );
    armor_flag_hbox->AddSpacer(30);
    mk_check( _armor_panel, idWeapNeedSkill, armor_flag_hbox, "Use in Assassinate ", _use_in_assassinate_check );
    _use_in_assassinate_check->Bind( wxEVT_CHECKBOX, &ItemView::on_use_in_assassinatecheck_update, this);
    armor_vbox->Add( armor_flag_hbox, 0, wxEXPAND, 0);

    wxBoxSizer *armor_save_hbox = new wxBoxSizer( wxHORIZONTAL );
    armor_save_hbox->AddSpacer(30);
    mk_spin( _armor_panel, wxID_ANY, armor_save_hbox, "From:", _from_spin, 0, 1000, 100);
    _from_spin->Bind( wxEVT_SPINCTRL, &ItemView::on_fromspin_update, this);
    _from_spin->Bind( wxEVT_TEXT, &ItemView::on_fromspin_updateenter, this);
    _from_spin->Bind( wxEVT_TEXT_ENTER, &ItemView::on_fromspin_updateenter, this);

    // array of spin
    for( int i=0; i<NUM_WEAPON_CLASSES; ++i ) {
        mk_spin( _armor_panel, idArmorBonusBase+i, armor_save_hbox, attack_enum[i], _chances_spin[i], 0, 1000, 100);
        _chances_spin[i]->Bind( wxEVT_SPINCTRL, &ItemView::on_chancesspin_update, this);
        _chances_spin[i]->Bind( wxEVT_TEXT, &ItemView::on_chancesspin_updateenter, this);
        _chances_spin[i]->Bind( wxEVT_TEXT_ENTER, &ItemView::on_chancesspin_updateenter, this);
    }
    armor_vbox->Add( armor_save_hbox, 0, wxEXPAND, 0 );
//    TODO array of callback
//        wxSpinCtrl *_chances_spin[];
//        void on_chancesspin_update( wxSpinEvent& event);
//        void on_chancesspin_updateenter( wxCommandEvent& event);


    wxBoxSizer *armor_effect_hbox = new wxBoxSizer( wxHORIZONTAL );
    armor_effect_hbox->AddSpacer(30);
    _saves_armor_text = new wxStaticText( _armor_panel, wxID_ANY, wxString(compute_armorsaves()));
    armor_effect_hbox->Add( _saves_armor_text, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 2 );
    armor_vbox->Add( armor_effect_hbox, 0, wxEXPAND, 0 );

    _armor_panel->Enable( false );
    _armor_panel->SetSizer( armor_vbox );
    item_vbox->Add( _armor_panel, 0, wxEXPAND, 0);

    // MOUNT
    wxBoxSizer *mount_type_hbox = new wxBoxSizer( wxHORIZONTAL );
    mk_check( _item_panel, wxID_ANY, mount_type_hbox, "Mount", _mount_check );
    _mount_check->Bind( wxEVT_CHECKBOX, &ItemView::on_mount_update, this);
    item_vbox->Add( mount_type_hbox, 0, wxEXPAND, 0);

    _mount_panel = new wxPanel( _item_panel, wxID_ANY );
    wxBoxSizer *mount_vbox = new wxBoxSizer( wxVERTICAL );

    wxBoxSizer *mount_skill_hbox = new wxBoxSizer( wxHORIZONTAL );
    mount_skill_hbox->AddSpacer(30);
    mk_title( _mount_panel, mount_skill_hbox, "Skill:");

    _mount_skill_combo = new wxComboBox(_mount_panel, wxID_ANY, _T(""), wxDefaultPosition, wxDefaultSize,
                                    0, NULL, wxTE_PROCESS_ENTER | wxTE_PROCESS_TAB);
    _mount_skill_combo->Append( wxString("NULL"));
    _mount_skill_combo->SetValue( wxString("NULL") );
    _mount_skill_combo->Bind( wxEVT_COMBOBOX, &ItemView::on_mount_skillcombo_update, this);
    mount_skill_hbox->Add( _mount_skill_combo, 0, wxEXPAND | wxRIGHT | wxALIGN_CENTER_VERTICAL, 20);
    for( int i=0; i<NSKILLS; ++i) {
        if( SkillDefs[i].name != NULL ) {
            std::string allLabel = std::string( SkillDefs[i].name);
            allLabel += " [" + std::string( SkillDefs[i].abbr) + "]";
            _map_allSkill[allLabel] = i;
            _mount_skill_combo->Append( wxString(allLabel));
            //std::cout << mLabel << "=>" << i << std::endl;
            }
    }
    mount_vbox->Add( mount_skill_hbox, 0, wxEXPAND, 0);

    wxBoxSizer *mount_bonus_hbox = new wxBoxSizer( wxHORIZONTAL );
    mount_bonus_hbox->AddSpacer(30);
    mk_title( _mount_panel, mount_bonus_hbox, "Bonuses: ");
    mk_spin( _mount_panel, wxID_ANY, mount_bonus_hbox, "min:", _mount_min_spin, 0, 10, 80);
    _mount_min_spin->Bind( wxEVT_SPINCTRL, &ItemView::on_mount_minspin_update, this);
    _mount_min_spin->Bind( wxEVT_TEXT, &ItemView::on_mount_minspin_updateenter, this);
    _mount_min_spin->Bind( wxEVT_TEXT_ENTER, &ItemView::on_mount_minspin_updateenter, this);
    mk_spin( _mount_panel, wxID_ANY, mount_bonus_hbox, " max:", _mount_max_spin, 0, 10, 80);
    _mount_max_spin->Bind( wxEVT_SPINCTRL, &ItemView::on_mount_maxspin_update, this);
    _mount_max_spin->Bind( wxEVT_TEXT, &ItemView::on_mount_maxspin_updateenter, this);
    _mount_max_spin->Bind( wxEVT_TEXT_ENTER, &ItemView::on_mount_maxspin_updateenter, this);
    mk_spin( _mount_panel, wxID_ANY, mount_bonus_hbox, "hampered:", _mount_hampered_spin, 0, 10, 80);
    _mount_hampered_spin->Bind( wxEVT_SPINCTRL, &ItemView::on_mount_hamperedspin_update, this);
    _mount_hampered_spin->Bind( wxEVT_TEXT, &ItemView::on_mount_hamperedspin_updateenter, this);
    _mount_hampered_spin->Bind( wxEVT_TEXT_ENTER, &ItemView::on_mount_hamperedspin_updateenter, this);
    mount_vbox->Add( mount_bonus_hbox, 0, wxEXPAND, 0);

    wxBoxSizer *mount_special_hbox = new wxBoxSizer( wxHORIZONTAL );
    mount_special_hbox->AddSpacer(30);
    mk_title( _mount_panel, mount_special_hbox, "Special: ");
    _mount_special_combo = new wxComboBox(_mount_panel, wxID_ANY, _T(""), wxDefaultPosition, wxDefaultSize,
                                    0, NULL, wxTE_PROCESS_ENTER | wxTE_PROCESS_TAB);
    _mount_special_combo->Append( wxString("NULL"));
    _mount_special_combo->SetValue( wxString("NULL") );
    _mount_special_combo->Bind( wxEVT_COMBOBOX, &ItemView::on_mount_specialcombo_update, this);
    mount_special_hbox->Add( _mount_special_combo, 0, wxEXPAND | wxRIGHT | wxALIGN_CENTER_VERTICAL, 20);
    for( int i=0; i<NUMSPECIALS; ++i) {
        if( SpecialDefs[i].key != NULL ) {
            std::string sLabel = std::string( SpecialDefs[i].key);
            //Will be done in BATTLE_map_specSkill[sLabel] = i;
            _mount_special_combo->Append( wxString(sLabel));
            //std::cout << mLabel << "=>" << i << std::endl;
            }
    }
    mk_spin( _mount_panel, wxID_ANY, mount_special_hbox, " lvl:", _mount_special_lvl_spin, 0, 10, 80);
    _mount_special_lvl_spin->Bind( wxEVT_SPINCTRL, &ItemView::on_mount_special_lvlspin_update, this);
    _mount_special_lvl_spin->Bind( wxEVT_TEXT, &ItemView::on_mount_special_lvlspin_updateenter, this);
    _mount_special_lvl_spin->Bind( wxEVT_TEXT_ENTER, &ItemView::on_mount_special_lvlspin_updateenter, this);
    mount_vbox->Add( mount_special_hbox, 0, wxEXPAND, 0);

    _mount_panel->Enable( false );
    _mount_panel->SetSizer( mount_vbox );
    item_vbox->Add( _mount_panel, 0, wxEXPAND, 0);

    // BATTLE ITEM
    wxBoxSizer *battle_type_hbox = new wxBoxSizer( wxHORIZONTAL );
    mk_check( _item_panel, wxID_ANY, battle_type_hbox, "Battle", _battle_check );
    _battle_check->Bind( wxEVT_CHECKBOX, &ItemView::on_battle_update, this);
    item_vbox->Add( battle_type_hbox, 0, wxEXPAND, 0);

    _battle_panel = new wxPanel( _item_panel, wxID_ANY );
    wxBoxSizer *battle_vbox = new wxBoxSizer( wxVERTICAL );

    wxBoxSizer *battle_flag_hbox = new wxBoxSizer( wxHORIZONTAL );
    battle_flag_hbox->AddSpacer(30);
    mk_title( _battle_panel, battle_flag_hbox, "Flags: ");
    mk_check( _battle_panel, wxID_ANY, battle_flag_hbox, "MageOnly", _battle_mageonly_check );
    _battle_mageonly_check->Bind( wxEVT_CHECKBOX, &ItemView::on_bat_mageonly_check_update, this);
    mk_check( _battle_panel, wxID_ANY, battle_flag_hbox, "Special", _battle_special_check );
    _battle_special_check->Bind( wxEVT_CHECKBOX, &ItemView::on_bat_special_check_update, this);
    mk_check( _battle_panel, wxID_ANY, battle_flag_hbox, "Shield", _battle_shield_check );
    _battle_shield_check->Bind( wxEVT_CHECKBOX, &ItemView::on_bat_shield_check_update, this);
    mk_check( _battle_panel, wxID_ANY, battle_flag_hbox, "Exclusive", _battle_exclusive_check );
    _battle_exclusive_check->Bind( wxEVT_CHECKBOX, &ItemView::on_bat_exclusive_check_update, this);
    battle_vbox->Add( battle_flag_hbox, 0, wxEXPAND, 0);

    wxBoxSizer *battle_effect_hbox = new wxBoxSizer( wxHORIZONTAL );
    battle_effect_hbox->AddSpacer(30);
    mk_title( _battle_panel, battle_effect_hbox, "Effect: ");
    _battle_special_combo = new wxComboBox(_battle_panel, wxID_ANY, _T(""), wxDefaultPosition, wxDefaultSize,
                                    0, NULL, wxTE_PROCESS_ENTER | wxTE_PROCESS_TAB);
    _battle_special_combo->Append( wxString("NULL"));
    _battle_special_combo->SetValue( wxString("NULL") );
    _battle_special_combo->Bind( wxEVT_COMBOBOX, &ItemView::on_bat_specialcombo_update, this);
    battle_effect_hbox->Add( _battle_special_combo, 0, wxEXPAND | wxRIGHT | wxALIGN_CENTER_VERTICAL, 20);
    for( int i=0; i<NUMSPECIALS; ++i) {
        if( SpecialDefs[i].key != NULL ) {
            std::string sLabel = std::string( SpecialDefs[i].key);
            _map_specSkill[sLabel] = i;
            _battle_special_combo->Append( wxString(sLabel));
            //std::cout << mLabel << "=>" << i << std::endl;
            }
    }

    mk_spin( _battle_panel, wxID_ANY, battle_effect_hbox, "Lvl:", _battle_lvl_spin, 0, 10, 80);
    _battle_lvl_spin->Bind( wxEVT_SPINCTRL, &ItemView::on_bat_specialspin_update, this);
    _battle_lvl_spin->Bind( wxEVT_TEXT, &ItemView::on_bat_specialspin_updateenter, this);
    _battle_lvl_spin->Bind( wxEVT_TEXT_ENTER, &ItemView::on_bat_specialspin_updateenter, this);
    battle_vbox->Add( battle_effect_hbox, 0, wxEXPAND, 0);

    _battle_panel->Enable( false );
    _battle_panel->SetSizer( battle_vbox );
    item_vbox->Add( _battle_panel, 0, wxEXPAND, 0);

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
void ItemView::on_nomarket_update( wxCommandEvent& event )
{
    if( _nomarket_check->GetValue() )
        _item->_item->flags = _item->_item->flags | ItemType::NOMARKET;
    else
        _item->_item->flags = _item->_item->flags ^ ItemType::NOMARKET;
}
void ItemView::on_orinput_update( wxCommandEvent& event )
{
    if( _orinput_check->GetValue() )
        _item->_item->flags = _item->_item->flags | ItemType::ORINPUTS;
    else
        _item->_item->flags = _item->_item->flags ^ ItemType::ORINPUTS;
}
void ItemView::on_skillout_update( wxCommandEvent& event )
{
    if( _skillout_check->GetValue() )
        _item->_item->flags = _item->_item->flags | ItemType::SKILLOUT;
    else
        _item->_item->flags = _item->_item->flags ^ ItemType::SKILLOUT;
}
void ItemView::on_notransport_update( wxCommandEvent& event )
{
    if( _notransport_check->GetValue() )
        _item->_item->flags = _item->_item->flags | ItemType::NOTRANSPORT;
    else
        _item->_item->flags = _item->_item->flags ^ ItemType::NOTRANSPORT;
}
void ItemView::on_pSkillcombo_update( wxCommandEvent& event )
{
    if ( event.GetEventType() == wxEVT_COMBOBOX )
    {
        std::cout << "EVT_COMBOBOX(mSkill), selection=" << _pSkill_combo->GetStringSelection().ToStdString() << std::endl;
        //std::cout << "  " << _mSkill_combo.GetStringSelection().ToStdString() << std::endl;
        std::string pLabel = _pSkill_combo->GetStringSelection().ToStdString();
        int id_skill = _map_pSkill[pLabel];
        //std::cout << "  => " << _list_abbr[event.GetSelection()] << std::endl;
        std::cout << "  => " << SkillDefs[id_skill].abbr << std::endl;

        _item->_item->pSkill = (const char*) malloc( strlen(SkillDefs[id_skill].abbr)+1 );
        strcpy( (char *) _item->_item->pSkill, SkillDefs[id_skill].abbr );
    }
}
void ItemView::on_pLevelspin_update( wxSpinEvent& event)
{
    _item->_item->pLevel = _pLevel_spin->GetValue();
}
void ItemView::on_pLevelspin_updateenter( wxCommandEvent& event)
{
    _item->_item->pLevel = _pLevel_spin->GetValue();
}
void ItemView::on_pMonthspin_update( wxSpinEvent& event)
{
    _item->_item->pMonths = _pMonth_spin->GetValue();
}
void ItemView::on_pMonthspin_updateenter( wxCommandEvent& event)
{
    _item->_item->pMonths = _pMonth_spin->GetValue();
}
void ItemView::on_pOutspin_update( wxSpinEvent& event)
{
    _item->_item->pOut = _pOut_spin->GetValue();
}
void ItemView::on_pOutspin_updateenter( wxCommandEvent& event)
{
    _item->_item->pOut = _pOut_spin->GetValue();
}
void ItemView::on_pMatcombo_update( wxCommandEvent& event )
{
    if ( event.GetEventType() == wxEVT_COMBOBOX )
    {
        int comboId = event.GetId() - idProdMat;
        //std::cout << "EVT_COMBOBOX(mSkill), selection=" << _gSkill_combo->GetStringSelection().ToStdString() << std::endl;
        //std::cout << "  " << _mSkill_combo.GetStringSelection().ToStdString() << std::endl;
        std::string pLabel = _pMat_combo[comboId]->GetStringSelection().ToStdString();
        if( pLabel.compare("NULL") != 0 ) {
            int id_item = _data._all_enumitems[pLabel];
            //std::cout << "  => " << _list_abbr[event.GetSelection()] << std::endl;
            //std::cout << "  => " << SkillDefs[id_skill].abbr << std::endl;

            _item->_item->pInput[comboId].item = id_item;
        }
        else {
            _item->_item->pInput[comboId].item = -1;
        }
    }
}
void ItemView::on_pMatspin_update( wxSpinEvent& event)
{
    int spinId = event.GetId() - idProdMat;
    _item->_item->pInput[spinId].amt = _pMat_spin[spinId]->GetValue();
}
void ItemView::on_pMatspin_updateenter( wxCommandEvent& event)
{
    int spinId = event.GetId() - idProdMat;
    _item->_item->pInput[spinId].amt = _pMat_spin[spinId]->GetValue();
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
void ItemView::on_mMatcombo_update( wxCommandEvent& event )
{
    if ( event.GetEventType() == wxEVT_COMBOBOX )
    {
        int comboId = event.GetId() - idProdMat;
        //std::cout << "EVT_COMBOBOX(mSkill), selection=" << _gSkill_combo->GetStringSelection().ToStdString() << std::endl;
        //std::cout << "  " << _mSkill_combo.GetStringSelection().ToStdString() << std::endl;
        std::string pLabel = _mMat_combo[comboId]->GetStringSelection().ToStdString();
        if( pLabel.compare("NULL") != 0 ) {
            int id_item = _data._all_enumitems[pLabel];
            //std::cout << "  => " << _list_abbr[event.GetSelection()] << std::endl;
            //std::cout << "  => " << SkillDefs[id_skill].abbr << std::endl;

            _item->_item->mInput[comboId].item = id_item;
        }
        else {
            _item->_item->mInput[comboId].item = -1;
        }
    }
}
void ItemView::on_mMatspin_update( wxSpinEvent& event)
{
    int spinId = event.GetId() - idProdMat;
    _item->_item->mInput[spinId].amt = _mMat_spin[spinId]->GetValue();
}
void ItemView::on_mMatspin_updateenter( wxCommandEvent& event)
{
    int spinId = event.GetId() - idProdMat;
    _item->_item->mInput[spinId].amt = _mMat_spin[spinId]->GetValue();
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
void ItemView::on_combatspin_update( wxSpinEvent& event)
{
    _item->_item->combat = _combat_spin->GetValue();
}
void ItemView::on_combatspin_updateenter( wxCommandEvent& event)
{
    _item->_item->combat = _combat_spin->GetValue();
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
    if( _weapon_check->GetValue() ) {
        _item->_item->type = _item->_item->type | IT_WEAPON;
        _weapon_panel->Enable( true );
        _armor_check->SetValue(false);
        _mount_check->SetValue(false);
    }
    else {
        _item->_item->type = _item->_item->type ^ IT_WEAPON;
        _weapon_panel->Enable( false );
    }
    _item->switch_weapon();
}
void ItemView::on_armor_update( wxCommandEvent& event )
{
    if( _armor_check->GetValue() ) {
        _item->_item->type = _item->_item->type | IT_ARMOR;
        _armor_panel->Enable( true );
        _weapon_check->SetValue(false);
        _mount_check->SetValue(false);
    }
    else {
        _item->_item->type = _item->_item->type ^ IT_ARMOR;
        _armor_panel->Enable( false );
    }
    _item->switch_armor();
}
void ItemView::on_mount_update( wxCommandEvent& event )
{
    if( _mount_check->GetValue() ) {
        _item->_item->type = _item->_item->type | IT_MOUNT;
        _mount_panel->Enable( true );
        _armor_check->SetValue(false);
        _weapon_check->SetValue(false);
    }
    else {
        _item->_item->type = _item->_item->type ^ IT_MOUNT;
        _mount_panel->Enable( false );
    }
    _item->switch_mount();
}
void ItemView::on_battle_update( wxCommandEvent& event )
{
    if( _battle_check->GetValue() ) {
        _item->_item->type = _item->_item->type | IT_BATTLE;
        _battle_panel->Enable( true );
        _item->switch_battle(true);
    }
    else {
        _item->_item->type = _item->_item->type ^ IT_BATTLE;
        _battle_panel->Enable( false );
        _item->switch_battle(false);
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
void ItemView::on_hitchitemspin_update( wxSpinEvent& event)
{
    _item->_item->hitchItem = _hitchitem_spin->GetValue();
}
void ItemView::on_hitchitemspin_updateenter( wxCommandEvent& event)
{
    _item->_item->hitchItem = _hitchitem_spin->GetValue();
}
void ItemView::on_hitchwalkspin_update( wxSpinEvent& event)
{
    _item->_item->hitchwalk = _hitchwalk_spin->GetValue();
}
void ItemView::on_hitchwalkspin_updateenter( wxCommandEvent& event)
{
    _item->_item->hitchwalk = _hitchwalk_spin->GetValue();
}
void ItemView::on_mult_itemspin_update( wxSpinEvent& event)
{
    _item->_item->mult_item = _mult_item_spin->GetValue();
}
void ItemView::on_mult_itemspin_updateenter( wxCommandEvent& event)
{
    _item->_item->mult_item = _mult_item_spin->GetValue();
}
void ItemView::on_mult_valspin_update( wxSpinEvent& event)
{
    _item->_item->mult_val = _mult_val_spin->GetValue();
}
void ItemView::on_mult_valspin_updateenter( wxCommandEvent& event)
{
    _item->_item->mult_val = _mult_val_spin->GetValue();
}
void ItemView::on_maxinventoryspin_update( wxSpinEvent& event)
{
    _item->_item->max_inventory = _maxinventory_spin->GetValue();
}
void ItemView::on_maxinventoryspin_updateenter( wxCommandEvent& event)
{
    _item->_item->max_inventory = _maxinventory_spin->GetValue();
}

void ItemView::on_gSkillcombo_update( wxCommandEvent& event )
{
    if ( event.GetEventType() == wxEVT_COMBOBOX )
    {
        std::cout << "EVT_COMBOBOX(mSkill), selection=" << _gSkill_combo->GetStringSelection().ToStdString() << std::endl;
        //std::cout << "  " << _mSkill_combo.GetStringSelection().ToStdString() << std::endl;
        std::string pLabel = _gSkill_combo->GetStringSelection().ToStdString();
        int id_skill = _map_allSkill[pLabel];
        //std::cout << "  => " << _list_abbr[event.GetSelection()] << std::endl;
        std::cout << "  => " << SkillDefs[id_skill].abbr << std::endl;

        _item->_item->grantSkill = (const char*) malloc( strlen(SkillDefs[id_skill].abbr)+1 );
        strcpy( (char *) _item->_item->grantSkill, SkillDefs[id_skill].abbr );
    }
}
void ItemView::on_min_grantspin_update( wxSpinEvent& event)
{
    _item->_item->minGrant = _min_grant_spin->GetValue();
}
void ItemView::on_min_grantspin_updateenter( wxCommandEvent& event)
{
    _item->_item->minGrant = _min_grant_spin->GetValue();
}
void ItemView::on_max_grantspin_update( wxSpinEvent& event)
{
    _item->_item->maxGrant = _max_grant_spin->GetValue();
}
void ItemView::on_max_grantspin_updateenter( wxCommandEvent& event)
{
    _item->_item->maxGrant = _max_grant_spin->GetValue();
}
void ItemView::on_fSkillcombo_update( wxCommandEvent& event )
{
    if ( event.GetEventType() == wxEVT_COMBOBOX )
    {
        int comboId = event.GetId() -idGrantSkill;
        //std::cout << "EVT_COMBOBOX(mSkill), selection=" << _gSkill_combo->GetStringSelection().ToStdString() << std::endl;
        //std::cout << "  " << _mSkill_combo.GetStringSelection().ToStdString() << std::endl;
        std::string pLabel = _fSkill_combo[comboId]->GetStringSelection().ToStdString();
        int id_skill = _map_allSkill[pLabel];
        //std::cout << "  => " << _list_abbr[event.GetSelection()] << std::endl;
        //std::cout << "  => " << SkillDefs[id_skill].abbr << std::endl;

        _item->_item->fromSkills[comboId] = (const char*) malloc( strlen(SkillDefs[id_skill].abbr)+1 );
        strcpy( (char *) _item->_item->fromSkills[comboId], SkillDefs[id_skill].abbr );
    }
}
// ********************************************************************* Weapon
void ItemView::on_weaponcheck_update( wxCommandEvent& event )
{
    //std::cout << "EVENT ESC id=" << event.GetId() << std::endl;
    _item->_wtype->flags = 0;
    //std::cout << "STATE " << _losschance_check->GetValue() << " " << _hasskill_check->GetValue() << " " << _esclinear_check->GetValue() << std::endl;
//    switch( event.GetId() ) {
//    case idEscLossChance:
//    }

    if( _needskill_ch->IsChecked() )
        _item->_wtype->flags = _item->_wtype->flags | WeaponType::NEEDSKILL;
    if( _alwaysready_ch->IsChecked() )
        _item->_wtype->flags = _item->_wtype->flags | WeaponType::ALWAYSREADY;
    if( _nodefense_ch->IsChecked() )
        _item->_wtype->flags = _item->_wtype->flags | WeaponType::NODEFENSE;
    if( _nofoot_ch->IsChecked() )
        _item->_wtype->flags = _item->_wtype->flags | WeaponType::NOFOOT;
    if( _nodefense_ch->IsChecked() )
        _item->_wtype->flags = _item->_wtype->flags | WeaponType::NODEFENSE;
    if( _nomount_ch->IsChecked() )
        _item->_wtype->flags = _item->_wtype->flags | WeaponType::NOMOUNT;
    if( _short_ch->IsChecked() )
        _item->_wtype->flags = _item->_wtype->flags | WeaponType::SHORT;
    if( _long_ch->IsChecked() )
        _item->_wtype->flags = _item->_wtype->flags | WeaponType::LONG;
    if( _ranged_ch->IsChecked() )
        _item->_wtype->flags = _item->_wtype->flags | WeaponType::RANGED;
    if( _noattackerskill_ch->IsChecked() )
        _item->_wtype->flags = _item->_wtype->flags | WeaponType::NOATTACKERSKILL;
    if( _riding_bonus_ch->IsChecked() )
        _item->_wtype->flags = _item->_wtype->flags | WeaponType::RIDINGBONUS;
    if( _riding_bonus_defense_ch->IsChecked() )
        _item->_wtype->flags = _item->_wtype->flags | WeaponType::RIDINGBONUSDEFENSE;
}
void ItemView::on_bSkillcombo_update( wxCommandEvent& event )
{
    if ( event.GetEventType() == wxEVT_COMBOBOX )
    {
        std::cout << "EVT_COMBOBOX(bSkill), selection=" << _bSkill_combo->GetStringSelection().ToStdString() << std::endl;
        //std::cout << "  " << _mSkill_combo.GetStringSelection().ToStdString() << std::endl;
        std::string bLabel = _bSkill_combo->GetStringSelection().ToStdString();
        if( bLabel.compare( "NULL" ) == 0 ) {
            _item->_wtype->baseSkill = NULL;
        }
        else {
            int id_skill = _map_bSkill[bLabel];
            //std::cout << "  => " << _list_abbr[event.GetSelection()] << std::endl;
            std::cout << "  => " << SkillDefs[id_skill].abbr << std::endl;

            _item->_wtype->baseSkill = (const char*) malloc( strlen(SkillDefs[id_skill].abbr)+1 );
            strcpy( (char *) _item->_wtype->baseSkill, SkillDefs[id_skill].abbr );
        }
    }
}
void ItemView::on_oSkillcombo_update( wxCommandEvent& event )
{
    if ( event.GetEventType() == wxEVT_COMBOBOX )
    {
        std::cout << "EVT_COMBOBOX(bSkill), selection=" << _oSkill_combo->GetStringSelection().ToStdString() << std::endl;
        //std::cout << "  " << _mSkill_combo.GetStringSelection().ToStdString() << std::endl;
        std::string bLabel = _oSkill_combo->GetStringSelection().ToStdString();
        if( bLabel.compare( "NULL" ) == 0 ) {
            _item->_wtype->orSkill = NULL;
        }
        else {
            int id_skill = _map_bSkill[bLabel];
            //std::cout << "  => " << _list_abbr[event.GetSelection()] << std::endl;
            std::cout << "  => " << SkillDefs[id_skill].abbr << std::endl;

            _item->_wtype->orSkill = (const char*) malloc( strlen(SkillDefs[id_skill].abbr)+1 );
            strcpy( (char *) _item->_wtype->orSkill, SkillDefs[id_skill].abbr );
        }
    }
}
void ItemView::on_weapClasscombo_update( wxCommandEvent& event )
{
    if ( event.GetEventType() == wxEVT_COMBOBOX )
    {
        std::cout << "EVT_COMBOBOX(bSkill), selection=" << _weap_class_combo->GetStringSelection().ToStdString() << std::endl;
        //std::cout << "  " << _mSkill_combo.GetStringSelection().ToStdString() << std::endl;
        std::string wcLabel = _weap_class_combo->GetStringSelection().ToStdString();
        if( wcLabel.compare( "NULL" ) == 0 ) {
            _item->_wtype->weapClass = 0;
        }
        else {
            int id_class = _map_wClass[wcLabel];

            _item->_wtype->weapClass = id_class;
        }
    }
}
void ItemView::on_numattackspin_update( wxSpinEvent& event)
{
    _item->_wtype->numAttacks = _num_attack_spin->GetValue();
    _num_attack_text->SetLabel( wxString( compute_numattack() ));
}
void ItemView::on_numattackspin_updateenter( wxCommandEvent& event)
{
    _item->_wtype->numAttacks = _num_attack_spin->GetValue();
    _num_attack_text->SetLabel( wxString( compute_numattack() ));
}
void ItemView::on_atk_bonusspin_update( wxSpinEvent& event)
{
    _item->_wtype->attackBonus = _atk_bonus_spin->GetValue();
}
void ItemView::on_atk_bonusspin_updateenter( wxCommandEvent& event)
{
    _item->_wtype->attackBonus = _atk_bonus_spin->GetValue();
}
void ItemView::on_def_bonusspin_update( wxSpinEvent& event)
{
    _item->_wtype->defenseBonus = _def_bonus_spin->GetValue();
}
void ItemView::on_def_bonusspin_updateenter( wxCommandEvent& event)
{
    _item->_wtype->defenseBonus = _def_bonus_spin->GetValue();
}
void ItemView::on_mount_bonusspin_update( wxSpinEvent& event)
{
    _item->_wtype->mountBonus = _mount_bonus_spin->GetValue();
}
void ItemView::on_mount_bonusspin_updateenter( wxCommandEvent& event)
{
    _item->_wtype->mountBonus = _mount_bonus_spin->GetValue();
}

void ItemView::on_use_in_assassinatecheck_update( wxCommandEvent& event)
{
    if( _use_in_assassinate_check->GetValue() )
        _item->_atype->flags = _item->_atype->flags | ArmorType::USEINASSASSINATE;
    else
        _item->_atype->flags = _item->_atype->flags ^ ArmorType::USEINASSASSINATE;
}
void ItemView::on_fromspin_update( wxSpinEvent& event)
{
    _item->_atype->from = _from_spin->GetValue();
}
void ItemView::on_fromspin_updateenter( wxCommandEvent& event)
{
    _item->_atype->from = _from_spin->GetValue();
    _saves_armor_text->SetLabel( wxString( compute_armorsaves()) );
}
void ItemView::on_chancesspin_update( wxSpinEvent& event)
{
    std::cout << "EVENT on_chancesspin_update id=" << event.GetId() << std::endl;
    int spinId = event.GetId() -idArmorBonusBase;
    _item->_atype->saves[spinId] = _chances_spin[spinId]->GetValue();
    _saves_armor_text->SetLabel( wxString( compute_armorsaves()) );
}
void ItemView::on_chancesspin_updateenter( wxCommandEvent& event)
{
    int spinId = event.GetId() -idArmorBonusBase;
    _item->_atype->saves[spinId] = _chances_spin[spinId]->GetValue();
    _saves_armor_text->SetLabel( wxString( compute_armorsaves()) );
}
//        wxSpinCtrl *_chances_spin[NUM_WEAPON_CLASSES];
//        wxStaticText *_saves_armor_text;

void ItemView::on_mount_skillcombo_update( wxCommandEvent& event)
{
    if ( event.GetEventType() == wxEVT_COMBOBOX )
    {
        //std::cout << "EVT_COMBOBOX(bSkill), selection=" << _weap_class_combo->GetStringSelection().ToStdString() << std::endl;
        //std::cout << "  " << _mSkill_combo.GetStringSelection().ToStdString() << std::endl;
        std::string skillLabel = _mount_skill_combo->GetStringSelection().ToStdString();
        if( skillLabel.compare( "NULL" ) == 0 ) {
            _item->_mtype->skill = NULL;
        }
        else {
            int id_skill = _map_allSkill[skillLabel];
            //std::cout << "  => " << _list_abbr[event.GetSelection()] << std::endl;
            //std::cout << "  => " << SkillDefs[id_skill].abbr << std::endl;

            _item->_mtype->skill = (const char*) malloc( strlen(SkillDefs[id_skill].abbr)+1 );
            strcpy( (char *) _item->_mtype->skill, SkillDefs[id_skill].abbr );
        }
    }
}
void ItemView::on_mount_maxspin_update( wxSpinEvent& event)
{
    _item->_mtype->maxBonus = _mount_max_spin->GetValue();
}
void ItemView::on_mount_maxspin_updateenter( wxCommandEvent& event)
{
    _item->_mtype->maxBonus = _mount_max_spin->GetValue();
}
void ItemView::on_mount_minspin_update( wxSpinEvent& event)
{
    _item->_mtype->minBonus = _mount_min_spin->GetValue();
}
void ItemView::on_mount_minspin_updateenter( wxCommandEvent& event)
{
    _item->_mtype->minBonus = _mount_min_spin->GetValue();
}
void ItemView::on_mount_hamperedspin_update( wxSpinEvent& event)
{
    _item->_mtype->maxHamperedBonus = _mount_hampered_spin->GetValue();
}
void ItemView::on_mount_hamperedspin_updateenter( wxCommandEvent& event)
{
    _item->_mtype->maxHamperedBonus = _mount_hampered_spin->GetValue();
}
void ItemView::on_mount_specialcombo_update( wxCommandEvent& event)
{
    if ( event.GetEventType() == wxEVT_COMBOBOX )
    {
        //std::cout << "EVT_COMBOBOX(bSkill), selection=" << _weap_class_combo->GetStringSelection().ToStdString() << std::endl;
        //std::cout << "  " << _mSkill_combo.GetStringSelection().ToStdString() << std::endl;
        std::string skillLabel = _mount_skill_combo->GetStringSelection().ToStdString();
        if( skillLabel.compare( "NULL" ) == 0 ) {
            _item->_mtype->mountSpecial = NULL;
        }
        else {
            int id_skill = _map_specSkill[skillLabel];
            //std::cout << "  => " << _list_abbr[event.GetSelection()] << std::endl;
            //std::cout << "  => " << SkillDefs[id_skill].abbr << std::endl;

            _item->_mtype->mountSpecial = (const char*) malloc( strlen(SpecialDefs[id_skill].key)+1 );
            strcpy( (char *) _item->_mtype->mountSpecial, SpecialDefs[id_skill].key );
        }
    }
}
void ItemView::on_mount_special_lvlspin_update( wxSpinEvent& event)
{
    _item->_mtype->specialLev = _mount_special_lvl_spin->GetValue();
}
void ItemView::on_mount_special_lvlspin_updateenter( wxCommandEvent& event)
{
    _item->_mtype->specialLev = _mount_special_lvl_spin->GetValue();
}


void ItemView::on_bat_mageonly_check_update( wxCommandEvent& event)
{
    if( _battle_mageonly_check->GetValue() )
        _item->_btype->flags = _item->_btype->flags | BattleItemType::MAGEONLY ;
    else
        _item->_btype->flags = _item->_btype->flags ^ BattleItemType::MAGEONLY;
}
void ItemView::on_bat_special_check_update( wxCommandEvent& event)
{
    if( _battle_special_check->GetValue() )
        _item->_btype->flags = _item->_btype->flags | BattleItemType::SPECIAL ;
    else
        _item->_btype->flags = _item->_btype->flags ^ BattleItemType::SPECIAL;
}
void ItemView::on_bat_shield_check_update( wxCommandEvent& event)
{
    if( _battle_shield_check->GetValue() )
        _item->_btype->flags = _item->_btype->flags | BattleItemType::SHIELD ;
    else
        _item->_btype->flags = _item->_btype->flags ^ BattleItemType::SHIELD;
}
void ItemView::on_bat_exclusive_check_update( wxCommandEvent& event)
{
    if( _battle_exclusive_check->GetValue() )
        _item->_btype->flags = _item->_btype->flags | BattleItemType::EXCLUSIVE ;
    else
        _item->_btype->flags = _item->_btype->flags ^ BattleItemType::EXCLUSIVE ;
}
void ItemView::on_bat_specialcombo_update( wxCommandEvent& event)
{
    if ( event.GetEventType() == wxEVT_COMBOBOX )
    {
        //std::cout << "EVT_COMBOBOX(bSkill), selection=" << _weap_class_combo->GetStringSelection().ToStdString() << std::endl;
        //std::cout << "  " << _mSkill_combo.GetStringSelection().ToStdString() << std::endl;
        std::string specLabel = _battle_special_combo->GetStringSelection().ToStdString();
        if( specLabel.compare( "NULL" ) == 0 ) {
            _item->_btype->special = NULL;
        }
        else {
            int id_skill = _map_specSkill[specLabel];
            //std::cout << "  => " << _list_abbr[event.GetSelection()] << std::endl;
            //std::cout << "  => " << SkillDefs[id_skill].abbr << std::endl;

            _item->_btype->special = (const char*) malloc( strlen(SpecialDefs[id_skill].key)+1 );
            strcpy( (char *) _item->_btype->special, SpecialDefs[id_skill].key );
        }
    }
}
void ItemView::on_bat_specialspin_update( wxSpinEvent& event)
{
    _item->_btype->skillLevel = _battle_lvl_spin->GetValue();
}
void ItemView::on_bat_specialspin_updateenter( wxCommandEvent& event)
{
    _item->_btype->skillLevel = _battle_lvl_spin->GetValue();
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
    _nomarket_check->SetValue( item->_item->flags & ItemType::NOMARKET );
    _orinput_check->SetValue( item->_item->flags & ItemType::ORINPUTS );
    _skillout_check->SetValue( item->_item->flags & ItemType::SKILLOUT );
    _notransport_check->SetValue( item->_item->flags & ItemType::NOTRANSPORT );

    // production
    if( item->_item->pSkill != NULL ) {
        // Find proper Skill
        for( int i=0; i < NSKILLS; ++i ) {
            std::cout << "CMP " << SkillDefs[i].abbr << " with " << item->_item->pSkill << std::endl;
            if( strcmp( SkillDefs[i].abbr, item->_item->pSkill) == 0 ) {
                std::cout << "  => FOUND" << std::endl;
                std::string pLabel = std::string( SkillDefs[i].name);
                pLabel += " [" + std::string( SkillDefs[i].abbr) + "]";
                //_map_pSkill[pLabel] = i;
                _pSkill_combo->SetValue( wxString(pLabel));
                break;
            }
        }
        _pLevel_spin->SetValue( item->_item->pLevel );
        _pMonth_spin->SetValue( item->_item->pMonths );
        _pOut_spin->SetValue( item->_item->pOut );
    }
    else {
//        _mSkill_text->Clear();
//        _mSkill_text->AppendText( _("-") );
        _pSkill_combo->SetValue( wxString("NULL") );
        _pLevel_spin->SetValue( 0 );
        _pMonth_spin->SetValue( 0 );
        _pOut_spin->SetValue( 0 );
    }
    for( int k=0; k<4; ++k ) {
        if( item->_item->pInput[k].item >= 0 ) {
            for( auto& allitem : _data._all_enumitems ) {
                if( allitem.second == item->_item->pInput[k].item)
                    _pMat_combo[k]->SetValue( wxString( allitem.first ));
            }
        }
        else {
            _pMat_combo[k]->SetValue( wxString( "NULL" ));
        }
        _pMat_spin[k]->SetValue( item->_item->pInput[k].amt );
    }
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
    for( int k=0; k<4; ++k ) {
        if( item->_item->mInput[k].item >= 0 ) {
            for( auto& allitem : _data._all_enumitems ) {
                if( allitem.second == item->_item->mInput[k].item)
                    _mMat_combo[k]->SetValue( wxString( allitem.first ));
            }
        }
        else {
            _mMat_combo[k]->SetValue( wxString( "NULL" ));
        }
        _mMat_spin[k]->SetValue( item->_item->mInput[k].amt );
    }

    // weight
    _weight_spin->SetValue( item->_item->weight );
    _baseprice_spin->SetValue( item->_item->baseprice );
    _combat_spin->SetValue( item->_item->combat );

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
    _hitchitem_spin->SetValue( item->_item->hitchItem );
    _hitchwalk_spin->SetValue( item->_item->hitchwalk );
    _mult_item_spin->SetValue( item->_item->mult_item );
    _mult_val_spin->SetValue( item->_item->mult_val );
    _maxinventory_spin->SetValue( item->_item->mult_val );

    // Grant
    if( item->_item->grantSkill != NULL ) {
        // Find proper Skill
        if( strcmp( "", item->_item->grantSkill) == 0 ) {
            _gSkill_combo->SetValue( wxString("NULL") );
            _min_grant_spin->SetValue( 0 );
            _max_grant_spin->SetValue( 0 );
        }
        else {
            for( int i=0; i < NSKILLS; ++i ) {
                std::cout << "CMP " << SkillDefs[i].abbr << " with " << item->_item->grantSkill << std::endl;
                if( strcmp( SkillDefs[i].abbr, item->_item->grantSkill) == 0 ) {
                    std::cout << "  => FOUND" << std::endl;
                    std::string pLabel = std::string( SkillDefs[i].name);
                    pLabel += " [" + std::string( SkillDefs[i].abbr) + "]";
                    //_map_pSkill[pLabel] = i;
                    _gSkill_combo->SetValue( wxString(pLabel));
                    break;
                }
            }
            _min_grant_spin->SetValue( item->_item->minGrant );
            _max_grant_spin->SetValue( item->_item->maxGrant );
        }
    }
    else {
//        _mSkill_text->Clear();
//        _mSkill_text->AppendText( _("-") );
        _gSkill_combo->SetValue( wxString("NULL") );
        _min_grant_spin->SetValue( 0 );
        _max_grant_spin->SetValue( 0 );
    }

    for( int k=0; k<4; ++k ) {
        if( item->_item->fromSkills[k] != NULL ) {
            // Find proper Skill
            if( strcmp( "", item->_item->fromSkills[k]) == 0 ) {
                _fSkill_combo[k]->SetValue( wxString("NULL") );
            }
            else {
                for( int i=0; i < NSKILLS; ++i ) {
                    std::cout << "CMP " << SkillDefs[i].abbr << " with " << item->_item->fromSkills[k] << std::endl;
                    if( strcmp( SkillDefs[i].abbr, item->_item->fromSkills[k] ) == 0 ) {
                        std::cout << "  => FOUND" << std::endl;
                        std::string pLabel = std::string( SkillDefs[i].name);
                        pLabel += " [" + std::string( SkillDefs[i].abbr) + "]";
                        //_map_pSkill[pLabel] = i;
                        _fSkill_combo[k]->SetValue( wxString(pLabel));
                        break;
                    }
                }
            }
        }
        else {
//        _mSkill_text->Clear();
//        _mSkill_text->AppendText( _("-") );
            _fSkill_combo[k]->SetValue( wxString("NULL") );
        }
    }

    // Weapon ? -> setweapon
    if( item->_item->type & IT_WEAPON ) {
        _weapon_panel->Enable( true );
        _needskill_ch->SetValue( item->_wtype->flags & WeaponType::NEEDSKILL );
        _alwaysready_ch->SetValue( item->_wtype->flags & WeaponType::ALWAYSREADY );
        _nodefense_ch->SetValue( item->_wtype->flags & WeaponType::NODEFENSE );
        _nofoot_ch->SetValue( item->_wtype->flags & WeaponType::NOFOOT );
        _nomount_ch->SetValue( item->_wtype->flags & WeaponType::NOMOUNT );
        _short_ch->SetValue( item->_wtype->flags & WeaponType::SHORT );
        _long_ch->SetValue( item->_wtype->flags & WeaponType::LONG );
        _ranged_ch->SetValue( item->_wtype->flags & WeaponType::RANGED );
        _noattackerskill_ch->SetValue( item->_wtype->flags & WeaponType::NOATTACKERSKILL );
        _riding_bonus_ch->SetValue( item->_wtype->flags & WeaponType::RIDINGBONUS );
        _riding_bonus_defense_ch->SetValue( item->_wtype->flags & WeaponType::RIDINGBONUSDEFENSE );

        // Skills
        if( item->_wtype->baseSkill != NULL ) {
            // Find proper Skill
            for( int i=0; i < NSKILLS; ++i ) {
                //std::cout << "CMP " << SkillDefs[i].abbr << " with " << item->_wtype->baseSkill << std::endl;
                if( strcmp( SkillDefs[i].abbr, item->_wtype->baseSkill) == 0 ) {
                    //std::cout << "  => FOUND" << std::endl;
                    std::string bLabel = std::string( SkillDefs[i].name);
                    //_map_bSkill[bLabel] = i;
                    _bSkill_combo->SetValue( wxString(bLabel));
                    break;
                }
            }
        }
        else {
            _bSkill_combo->SetValue( wxString("NULL") );
        }
        if( item->_wtype->orSkill != NULL ) {
            // Find proper Skill
            for( int i=0; i < NSKILLS; ++i ) {
                //std::cout << "CMP " << SkillDefs[i].abbr << " with " << item->_wtype->orSkill << std::endl;
                if( strcmp( SkillDefs[i].abbr, item->_wtype->orSkill) == 0 ) {
                    //std::cout << "  => FOUND" << std::endl;
                    std::string oLabel = std::string( SkillDefs[i].name);
                    //_map_bSkill[bLabel] = i;
                    _oSkill_combo->SetValue( wxString(oLabel));
                    break;
                }
            }
        }
        else {
            _oSkill_combo->SetValue( wxString("NULL") );
        }
        std::string attack_enum[] = {"SLASHING","PIERCING","CRUSHING","CLEAVING","ARMORPIERCING","MAGIC_ENERGY","MAGIC_SPIRIT","MAGIC_WEATHER","NUM_WEAPON_CLASSES"};
        _weap_class_combo->SetValue( wxString(attack_enum[item->_wtype->weapClass]));

        _num_attack_spin->SetValue( item->_wtype->numAttacks );
        _atk_bonus_spin->SetValue( item->_wtype->attackBonus );
        _def_bonus_spin->SetValue( item->_wtype->defenseBonus );
        _mount_bonus_spin->SetValue( item->_wtype->mountBonus );
    }
    else { // Not a Weapon
        _weapon_panel->Enable( false );
        _bSkill_combo->SetValue( wxString("NULL") );
        _oSkill_combo->SetValue( wxString("NULL") );
        _weap_class_combo->SetValue( wxString("NULL") );

        _num_attack_spin->SetValue( 0 );
        _atk_bonus_spin->SetValue( 0 );
        _def_bonus_spin->SetValue( 0 );
        _mount_bonus_spin->SetValue( 0 );

        _needskill_ch->SetValue( false );
        _alwaysready_ch->SetValue( false );
        _nodefense_ch->SetValue( false );
        _nofoot_ch->SetValue( false );
        _nomount_ch->SetValue( false );
        _short_ch->SetValue( false );
        _long_ch->SetValue( false );
        _ranged_ch->SetValue( false );
        _noattackerskill_ch->SetValue( false );
        _riding_bonus_ch->SetValue( false );
        _riding_bonus_defense_ch->SetValue( false );
    }
    _num_attack_text->SetLabel( wxString( compute_numattack() ));

    // Armor
    if( item->_item->type & IT_ARMOR ) {
        _armor_panel->Enable( true );
        _use_in_assassinate_check->SetValue( item->_atype->flags & ArmorType::USEINASSASSINATE );
        _from_spin->SetValue( item->_atype->from );
        for( int i=0; i < NUM_WEAPON_CLASSES; ++i ) {
            _chances_spin[i]->SetValue( item->_atype->saves[i] );
        }
    }
    else { // No ARMOR
        _armor_panel->Enable( false );
        _use_in_assassinate_check->SetValue( false );
        _from_spin->SetValue( 0 );
        for( int i=0; i < NUM_WEAPON_CLASSES; ++i ) {
            _chances_spin[i]->SetValue( 0 );
        }
    }
    _saves_armor_text->SetLabel( wxString( compute_armorsaves()) );

    // Mount
    if( item->_item->type & IT_MOUNT ) {
        _mount_panel->Enable( true );
        if( item->_mtype->skill != NULL ) {
            // Find proper Skill
            for( int i=0; i < NSKILLS; ++i ) {
                //std::cout << "CMP " << SkillDefs[i].abbr << " with " << item->_wtype->orSkill << std::endl;
                if( strcmp( SkillDefs[i].abbr, item->_mtype->skill) == 0 ) {
                    //std::cout << "  => FOUND" << std::endl;
                    std::string allLabel = std::string( SkillDefs[i].name);
                    allLabel += " [" + std::string( SkillDefs[i].abbr) + "]";
                    //_map_bSkill[bLabel] = i;
                    _mount_skill_combo->SetValue( wxString(allLabel));
                    break;
                }
            }
        }
        else {
            _mount_skill_combo->SetValue( wxString("NULL") );
        }
        _mount_min_spin->SetValue( item->_mtype->minBonus );
        _mount_max_spin->SetValue( item->_mtype->maxBonus );
        _mount_hampered_spin->SetValue( item->_mtype->maxHamperedBonus );
        if( item->_mtype->mountSpecial != NULL ) {
            // Find proper Skill
            for( int i=1; i<NUMSPECIALS; ++i) {
                if( strcmp( SpecialDefs[i].key, item->_mtype->mountSpecial) == 0 ) {
                    std::string sLabel = std::string( SpecialDefs[i].key);
                    _mount_special_combo->SetValue( wxString( sLabel ));
                    break;
                }
            }
        }
        else {
            _mount_special_combo->SetValue( wxString( "NULL" ));
        }
        _mount_special_lvl_spin->SetValue( item->_mtype->specialLev );
    }
    else { // No Mount
        _mount_panel->Enable( false );
        _mount_skill_combo->SetValue( wxString("NULL") );
        _mount_min_spin->SetValue( 0 );
        _mount_max_spin->SetValue( 0 );
        _mount_hampered_spin->SetValue( 0 );
        _mount_special_combo->SetValue( wxString( "NULL" ));
        _mount_special_lvl_spin->SetValue( 0 );
    }

    // Battle
    if( item->_item->type & IT_BATTLE ) {
//        std::cout << "***** BATTLE *****" << std::endl;
//        item->write_type_battle( std::cout );
        _battle_panel->Enable( true );

        _battle_mageonly_check->SetValue( item->_btype->flags & BattleItemType::MAGEONLY );
        _battle_special_check->SetValue( item->_btype->flags & BattleItemType::SPECIAL );
        _battle_shield_check->SetValue( item->_btype->flags & BattleItemType::SHIELD );
        _battle_exclusive_check->SetValue( item->_btype->flags & BattleItemType::EXCLUSIVE );

        if( item->_btype->special != NULL ) {
            // Find proper Skill
            for( int i=1; i < NUMSPECIALS; ++i ) {
//                std::cout << "i=" << i << std::endl;
//                std::cout << "CMP " << SpecialDefs[i].key << " with " << item->_btype->special << std::endl;
                if( strcmp( SpecialDefs[i].key, item->_btype->special) == 0 ) {
//                    std::cout << "  => FOUND" << std::endl;
                    std::string specLabel = std::string( SpecialDefs[i].key);
                    //_map_bSkill[bLabel] = i;
                    _battle_special_combo->SetValue( wxString(specLabel));
                    break;
                }
            }
        }
        else {
            _battle_special_combo->SetValue( wxString("NULL") );
        }
        _battle_lvl_spin->SetValue( item->_btype->skillLevel );
    }
    else { // No BATTLE
        _battle_panel->Enable( false );
        _battle_mageonly_check->SetValue( false );
        _battle_special_check->SetValue( false );
        _battle_shield_check->SetValue( false );
        _battle_exclusive_check->SetValue( false );
        _battle_special_combo->SetValue( wxString("NULL") );
        _battle_lvl_spin->SetValue( 0 );
    }
}
// ************************************************************** compute_numattack
std::string ItemView::compute_numattack()
{
    std::stringstream msg;
    msg << "NumAttack : ";
    // a weapon
    if( !_item ) {
        msg << " -";
    }
    else if( _item->_item->type & IT_WEAPON) {
        int num = _item->_wtype->numAttacks;
        if( num < 0 ) {
            msg << " 1 atk / " << (-num) << " rnds";
        }
        else if( num < 50) {
            msg << num << " atk / rnd";
        }
        else if( num < 100 ) {
            msg << "skillLevel/2 + " << num-50 << " atk / rnd";
        }
        else {
            msg << "skillLevel + " << num-100 << " atk / rnd";
        }
    }
    else {
        msg << " -";
    }
    return msg.str();
}
std::string ItemView::compute_armorsaves()
{
    std::string attack_enum_str[] = {"Slash","Pierce","Crush","Cleave","ArmorPierce",
    "MAG_NRJ","MAG_SPI","MAG_WEATHER"};
    std::stringstream msg;
    msg << "ArmorEffect :";
    // a weapon
    if( !_item ) {
        msg << " -";
    }
    else if( _item->_item->type & IT_ARMOR) {
        for( int i=0; i<NUM_WEAPON_CLASSES; ++i ) {
            msg << " " << attack_enum_str[i] << "=" << std::fixed << std::setprecision(0)<< (float) _item->_atype->saves[i] / (float) _item->_atype->from *100.f << "%";
        }
    }
    else {
        msg << " -";
    }

    return msg.str();
}
