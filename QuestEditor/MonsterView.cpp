#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "MonsterView.h"
#include <items.h>
#include <skills.h>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <wx_utils.h>

wxBEGIN_EVENT_TABLE(MonsterView, wxPanel)
    EVT_COMBOBOX(idAbbrCombo, MonsterView::on_cbox_update)
    EVT_TEXT(idAbbrCombo, MonsterView::on_cbox_update)
    EVT_TEXT_ENTER(idAbbrCombo, MonsterView::on_cbox_update)
wxEND_EVENT_TABLE()

MonsterView::MonsterView(wxWindow *parent, AllData& data)
    : wxPanel( parent ), _data(data), _monster(nullptr)
{
    // read file
    //parse_gamedata();

    // main Vertical
    wxBoxSizer *main_vbox = new wxBoxSizer( wxVERTICAL );

    // Identification
    wxBoxSizer *id_hbox = new wxBoxSizer( wxHORIZONTAL );
    mk_title(this, id_hbox, "Identifiant : ");
    _abbr_combo = new wxComboBox(this, idAbbrCombo, _T(""), wxDefaultPosition, wxDefaultSize,
                                0, NULL, wxTE_PROCESS_ENTER | wxTE_PROCESS_TAB);
   //_abbr_combo->Bind(wxEVT_COMBOBOX, &MonsterView::on_cbox_update, this );
    // all abbr
//    for( int i=0; i<NITEMS; ++i) {
//            if( ItemDefs[i].type & IT_MONSTER ) {
//            _list_abbr.push_back( wxString(ItemDefs[i].abr));
//            _abbr_combo->Append( wxString(ItemDefs[i].abr));
//            }
//    }
    // add all labels
    for( auto& monster : _data._all_monsters ) {
        _abbr_combo->Append( wxString( monster.second._item_enum ));
    }
//    std::map<std::string,int>::iterator it;
//    for (it=_map_abbr.begin(); it!=_map_abbr.end(); ++it) {
//        //_list_abbr.push_back( wxString( it->first));
//        _abbr_combo->Append( wxString( it->first));
//    }
    id_hbox->Add( _abbr_combo, 0, wxEXPAND | wxRIGHT | wxALIGN_CENTER_VERTICAL, 20);

    mk_check( this, wxID_ANY, id_hbox, "Edited", _edit_check );
    _edit_check->Bind( wxEVT_CHECKBOX, &MonsterView::on_edit_update, this);

    main_vbox->Add( id_hbox, 0, wxEXPAND, 0 );
    main_vbox->AddSpacer( 20 );

    _monster_panel = new wxPanel( this, wxID_ANY );
    _monster_panel->Enable(false);
    wxBoxSizer *monster_vbox = new wxBoxSizer( wxVERTICAL );

    wxBoxSizer *name_hbox = new wxBoxSizer( wxHORIZONTAL );
    mk_field( _monster_panel, wxID_ANY, name_hbox, "abbr:", _abbr_text, 0, 80);
    _abbr_text->Bind( wxEVT_TEXT_ENTER, &MonsterView::on_abbrtext_update, this );
    mk_field( _monster_panel, wxID_ANY, name_hbox, "name:", _name_text, 1, 200);
    _name_text->Bind( wxEVT_TEXT_ENTER, &MonsterView::on_nametext_update, this );
    mk_field( _monster_panel, wxID_ANY, name_hbox, "names:", _names_text, 1, 200);
    _names_text->Bind( wxEVT_TEXT_ENTER, &MonsterView::on_namestext_update, this );
    monster_vbox->Add( name_hbox, 0, wxEXPAND, 0 );

    // flags
    wxBoxSizer *flags_hbox = new wxBoxSizer( wxHORIZONTAL );
    mk_title( _monster_panel, flags_hbox, "Flags: ");
    mk_check( _monster_panel, wxID_ANY, flags_hbox, "Disabled", _disabled_check );
    _disabled_check->Bind( wxEVT_CHECKBOX, &MonsterView::on_disabled_update, this);
    mk_check( _monster_panel, wxID_ANY, flags_hbox, "Can't give", _cantgive_check );
    _cantgive_check->Bind( wxEVT_CHECKBOX, &MonsterView::on_cantgive_update, this);
    monster_vbox->Add( flags_hbox, 0, wxEXPAND, 0 );

    // magical skill
    wxBoxSizer *magic_hbox = new wxBoxSizer( wxHORIZONTAL );
    mk_title( _monster_panel, magic_hbox, "Magical Production: skill ");
    //mk_field( this, wxID_ANY, magic_hbox, "Skill:", _mSkill_text, 0, 80);
    _mSkill_combo = new wxComboBox(_monster_panel, idAbbrCombo, _T(""), wxDefaultPosition, wxDefaultSize,
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
    _mSkill_combo->Bind( wxEVT_COMBOBOX, &MonsterView::on_mSkillcombo_update, this);
    magic_hbox->Add( _mSkill_combo, 0, wxEXPAND | wxRIGHT | wxALIGN_CENTER_VERTICAL, 20);
    mk_spin( _monster_panel, wxID_ANY, magic_hbox, "Level:", _mLevel_spin, 0, 10, 50);
    _mLevel_spin->Bind( wxEVT_SPINCTRL, &MonsterView::on_mLevelspin_update, this);
    _mLevel_spin->Bind( wxEVT_TEXT, &MonsterView::on_mLevelspin_updateenter, this);
    _mLevel_spin->Bind( wxEVT_TEXT_ENTER, &MonsterView::on_mLevelspin_updateenter, this);
    //mk_title( _monster_panel, magic_hbox, "Materials: TODO");
    monster_vbox->Add( magic_hbox, 0, wxEXPAND, 0 );

    // weight, baseprice
    wxBoxSizer *weight_hbox = new wxBoxSizer( wxHORIZONTAL );
    mk_spin( _monster_panel, wxID_ANY, weight_hbox, "Weight:", _weight_spin, 0, 1000, 200);
    _weight_spin->Bind( wxEVT_SPINCTRL, &MonsterView::on_weightspin_update, this);
    _weight_spin->Bind( wxEVT_TEXT, &MonsterView::on_weightspin_updateenter, this);
    _weight_spin->Bind( wxEVT_TEXT_ENTER, &MonsterView::on_weightspin_updateenter, this);
    mk_spin( _monster_panel, wxID_ANY, weight_hbox, "Baseprice:", _baseprice_spin, 0, 1000, 200);
    _baseprice_spin->Bind( wxEVT_SPINCTRL, &MonsterView::on_basepricespin_update, this);
    _baseprice_spin->Bind( wxEVT_TEXT, &MonsterView::on_basepricespin_updateenter, this);
    _baseprice_spin->Bind( wxEVT_TEXT_ENTER, &MonsterView::on_basepricespin_updateenter, this);
    monster_vbox->Add( weight_hbox, 0, wxEXPAND, 0 );

    // type
    wxBoxSizer *type_hbox = new wxBoxSizer( wxHORIZONTAL );
    mk_title( _monster_panel, type_hbox, "Type:");
    mk_check( _monster_panel, wxID_ANY, type_hbox, "Animal", _animal_check );
    _animal_check->Bind( wxEVT_CHECKBOX, &MonsterView::on_animal_update, this);
    mk_check( _monster_panel, wxID_ANY, type_hbox, "Demon", _demon_check );
    _demon_check->Bind( wxEVT_CHECKBOX, &MonsterView::on_demon_update, this);
    mk_check( _monster_panel, wxID_ANY, type_hbox, "Illusion", _illusion_check );
    _illusion_check->Bind( wxEVT_CHECKBOX, &MonsterView::on_illusion_update, this);
    mk_check( _monster_panel, wxID_ANY, type_hbox, "Undead", _undead_check );
    _undead_check->Bind( wxEVT_CHECKBOX, &MonsterView::on_undead_update, this);
    monster_vbox->Add( type_hbox, 0, wxEXPAND, 0 );

    // Movements
    wxBoxSizer *move_hbox = new wxBoxSizer( wxHORIZONTAL );
    mk_title( _monster_panel, move_hbox, "Movements: ");
    mk_spin( _monster_panel, wxID_ANY, move_hbox, "Walk", _walk_spin, 0, 1000, 100);
    _walk_spin->Bind( wxEVT_SPINCTRL, &MonsterView::on_walkspin_update, this);
    _walk_spin->Bind( wxEVT_TEXT, &MonsterView::on_walkspin_updateenter, this);
    _walk_spin->Bind( wxEVT_TEXT_ENTER, &MonsterView::on_walkspin_updateenter, this);
    mk_spin( _monster_panel, wxID_ANY, move_hbox, "Ride", _ride_spin, 0, 1000, 100);
    _ride_spin->Bind( wxEVT_SPINCTRL, &MonsterView::on_ridespin_update, this);
    _ride_spin->Bind( wxEVT_TEXT, &MonsterView::on_ridespin_updateenter, this);
    _ride_spin->Bind( wxEVT_TEXT_ENTER, &MonsterView::on_ridespin_updateenter, this);
    mk_spin( _monster_panel, wxID_ANY, move_hbox, "Fly", _fly_spin, 0, 1000, 100);
    _fly_spin->Bind( wxEVT_SPINCTRL, &MonsterView::on_flyspin_update, this);
    _fly_spin->Bind( wxEVT_TEXT, &MonsterView::on_flyspin_updateenter, this);
    _fly_spin->Bind( wxEVT_TEXT_ENTER, &MonsterView::on_flyspin_updateenter, this);
    mk_spin( _monster_panel, wxID_ANY, move_hbox, "Swim", _swim_spin, 0, 1000, 100);
    _swim_spin->Bind( wxEVT_SPINCTRL, &MonsterView::on_swimspin_update, this);
    _swim_spin->Bind( wxEVT_TEXT, &MonsterView::on_swimspin_updateenter, this);
    _swim_spin->Bind( wxEVT_TEXT_ENTER, &MonsterView::on_swimspin_updateenter, this);
    mk_spin( _monster_panel, wxID_ANY, move_hbox, "Speed", _speed_spin, 0, 20, 80);
    _speed_spin->Bind( wxEVT_SPINCTRL, &MonsterView::on_speedspin_update, this);
    _speed_spin->Bind( wxEVT_TEXT, &MonsterView::on_speedspin_updateenter, this);
    _speed_spin->Bind( wxEVT_TEXT_ENTER, &MonsterView::on_speedspin_updateenter, this);
    monster_vbox->Add( move_hbox, 0, wxEXPAND, 0 );

    // Max Inventory
    wxBoxSizer *inventory_hbox = new wxBoxSizer( wxHORIZONTAL );
    mk_spin( _monster_panel, wxID_ANY, inventory_hbox, "Max Inventory:", _maxinventory_spin, 0, 1000, 150);
    _maxinventory_spin->Bind( wxEVT_SPINCTRL, &MonsterView::on_maxinventoryspin_update, this);
    _maxinventory_spin->Bind( wxEVT_TEXT, &MonsterView::on_maxinventoryspin_updateenter, this);
    _maxinventory_spin->Bind( wxEVT_TEXT_ENTER, &MonsterView::on_maxinventoryspin_updateenter, this);
    monster_vbox->Add( inventory_hbox, 0, wxEXPAND, 0 );

    // Escape
    wxBoxSizer *esc_main_hbox = new wxBoxSizer( wxHORIZONTAL );
    mk_check( _monster_panel, wxID_ANY, esc_main_hbox, "Can Escape", _escape_check );
    _escape_check->Bind( wxEVT_CHECKBOX, &MonsterView::on_escape_update, this);
    monster_vbox->Add( esc_main_hbox, 0, wxEXPAND, 0 );

    _escape_panel = new wxPanel( _monster_panel, wxID_ANY );
    wxBoxSizer *esc_vbox = new wxBoxSizer( wxVERTICAL );

    wxBoxSizer *esc_way_hbox = new wxBoxSizer( wxHORIZONTAL );
    esc_way_hbox->AddSpacer(30);
    mk_title( _escape_panel, esc_way_hbox, "Escape mode:");
    mk_check( _escape_panel, idEscLossChance, esc_way_hbox, "Loss chance", _losschance_check );
    _losschance_check->Bind( wxEVT_CHECKBOX, &MonsterView::on_esccheck_update, this);
    mk_check( _escape_panel, idEscHasSkill, esc_way_hbox, "Has skill", _hasskill_check );
    _hasskill_check->Bind( wxEVT_CHECKBOX, &MonsterView::on_esccheck_update, this);
    mk_check( _escape_panel, idEscLinear, esc_way_hbox, "EscLINEAR", _esclinear_check );
    _esclinear_check->Bind( wxEVT_CHECKBOX, &MonsterView::on_esccheck_update, this);
    mk_check( _escape_panel, idEscSquare, esc_way_hbox, "EscSQUARE", _escsquare_check );
    _escsquare_check->Bind( wxEVT_CHECKBOX, &MonsterView::on_esccheck_update, this);
    mk_check( _escape_panel, idEscCube, esc_way_hbox, "EscCUBE", _esccube_check );
    _esccube_check->Bind( wxEVT_CHECKBOX, &MonsterView::on_esccheck_update, this);
    mk_check( _escape_panel, idEscQuad, esc_way_hbox, "EscQUAD", _escquad_check );
    _escquad_check->Bind( wxEVT_CHECKBOX, &MonsterView::on_esccheck_update, this);
//
//    _hasskill_radio = new wxRadioButton( _escape_panel, wxID_ANY, "has skill",
//                                        wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
//    _hasskill_radio->Bind( wxEVT_RADIOBUTTON, &MonsterView::on_esccheck_update, this);
//    esc_way_hbox->Add( _hasskill_radio, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 20);
//    mk_radio( _escape_panel, wxID_ANY, esc_way_hbox, "Linear", _esclinear_radio );
//    _esclinear_radio->Bind( wxEVT_RADIOBUTTON, &MonsterView::on_esccheck_update, this);
//    mk_radio( _escape_panel, wxID_ANY, esc_way_hbox, "Square", _escsquare_radio );
//    _escsquare_radio->Bind( wxEVT_RADIOBUTTON, &MonsterView::on_esccheck_update, this);
//    mk_radio( _escape_panel, wxID_ANY, esc_way_hbox, "Cube", _esccube_radio );
//    _esccube_radio->Bind( wxEVT_RADIOBUTTON, &MonsterView::on_esccheck_update, this);
//    mk_radio( _escape_panel, wxID_ANY, esc_way_hbox, "Quad", _escquad_radio );
//    _escquad_radio->Bind( wxEVT_RADIOBUTTON, &MonsterView::on_esccheck_update, this);
//    mk_radio( _escape_panel, wxID_ANY, esc_way_hbox, "Loss chance", _losschance_radio );
//    _losschance_radio->Bind( wxEVT_RADIOBUTTON, &MonsterView::on_esccheck_update, this);
//    mk_field( _escape_panel, wxID_ANY, esc_way_hbox, "Skill:", _eSkill_text, 0, 80);
//    _eSkill_text->Bind( wxEVT_TEXT_ENTER, &MonsterView::on_esctext_update, this );

    _eSkill_combo = new wxComboBox(_escape_panel, idAbbrCombo, _T(""), wxDefaultPosition, wxDefaultSize,
                                0, NULL, wxTE_PROCESS_ENTER | wxTE_PROCESS_TAB);
    _eSkill_combo->Append( wxString("NULL"));
    for( int i=0; i<NSKILLS; ++i) {
        //if( SkillDefs[i].flags & SkillType::MAGIC ) {
        std::string eLabel = std::string( SkillDefs[i].name);
        eLabel += " [" + std::string( SkillDefs[i].abbr) + "]";
        _map_eSkill[eLabel] = i;
        _eSkill_combo->Append( wxString(eLabel));
        //std::cout << mLabel << "=>" << i << std::endl;
        //}
    }
    _eSkill_combo->SetValue( wxString("NULL") );
    _eSkill_combo->Bind( wxEVT_COMBOBOX, &MonsterView::on_eSkillcombo_update, this);
    esc_way_hbox->Add( _eSkill_combo, 0, wxRIGHT | wxEXPAND | wxALIGN_CENTER_VERTICAL, 20 );
    //magic_hbox->Add( _mSkill_combo, 0, wxEXPAND | wxRIGHT | wxALIGN_CENTER_VERTICAL, 20);

    mk_spin( _escape_panel, wxID_ANY, esc_way_hbox, "Level/Val:", _esc_spin, 1, 10, 50);
    _esc_spin->Bind( wxEVT_SPINCTRL, &MonsterView::on_escspin_update, this);
    _esc_spin->Bind( wxEVT_TEXT, &MonsterView::on_escspin_updateenter, this);
    _esc_spin->Bind( wxEVT_TEXT_ENTER, &MonsterView::on_escspin_updateenter, this);
    esc_vbox->Add( esc_way_hbox );

    _esc_opt_panel = new wxPanel( _escape_panel, wxID_ANY );
    wxBoxSizer * esc_opt_hbox = new wxBoxSizer( wxHORIZONTAL );
    esc_opt_hbox->AddSpacer(30);
    mk_title( _esc_opt_panel, esc_opt_hbox, "Escape options:");
    mk_check( _esc_opt_panel, idEscLinked, esc_opt_hbox, "Lose Linked", _loselinked_check );
    _loselinked_check->Bind( wxEVT_CHECKBOX, &MonsterView::on_esccheck_update, this);
    mk_check( _esc_opt_panel, idEscNumSquare, esc_opt_hbox, "Esc num square", _escnum_check );
    _escnum_check->Bind( wxEVT_CHECKBOX, &MonsterView::on_esccheck_update, this);
    _esc_opt_panel->SetSizer( esc_opt_hbox );
    _esc_opt_panel->Enable( false );
    esc_vbox->Add( _esc_opt_panel );

    wxBoxSizer *esc_effet_hbox = new wxBoxSizer( wxHORIZONTAL );
    esc_effet_hbox->AddSpacer(30);
    _esc_text = new wxStaticText( _escape_panel, wxID_ANY, wxString(compute_escape()));
    esc_effet_hbox->Add( _esc_text, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 2 );
    esc_vbox->Add( esc_effet_hbox );
    _escape_panel->SetSizer( esc_vbox );

    monster_vbox->Add( _escape_panel, 0, wxEXPAND, 0 );

    // Attack
    wxBoxSizer *atk_hbox = new wxBoxSizer( wxHORIZONTAL );
    mk_spin( _monster_panel, wxID_ANY, atk_hbox, "Attack Level:", _atk_spin, -10, 10, 80);
    _atk_spin->Bind( wxEVT_SPINCTRL, &MonsterView::on_atkspin_update, this);
    _atk_spin->Bind( wxEVT_TEXT, &MonsterView::on_atkspin_updateenter, this);
    _atk_spin->Bind( wxEVT_TEXT_ENTER, &MonsterView::on_atkspin_updateenter, this);
    mk_spin( _monster_panel, wxID_ANY, atk_hbox, "Num Attacks:", _numatk_spin, 0, 10, 50);
    _numatk_spin->Bind( wxEVT_SPINCTRL, &MonsterView::on_numatkspin_update, this);
    _numatk_spin->Bind( wxEVT_TEXT, &MonsterView::on_numatkspin_updateenter, this);
    _numatk_spin->Bind( wxEVT_TEXT_ENTER, &MonsterView::on_numatkspin_updateenter, this);
    mk_spin( _monster_panel, wxID_ANY, atk_hbox, "Nb Hits:", _nbhits_spin, 0, 10, 200);
    _nbhits_spin->Bind( wxEVT_SPINCTRL, &MonsterView::on_nbhitsspin_update, this);
    _nbhits_spin->Bind( wxEVT_TEXT, &MonsterView::on_nbhitsspin_updateenter, this);
    _nbhits_spin->Bind( wxEVT_TEXT_ENTER, &MonsterView::on_nbhitsspin_updateenter, this);
    mk_spin( _monster_panel, wxID_ANY, atk_hbox, "Regen:", _regen_spin, 0, 10, 200);
    _regen_spin->Bind( wxEVT_SPINCTRL, &MonsterView::on_regenspin_update, this);
    _regen_spin->Bind( wxEVT_TEXT, &MonsterView::on_regenspin_updateenter, this);
    _regen_spin->Bind( wxEVT_TEXT_ENTER, &MonsterView::on_regenspin_updateenter, this);
    monster_vbox->Add( atk_hbox, 0, wxEXPAND, 0 );

    // Defense
    wxBoxSizer *def_hbox = new wxBoxSizer( wxHORIZONTAL );
    mk_title( _monster_panel, def_hbox, "Defense: ");
    mk_spin( _monster_panel, wxID_ANY, def_hbox, "Combat:", _def_combat_spin, -10, 10, 80);
    _def_combat_spin->Bind( wxEVT_SPINCTRL, &MonsterView::on_defcombatspin_update, this);
    _def_combat_spin->Bind( wxEVT_TEXT, &MonsterView::on_defcombatspin_updateenter, this);
    _def_combat_spin->Bind( wxEVT_TEXT_ENTER, &MonsterView::on_defcombatspin_updateenter, this);
    mk_spin( _monster_panel, wxID_ANY, def_hbox, "Energy:", _def_nrj_spin, -10, 10, 80);
    _def_nrj_spin->Bind( wxEVT_SPINCTRL, &MonsterView::on_defnrjspin_update, this);
    _def_nrj_spin->Bind( wxEVT_TEXT, &MonsterView::on_defnrjspin_updateenter, this);
    _def_nrj_spin->Bind( wxEVT_TEXT_ENTER, &MonsterView::on_defnrjspin_updateenter, this);
    mk_spin( _monster_panel, wxID_ANY, def_hbox, "Spirit:", _def_spirit_spin, -10, 10, 80);
    _def_spirit_spin->Bind( wxEVT_SPINCTRL, &MonsterView::on_defspiritspin_update, this);
    _def_spirit_spin->Bind( wxEVT_TEXT, &MonsterView::on_defspiritspin_updateenter, this);
    _def_spirit_spin->Bind( wxEVT_TEXT_ENTER, &MonsterView::on_defspiritspin_updateenter, this);
    mk_spin( _monster_panel, wxID_ANY, def_hbox, "Weather:", _def_weather_spin, -10, 10, 80);
    _def_weather_spin->Bind( wxEVT_SPINCTRL, &MonsterView::on_defweatherspin_update, this);
    _def_weather_spin->Bind( wxEVT_TEXT, &MonsterView::on_defweatherspin_updateenter, this);
    _def_weather_spin->Bind( wxEVT_TEXT_ENTER, &MonsterView::on_defweatherspin_updateenter, this);
    mk_spin( _monster_panel, wxID_ANY, def_hbox, "Riding:", _def_riding_spin, -10, 10, 80);
    _def_riding_spin->Bind( wxEVT_SPINCTRL, &MonsterView::on_defridingspin_update, this);
    _def_riding_spin->Bind( wxEVT_TEXT, &MonsterView::on_defridingspin_updateenter, this);
    _def_riding_spin->Bind( wxEVT_TEXT_ENTER, &MonsterView::on_defridingspin_updateenter, this);
    mk_spin( _monster_panel, wxID_ANY, def_hbox, "Ranged:", _def_ranged_spin, -10, 10, 80);
    _def_ranged_spin->Bind( wxEVT_SPINCTRL, &MonsterView::on_defrangedspin_update, this);
    _def_ranged_spin->Bind( wxEVT_TEXT, &MonsterView::on_defrangedspin_updateenter, this);
    _def_ranged_spin->Bind( wxEVT_TEXT_ENTER, &MonsterView::on_defrangedspin_updateenter, this);

    monster_vbox->Add( def_hbox, 0, wxEXPAND, 0 );

    // skills
    wxBoxSizer *skill_hbox = new wxBoxSizer( wxHORIZONTAL );
    mk_spin( _monster_panel, wxID_ANY, skill_hbox, "Tactics:", _tactics_spin, 0, 10, 50);
    _tactics_spin->Bind( wxEVT_SPINCTRL, &MonsterView::on_tacticsspin_update, this);
    _tactics_spin->Bind( wxEVT_TEXT, &MonsterView::on_tacticsspin_updateenter, this);
    _tactics_spin->Bind( wxEVT_TEXT_ENTER, &MonsterView::on_tacticsspin_updateenter, this);
    mk_spin( _monster_panel, wxID_ANY, skill_hbox, "Stealth:", _stealth_spin, 0, 10, 50);
    _stealth_spin->Bind( wxEVT_SPINCTRL, &MonsterView::on_stealthspin_update, this);
    _stealth_spin->Bind( wxEVT_TEXT, &MonsterView::on_stealthspin_updateenter, this);
    _stealth_spin->Bind( wxEVT_TEXT_ENTER, &MonsterView::on_stealthspin_updateenter, this);
    mk_spin( _monster_panel, wxID_ANY, skill_hbox, "Observation:", _obs_spin, 0, 10, 50);
    _obs_spin->Bind( wxEVT_SPINCTRL, &MonsterView::on_obsspin_update, this);
    _obs_spin->Bind( wxEVT_TEXT, &MonsterView::on_obsspin_updateenter, this);
    _obs_spin->Bind( wxEVT_TEXT_ENTER, &MonsterView::on_obsspin_updateenter, this);
    monster_vbox->Add( skill_hbox, 0, wxEXPAND, 0 );

    // special
    wxBoxSizer *special_hbox = new wxBoxSizer( wxHORIZONTAL );
    mk_title( _monster_panel, special_hbox, "Special attack:" );
    _special_combo = new wxComboBox(_monster_panel, wxID_ANY, _T(""), wxDefaultPosition, wxDefaultSize,
                                0, NULL, wxTE_PROCESS_ENTER | wxTE_PROCESS_TAB);
    _special_combo->Append( wxString("NULL"));
    for( int i=1; i<NUMSPECIALS; ++i) {
        //if( SpecialDefs[i].flags & SkillType::MAGIC ) {
        std::string sLabel = std::string( SpecialDefs[i].key);
        _map_special[sLabel] = i;
        _special_combo->Append( wxString(sLabel));
        //std::cout << mLabel << "=>" << i << std::endl;
        //}
    }
    _special_combo->SetValue( wxString("NULL") );
    _special_combo->Bind( wxEVT_COMBOBOX, &MonsterView::on_special_update, this);
    special_hbox->Add( _special_combo, 0, wxEXPAND | wxRIGHT | wxALIGN_CENTER_VERTICAL, 20);
    mk_spin( _monster_panel, wxID_ANY, special_hbox, "Level:", _specialLevel_spin, 0, 10, 50);
    _specialLevel_spin->Bind( wxEVT_SPINCTRL, &MonsterView::on_specialspin_update, this);
    _specialLevel_spin->Bind( wxEVT_TEXT, &MonsterView::on_specialspin_updateenter, this);
    _specialLevel_spin->Bind( wxEVT_TEXT_ENTER, &MonsterView::on_specialspin_updateenter, this);
    monster_vbox->Add( special_hbox, 0, wxEXPAND, 0 );

    // spoil
    wxBoxSizer *spoil_hbox = new wxBoxSizer( wxHORIZONTAL );
    mk_title( _monster_panel, spoil_hbox, "Spoil : ");
    mk_spin( _monster_panel, wxID_ANY, spoil_hbox, "Silver:", _silverspoil_spin, 0, 1000000, 150);
    _silverspoil_spin->Bind( wxEVT_SPINCTRL, &MonsterView::on_silverspoilspin_update, this);
    _silverspoil_spin->Bind( wxEVT_TEXT, &MonsterView::on_silverspoilspin_updateenter, this);
    _silverspoil_spin->Bind( wxEVT_TEXT_ENTER, &MonsterView::on_silverspoilspin_updateenter, this);

    _spoil_combo = new wxComboBox(_monster_panel, wxID_ANY, _T(""), wxDefaultPosition, wxDefaultSize,
                                0, NULL, wxTE_PROCESS_ENTER | wxTE_PROCESS_TAB);
    _spoil_combo->Append( wxString("NULL"));
    for( auto& spoil : _map_itype ) {
        std::cout << "Spoil = " << spoil.first << std::endl;
        _spoil_combo->Append( wxString(spoil.first));
    }
    _spoil_combo->SetValue( wxString("NULL") );
    _spoil_combo->Bind( wxEVT_COMBOBOX, &MonsterView::on_spoilcombo_update, this);
    spoil_hbox->Add( _spoil_combo, 0, wxEXPAND | wxRIGHT | wxALIGN_CENTER_VERTICAL, 20);

//    mk_check( this, wxID_ANY, spoil_hbox, "normal", _normalspoil_check );
//    mk_check( this, wxID_ANY, spoil_hbox, "advanced", _advancedspoil_check );
//    mk_check( this, wxID_ANY, spoil_hbox, "trade", _tradespoil_check );
//    mk_check( this, wxID_ANY, spoil_hbox, "man", _manspoil_check );
//    mk_check( this, wxID_ANY, spoil_hbox, "monster", _monsterspoil_check );
//    mk_check( this, wxID_ANY, spoil_hbox, "magic", _magicspoil_check );
//    mk_check( this, wxID_ANY, spoil_hbox, "weapon", _weaponspoil_check );
//    mk_check( this, wxID_ANY, spoil_hbox, "armor", _armorspoil_check );
//    mk_check( this, wxID_ANY, spoil_hbox, "mount", _mountspoil_check );
//    mk_check( this, wxID_ANY, spoil_hbox, "battle", _battlespoil_check );
//    mk_check( this, wxID_ANY, spoil_hbox, "special", _specialspoil_check );
//    mk_check( this, wxID_ANY, spoil_hbox, "tool", _toolspoil_check );
    monster_vbox->Add( spoil_hbox, 0, wxEXPAND, 0 );

//    wxBoxSizer *spoil2_hbox = new wxBoxSizer( wxHORIZONTAL );
//    mk_title( this, spoil2_hbox, "                     ");
//    mk_check( this, wxID_ANY, spoil2_hbox, "food", _foodspoil_check );
//    mk_check( this, wxID_ANY, spoil2_hbox, "illusion", _illusionspoil_check );
//    mk_check( this, wxID_ANY, spoil2_hbox, "undead", _undeadspoil_check );
//    mk_check( this, wxID_ANY, spoil2_hbox, "demon", _demonspoil_check );
//    mk_check( this, wxID_ANY, spoil2_hbox, "leader", _leaderspoil_check );
//    mk_check( this, wxID_ANY, spoil2_hbox, "money", _moneyspoil_check );
//    mk_check( this, wxID_ANY, spoil2_hbox, "animal", _animalspoil_check );
//    mk_check( this, wxID_ANY, spoil2_hbox, "ship", _shipspoil_check );
//    mk_check( this, wxID_ANY, spoil2_hbox, "mage only", _mageonlyspoil_check );
//    mk_check( this, wxID_ANY, spoil2_hbox, "always", _alwaysspoil_check );
//    mk_check( this, wxID_ANY, spoil2_hbox, "never", _neverspoil_check );
//    main_vbox->Add( spoil2_hbox, 0, wxEXPAND, 0 );

    wxBoxSizer *enc_hbox = new wxBoxSizer( wxHORIZONTAL );
    mk_spin( _monster_panel, wxID_ANY, enc_hbox, "Hostile:", _hostile_spin, 0, 100, 150);
    _hostile_spin->Bind( wxEVT_SPINCTRL, &MonsterView::on_hostilespin_update, this);
    _hostile_spin->Bind( wxEVT_TEXT, &MonsterView::on_hostilespin_updateenter, this);
    _hostile_spin->Bind( wxEVT_TEXT_ENTER, &MonsterView::on_hostilespin_updateenter, this);
    mk_spin( _monster_panel, wxID_ANY, enc_hbox, "Number:", _number_spin, 0, 1000, 150);
    _number_spin->Bind( wxEVT_SPINCTRL, &MonsterView::on_numberspin_update, this);
    _number_spin->Bind( wxEVT_TEXT, &MonsterView::on_numberspin_updateenter, this);
    _number_spin->Bind( wxEVT_TEXT_ENTER, &MonsterView::on_numberspin_updateenter, this);
    monster_vbox->Add( enc_hbox, 0, wxEXPAND, 0 );

    // mdef name and abbr
    wxBoxSizer *mdef_hbox = new wxBoxSizer( wxHORIZONTAL );
    mk_title( _monster_panel, mdef_hbox, "Special for MonDefs ");
    mk_field( _monster_panel, wxID_ANY, mdef_hbox, "abbr:", _mdefabbr_text, 0, 80);
    _mdefabbr_text->Enable(false);
    mk_field( _monster_panel, wxID_ANY, mdef_hbox, "desc:", _mdefname_text, 1, 200);
    _mdefname_text->Bind( wxEVT_TEXT_ENTER, &MonsterView::on_mdefnametext_update, this );
    monster_vbox->Add( mdef_hbox, 0, wxEXPAND, 0 );
    _monster_panel->SetSizer( monster_vbox );

    main_vbox->Add( _monster_panel, 0, wxEXPAND, 0);

    this->SetSizer( main_vbox );

}
// ****************************************************** MonsterView::add_item
void MonsterView::add_monster( AMonster* new_monster )
{
    _abbr_combo->Append( wxString(new_monster->_item_enum) );
    _abbr_combo->SetValue( wxString(new_monster->_item_enum) );
    set_monster( new_monster );
}
// ************************************************ MonsterView::on_cbox_update
void MonsterView::on_cbox_update( wxCommandEvent& event )
{
    // Don't show messages for the log output window (it'll crash)
    if ( !event.GetEventObject()->IsKindOf(CLASSINFO(wxComboBox)) )
        return;

    if ( event.GetEventType() == wxEVT_COMBOBOX )
    {
        std::cout << "EVT_COMBOBOX(id="<< event.GetId() << ",selection=" << event.GetSelection() << std::endl;
        //std::cout << "  => " << _list_abbr[event.GetSelection()] << std::endl;
        std::cout << "  => " << _abbr_combo->GetStringSelection() << std::endl;
        set_monster( _data.find_monster( _abbr_combo->GetStringSelection().ToStdString()) );
    }
    else if ( event.GetEventType() == wxEVT_TEXT )
    {
        std::cout << "EVT_TEXT(id="<< event.GetId() << ",selection=" << event.GetSelection() << std::endl;
    }
    else if ( event.GetEventType() == wxEVT_TEXT_ENTER )
    {
        std::cout << "EVT_TEXT_ENTER(id="<< event.GetId() << ",string=" << event.GetString() << std::endl;
        //wxLogDebug("EVT_TEXT_ENTER(id=%i,string=\"%s\")",
        //            event.GetId(), event.GetString().c_str());
    }
}
void MonsterView::on_edit_update( wxCommandEvent& event )
{
    if( _monster->_fg_edited == false ) {
        int answer = wxMessageBox("Tu veux vraiment modifier ce monstre ?? ", "Editer", wxOK | wxCANCEL, this);
        if( answer == wxOK) {
            _monster->_fg_edited = true;
            _monster_panel->Enable( true );
        }
        else {
            _edit_check->SetValue( false );
        }
    }
    else {
        if( _edit_check->GetValue() == false ) {
            wxMessageBox("'tention, les modifs sont memorisees ", "Editer", wxOK, this);
            _monster_panel->Enable( false );
        }
        else {
            _monster_panel->Enable( _edit_check->GetValue() );
        }
    }
}

void MonsterView::on_abbrtext_update( wxCommandEvent& event )
{
    std::string abbr = _abbr_text->GetValue().ToStdString();
    std::transform(abbr.begin(), abbr.end(), abbr.begin(), ::toupper);
    // check only Caps and no more than 4
    if( abbr.length() > 4) {
        wxMessageBox("Plus de 4 lettres !!", "Erreur", wxOK, this);
        _abbr_text->Clear();
        _abbr_text->AppendText( wxString( _monster->_item->abr));
    }
    else {
        _monster->_item->abr = (const char*) malloc( abbr.length()+1 );
        strcpy( (char *) _monster->_item->abr, abbr.c_str() );
        _abbr_text->Clear();
        _abbr_text->AppendText( wxString( _monster->_item->abr));

        // Change also _monster->_mtype->abbr;
        _monster->_mtype->abbr = (const char*) malloc( abbr.length()+2 );
        if( _monster->_item->type & IT_ILLUSION ) {
            abbr = "i"+abbr;
            strcpy( (char *) _monster->_mtype->abbr, abbr.c_str() );
        }
        else {
            strcpy( (char *) _monster->_mtype->abbr, abbr.c_str() );
        }
        _mdefabbr_text->Clear();
        _mdefabbr_text->AppendText( wxString( _monster->_mtype->abbr));
    }
}
void MonsterView::on_nametext_update( wxCommandEvent& event )
{
    std::string name = _name_text->GetValue().ToStdString();
    _monster->_item->name = (const char*) malloc( name.length()+1 );
    strcpy( (char *) _monster->_item->name, name.c_str() );
    _name_text->Clear();
    _name_text->AppendText( wxString( _monster->_item->name));
}
void MonsterView::on_namestext_update( wxCommandEvent& event )
{
    std::string names = _names_text->GetValue().ToStdString();
    _monster->_item->names = (const char*) malloc( names.length()+1 );
    strcpy( (char *) _monster->_item->names, names.c_str() );
    _names_text->Clear();
    _names_text->AppendText( wxString( _monster->_item->names));
}
void MonsterView::on_disabled_update( wxCommandEvent& event )
{
    if( _disabled_check->GetValue() )
        _monster->_item->flags = _monster->_item->flags | ItemType::DISABLED;
    else
        _monster->_item->flags = _monster->_item->flags ^ ItemType::DISABLED;
}
void MonsterView::on_cantgive_update( wxCommandEvent& event )
{
    if( _cantgive_check->GetValue() )
        _monster->_item->flags = _monster->_item->flags | ItemType::CANTGIVE;
    else
        _monster->_item->flags = _monster->_item->flags ^ ItemType::CANTGIVE;
}
void MonsterView::on_mSkillcombo_update( wxCommandEvent& event )
{
    if ( event.GetEventType() == wxEVT_COMBOBOX )
    {
        std::cout << "EVT_COMBOBOX(mSkill), selection=" << _mSkill_combo->GetStringSelection().ToStdString() << std::endl;
        //std::cout << "  " << _mSkill_combo.GetStringSelection().ToStdString() << std::endl;
        std::string mLabel = _mSkill_combo->GetStringSelection().ToStdString();
        int id_skill = _map_mSkill[mLabel];
        //std::cout << "  => " << _list_abbr[event.GetSelection()] << std::endl;
        std::cout << "  => " << SkillDefs[id_skill].abbr << std::endl;

        _monster->_item->mSkill = (const char*) malloc( strlen(SkillDefs[id_skill].abbr)+1 );
        strcpy( (char *) _monster->_item->mSkill, SkillDefs[id_skill].abbr );
    }
}
void MonsterView::on_mLevelspin_update( wxSpinEvent& event)
{
    _monster->_item->mLevel = _mLevel_spin->GetValue();
}
void MonsterView::on_mLevelspin_updateenter( wxCommandEvent& event)
{
    _monster->_item->mLevel = _mLevel_spin->GetValue();
}
void MonsterView::on_weightspin_update( wxSpinEvent& event)
{
    _monster->_item->weight = _weight_spin->GetValue();
}
void MonsterView::on_weightspin_updateenter( wxCommandEvent& event)
{
    _monster->_item->weight = _weight_spin->GetValue();
}
void MonsterView::on_basepricespin_update( wxSpinEvent& event)
{
    _monster->_item->baseprice = _baseprice_spin->GetValue();
}
void MonsterView::on_basepricespin_updateenter( wxCommandEvent& event)
{
    _monster->_item->baseprice = _baseprice_spin->GetValue();
}
void MonsterView::on_animal_update( wxCommandEvent& event )
{
    if( _animal_check->GetValue() )
        _monster->_item->type = _monster->_item->type | IT_ANIMAL;
    else
        _monster->_item->type = _monster->_item->type ^ IT_ANIMAL;
}
void MonsterView::on_demon_update( wxCommandEvent& event )
{
    if( _demon_check->GetValue() )
        _monster->_item->type = _monster->_item->type | IT_DEMON;
    else
        _monster->_item->type = _monster->_item->type ^ IT_DEMON;
}
void MonsterView::on_illusion_update( wxCommandEvent& event )
{
    if( _illusion_check->GetValue() )
        _monster->_item->type = _monster->_item->type | IT_ILLUSION;
    else
        _monster->_item->type = _monster->_item->type ^ IT_ILLUSION;

    std::string abbr(_monster->_item->abr);
    if( _monster->_item->type & IT_ILLUSION ) {
        abbr = "i"+abbr;
        strcpy( (char *) _monster->_mtype->abbr, abbr.c_str() );
    }
    else {
        strcpy( (char *) _monster->_mtype->abbr, abbr.c_str() );
    }
    _mdefabbr_text->Clear();
    _mdefabbr_text->AppendText( wxString( _monster->_mtype->abbr));
}
void MonsterView::on_undead_update( wxCommandEvent& event )
{
    if( _undead_check->GetValue() )
        _monster->_item->type = _monster->_item->type | IT_UNDEAD;
    else
        _monster->_item->type = _monster->_item->type ^ IT_UNDEAD;
}
void MonsterView::on_walkspin_update( wxSpinEvent& event)
{
    _monster->_item->walk = _walk_spin->GetValue();
}
void MonsterView::on_walkspin_updateenter( wxCommandEvent& event)
{
    _monster->_item->walk = _walk_spin->GetValue();
}
void MonsterView::on_ridespin_update( wxSpinEvent& event)
{
    _monster->_item->ride = _ride_spin->GetValue();
}
void MonsterView::on_ridespin_updateenter( wxCommandEvent& event)
{
    _monster->_item->ride = _ride_spin->GetValue();
}
void MonsterView::on_flyspin_update( wxSpinEvent& event)
{
    _monster->_item->fly = _fly_spin->GetValue();
}
void MonsterView::on_flyspin_updateenter( wxCommandEvent& event)
{
    _monster->_item->fly = _fly_spin->GetValue();
}
void MonsterView::on_swimspin_update( wxSpinEvent& event)
{
    _monster->_item->swim = _swim_spin->GetValue();
}
void MonsterView::on_swimspin_updateenter( wxCommandEvent& event)
{
    _monster->_item->swim = _swim_spin->GetValue();
}
void MonsterView::on_speedspin_update( wxSpinEvent& event)
{
    _monster->_item->speed = _speed_spin->GetValue();
}
void MonsterView::on_speedspin_updateenter( wxCommandEvent& event)
{
    _monster->_item->speed = _speed_spin->GetValue();
}
void MonsterView::on_maxinventoryspin_update( wxSpinEvent& event)
{
    _monster->_item->max_inventory = _maxinventory_spin->GetValue();
}
void MonsterView::on_maxinventoryspin_updateenter( wxCommandEvent& event)
{
    _monster->_item->max_inventory = _maxinventory_spin->GetValue();
}
void MonsterView::on_escape_update( wxCommandEvent& event )
{
    _escape_panel->Enable( _escape_check->GetValue() );
}
void MonsterView::on_esccheck_update( wxCommandEvent& event )
{
    std::cout << "EVENT ESC id=" << event.GetId() << std::endl;
    _monster->_item->escape = 0;
    //std::cout << "STATE " << _losschance_check->GetValue() << " " << _hasskill_check->GetValue() << " " << _esclinear_check->GetValue() << std::endl;
    switch( event.GetId() ) {
    case idEscLossChance:
        if( _losschance_check->GetValue() ) {
            _esclinear_check->SetValue( false );
            _escsquare_check->SetValue( false );
            _esccube_check->SetValue( false );
            _escquad_check->SetValue( false );
            _esc_opt_panel->Enable( false );
        }
        break;
    case idEscHasSkill:
        if( _hasskill_check->GetValue() ) {
            _esclinear_check->SetValue( false );
            _escsquare_check->SetValue( false );
            _esccube_check->SetValue( false );
            _escquad_check->SetValue( false );
            _esc_opt_panel->Enable( false );
        }
        break;
    case idEscLinear:
        if( _esclinear_check->GetValue()) {
            _losschance_check->SetValue( false );
            _hasskill_check->SetValue( false );
            _escsquare_check->SetValue( false );
            _esccube_check->SetValue( false );
            _escquad_check->SetValue( false );
            _esc_opt_panel->Enable( true );
        }
        break;
    case idEscSquare:
        if( _escsquare_check->GetValue()) {
            _losschance_check->SetValue( false );
            _hasskill_check->SetValue( false );
            _esclinear_check->SetValue( false );
            _esccube_check->SetValue( false );
            _escquad_check->SetValue( false );
            _esc_opt_panel->Enable( true );
        }
        break;
    case idEscCube:
        if( _esccube_check->GetValue()) {
            _losschance_check->SetValue( false );
            _hasskill_check->SetValue( false );
            _esclinear_check->SetValue( false );
            _escsquare_check->SetValue( false );
            _escquad_check->SetValue( false );
            _esc_opt_panel->Enable( true );
        }
        break;
    case idEscQuad:
        if( _escquad_check->GetValue()) {
            _losschance_check->SetValue( false );
            _hasskill_check->SetValue( false );
            _esclinear_check->SetValue( false );
            _esccube_check->SetValue( false );
            _escsquare_check->SetValue( false );
            _esc_opt_panel->Enable( true );
        }
        break;
    }
    //std::cout << "STATE " << _losschance_check->GetValue() << " " << _hasskill_check->GetValue() << " " << _esclinear_check->GetValue() << std::endl;
//    std::cout << "Val of loselinked = " <<  _loselinked_check->GetValue() << std::endl;
    if( _loselinked_check->IsChecked() )
        _monster->_item->escape = _monster->_item->escape | ItemType::LOSE_LINKED;
//    std::cout << "LOSE_LINKED escape = " << _monster->_item->escape << std::endl;
    if( _escnum_check->IsChecked() )
        _monster->_item->escape = _monster->_item->escape | ItemType::ESC_NUM_SQUARE;
//    else
//        _monster->_item->escape = _monster->_item->escape ^ ItemType::ESC_NUM_SQUARE;
//    std::cout << "NUM_SQUARE escape = " << _monster->_item->escape << std::endl;
    if( _hasskill_check->GetValue() )
        _monster->_item->escape = _monster->_item->escape | ItemType::HAS_SKILL;
//    else
//        _monster->_item->escape = _monster->_item->escape ^ ItemType::HAS_SKILL;
//    std::cout << "HAS SKILL escape = " << _monster->_item->escape << std::endl;
    if( _esclinear_check->GetValue() )
        _monster->_item->escape = _monster->_item->escape | ItemType::ESC_LEV_LINEAR;
//    else
//        _monster->_item->escape = _monster->_item->escape ^ ItemType::ESC_LEV_LINEAR;
//    std::cout << "LINEAR escape = " << _monster->_item->escape << std::endl;
    if( _escsquare_check->GetValue() )
        _monster->_item->escape = _monster->_item->escape | ItemType::ESC_LEV_SQUARE;
//    else
//        _monster->_item->escape = _monster->_item->escape ^ ItemType::ESC_LEV_SQUARE;
//    std::cout << "SQUARE escape = " << _monster->_item->escape << std::endl;
    if( _esccube_check->GetValue() )
        _monster->_item->escape = _monster->_item->escape | ItemType::ESC_LEV_CUBE;
//    else
//        _monster->_item->escape = _monster->_item->escape ^ ItemType::ESC_LEV_CUBE;
//    std::cout << "CUBE escape = " << _monster->_item->escape << std::endl;
    if( _escquad_check->GetValue() )
        _monster->_item->escape = _monster->_item->escape | ItemType::ESC_LEV_QUAD;
//    else
//        _monster->_item->escape = _monster->_item->escape ^ ItemType::ESC_LEV_QUAD;
//    std::cout << "QUAD escape = " << _monster->_item->escape << std::endl;
    if( _losschance_check->GetValue() == true) {
//        std::cout << "LOSS TRUE escape=" << _monster->_item->escape << " " << ItemType::LOSS_CHANCE << std::endl;
        _monster->_item->escape = (_monster->_item->escape | ItemType::LOSS_CHANCE);
//        std::cout << "LOSS AFTER escape=" << _monster->_item->escape << std::endl;
    }
//    else {
//        std::cout << "LOSS FALSE escape=" << _monster->_item->escape << " " << ItemType::LOSS_CHANCE << std::endl;
//        _monster->_item->escape = (_monster->_item->escape ^ ItemType::LOSS_CHANCE);
//    std::cout << "LOSS AFTER escape=" << _monster->_item->escape << std::endl;
//    }
    std::cout << "FINAL escape = " << _monster->_item->escape << std::endl;
    _monster->_item->esc_val = _esc_spin->GetValue();
    _esc_text->SetLabel( wxString( compute_escape() ));
}
void MonsterView::on_eSkillcombo_update( wxCommandEvent& event )
{
    if ( event.GetEventType() == wxEVT_COMBOBOX )
    {
        std::cout << "EVT_COMBOBOX(eSkill), selection=" << _eSkill_combo->GetStringSelection().ToStdString() << std::endl;
        //std::cout << "  " << _mSkill_combo.GetStringSelection().ToStdString() << std::endl;
        std::string eLabel = _eSkill_combo->GetStringSelection().ToStdString();
        if( eLabel.compare( "NULL" ) == 0 ) {
            _monster->_item->esc_skill = NULL;
        }
        else {
            int id_skill = _map_eSkill[eLabel];
            //std::cout << "  => " << _list_abbr[event.GetSelection()] << std::endl;
            std::cout << "  => " << SkillDefs[id_skill].abbr << std::endl;

            _monster->_item->esc_skill = (const char*) malloc( strlen(SkillDefs[id_skill].abbr)+1 );
            strcpy( (char *) _monster->_item->esc_skill, SkillDefs[id_skill].abbr );
        }
    }
    _monster->_item->esc_val = _esc_spin->GetValue();
    _esc_text->SetLabel( wxString( compute_escape() ));
}
void MonsterView::on_escspin_update( wxSpinEvent& event)
{
    _monster->_item->esc_val = _esc_spin->GetValue();
    _esc_text->SetLabel( wxString( compute_escape() ));
}
void MonsterView::on_escspin_updateenter( wxCommandEvent& event)
{
    _monster->_item->esc_val = _esc_spin->GetValue();
    _esc_text->SetLabel( wxString( compute_escape() ));
}

void MonsterView::on_atkspin_update( wxSpinEvent& event)
{
    _monster->_mtype->attackLevel = _atk_spin->GetValue();
}
void MonsterView::on_atkspin_updateenter( wxCommandEvent& event)
{
    _monster->_mtype->attackLevel = _atk_spin->GetValue();
}
void MonsterView::on_numatkspin_update( wxSpinEvent& event)
{
    _monster->_mtype->numAttacks = _numatk_spin->GetValue();
}
void MonsterView::on_numatkspin_updateenter( wxCommandEvent& event)
{
    _monster->_mtype->numAttacks = _numatk_spin->GetValue();
}
void MonsterView::on_nbhitsspin_update( wxSpinEvent& event)
{
    _monster->_mtype->hits = _nbhits_spin->GetValue();
}
void MonsterView::on_nbhitsspin_updateenter( wxCommandEvent& event)
{
    _monster->_mtype->hits = _nbhits_spin->GetValue();
}
void MonsterView::on_regenspin_update( wxSpinEvent& event)
{
    _monster->_mtype->regen = _regen_spin->GetValue();
}
void MonsterView::on_regenspin_updateenter( wxCommandEvent& event)
{
    _monster->_mtype->regen = _regen_spin->GetValue();
}
void MonsterView::on_defcombatspin_update( wxSpinEvent& event)
{
    _monster->_mtype->defense[0] = _def_combat_spin->GetValue();
}
void MonsterView::on_defcombatspin_updateenter( wxCommandEvent& event)
{
    _monster->_mtype->defense[0] = _def_combat_spin->GetValue();
}
void MonsterView::on_defnrjspin_update( wxSpinEvent& event)
{
    _monster->_mtype->defense[1] = _def_nrj_spin->GetValue();
}
void MonsterView::on_defnrjspin_updateenter( wxCommandEvent& event)
{
    _monster->_mtype->defense[1] = _def_nrj_spin->GetValue();
}
void MonsterView::on_defspiritspin_update( wxSpinEvent& event)
{
    _monster->_mtype->defense[2] = _def_spirit_spin->GetValue();
}
void MonsterView::on_defspiritspin_updateenter( wxCommandEvent& event)
{
    _monster->_mtype->defense[2] = _def_spirit_spin->GetValue();
}
void MonsterView::on_defweatherspin_update( wxSpinEvent& event)
{
    _monster->_mtype->defense[3] = _def_weather_spin->GetValue();
}
void MonsterView::on_defweatherspin_updateenter( wxCommandEvent& event)
{
    _monster->_mtype->defense[3] = _def_weather_spin->GetValue();
}
void MonsterView::on_defridingspin_update( wxSpinEvent& event)
{
    _monster->_mtype->defense[4] = _def_riding_spin->GetValue();
}
void MonsterView::on_defridingspin_updateenter( wxCommandEvent& event)
{
    _monster->_mtype->defense[4] = _def_riding_spin->GetValue();
}
void MonsterView::on_defrangedspin_update( wxSpinEvent& event)
{
    _monster->_mtype->defense[5] = _def_ranged_spin->GetValue();
}
void MonsterView::on_defrangedspin_updateenter( wxCommandEvent& event)
{
    _monster->_mtype->defense[5] = _def_ranged_spin->GetValue();
}

void MonsterView::on_tacticsspin_update( wxSpinEvent& event)
{
    _monster->_mtype->tactics = _tactics_spin->GetValue();
}
void MonsterView::on_tacticsspin_updateenter( wxCommandEvent& event)
{
    _monster->_mtype->tactics = _tactics_spin->GetValue();
}
void MonsterView::on_stealthspin_update( wxSpinEvent& event)
{
    _monster->_mtype->stealth = _stealth_spin->GetValue();
}
void MonsterView::on_stealthspin_updateenter( wxCommandEvent& event)
{
    _monster->_mtype->stealth = _stealth_spin->GetValue();
}
void MonsterView::on_obsspin_update( wxSpinEvent& event)
{
    _monster->_mtype->obs = _obs_spin->GetValue();
}
void MonsterView::on_obsspin_updateenter( wxCommandEvent& event)
{
    _monster->_mtype->obs = _obs_spin->GetValue();
}

void MonsterView::on_special_update( wxCommandEvent& event )
{
    if ( event.GetEventType() == wxEVT_COMBOBOX )
    {
        std::cout << "EVT_COMBOBOX(special), selection=" << _special_combo->GetStringSelection().ToStdString() << std::endl;
        //std::cout << "  " << _mSkill_combo.GetStringSelection().ToStdString() << std::endl;
        std::string sLabel = _special_combo->GetStringSelection().ToStdString();
        if( sLabel.compare( "NULL" ) == 0 ) {
            _monster->_mtype->special = NULL;
        }
        else {
            int id_special = _map_special[sLabel];
            //std::cout << "  => " << _list_abbr[event.GetSelection()] << std::endl;
            std::cout << "  => " << SpecialDefs[id_special].key << std::endl;

            _monster->_mtype->special = (const char*) malloc( strlen(SpecialDefs[id_special].key)+1 );
            strcpy( (char *) _monster->_mtype->special, SpecialDefs[id_special].key );
        }
    }
}
void MonsterView::on_specialspin_update( wxSpinEvent& event)
{
    _monster->_mtype->specialLevel = _specialLevel_spin->GetValue();
}
void MonsterView::on_specialspin_updateenter( wxCommandEvent& event)
{
    _monster->_mtype->specialLevel = _specialLevel_spin->GetValue();
}

void MonsterView::on_silverspoilspin_update( wxSpinEvent& event)
{
    _monster->_mtype->silver = _silverspoil_spin->GetValue();
}
void MonsterView::on_silverspoilspin_updateenter( wxCommandEvent& event)
{
    _monster->_mtype->silver = _silverspoil_spin->GetValue();
}
void MonsterView::on_spoilcombo_update( wxCommandEvent& event )
{
    if ( event.GetEventType() == wxEVT_COMBOBOX )
    {
        std::cout << "EVT_COMBOBOX(spoil), selection=" << _spoil_combo->GetStringSelection().ToStdString() << std::endl;
        //std::cout << "  " << _mSkill_combo.GetStringSelection().ToStdString() << std::endl;
        std::string sLabel = _spoil_combo->GetStringSelection().ToStdString();
        if( sLabel.compare( "NULL" ) == 0 ) {
            _monster->_mtype->spoiltype = -1;
        }
        else {
            int id_special = _map_itype[sLabel];
            //std::cout << "  => " << _list_abbr[event.GetSelection()] << std::endl;
            //std::cout << "  => " << SpecialDefs[id_special].key << std::endl;

            _monster->_mtype->spoiltype = id_special;
        }
    }
}

void MonsterView::on_hostilespin_update( wxSpinEvent& event)
{
    _monster->_mtype->hostile = _hostile_spin->GetValue();
}
void MonsterView::on_hostilespin_updateenter( wxCommandEvent& event)
{
    _monster->_mtype->hostile = _hostile_spin->GetValue();
}
void MonsterView::on_numberspin_update( wxSpinEvent& event)
{
    _monster->_mtype->number = _number_spin->GetValue();
}
void MonsterView::on_numberspin_updateenter( wxCommandEvent& event)
{
    _monster->_mtype->number = _number_spin->GetValue();
}

void MonsterView::on_mdefnametext_update( wxCommandEvent& event )
{
    std::string name = _mdefname_text->GetValue().ToStdString();
    _monster->_mtype->name = (const char*) malloc( name.length()+1 );
    strcpy( (char *) _monster->_mtype->name, name.c_str() );
    _mdefname_text->Clear();
    _mdefname_text->AppendText( wxString( _monster->_mtype->name));
}
// ****************************************************************************
void MonsterView::set_monster( AMonster* monster )
{
    _monster = monster;
    _edit_check->SetValue( false );
    _monster_panel->Enable( false );
    // abr, name, names
    _abbr_text->Clear();
    _abbr_text->AppendText( wxString( monster->_item->abr));
    _name_text->Clear();
    _name_text->AppendText( wxString( monster->_item->name));
    _names_text->Clear();
    _names_text->AppendText( wxString( monster->_item->names));

    // flags
    _disabled_check->SetValue( monster->_item->flags & ItemType::DISABLED );
    _cantgive_check->SetValue( monster->_item->flags & ItemType::CANTGIVE );

    // Magic production
    if( monster->_item->mSkill != NULL ) {
        // Find proper Skill
        for( int i=0; i < NSKILLS; ++i ) {
            std::cout << "CMP " << SkillDefs[i].abbr << " with " << monster->_item->mSkill << std::endl;
            if( strcmp( SkillDefs[i].abbr, monster->_item->mSkill) == 0 ) {
                std::cout << "  => FOUND" << std::endl;
                std::string mLabel = std::string( SkillDefs[i].name);
                mLabel += " [" + std::string( SkillDefs[i].abbr) + "]";
                _map_mSkill[mLabel] = i;
                _mSkill_combo->SetValue( wxString(mLabel));
                break;
            }
        }
        _mLevel_spin->SetValue( monster->_item->mLevel );
    }
    else {
//        _mSkill_text->Clear();
//        _mSkill_text->AppendText( _("-") );
        _mSkill_combo->SetValue( wxString("NULL") );
        _mLevel_spin->SetValue( 0 );
    }

    // weight
    _weight_spin->SetValue( monster->_item->weight );
    _baseprice_spin->SetValue( monster->_item->baseprice );

    // type
    _animal_check->SetValue( monster->_item->type & IT_ANIMAL );
    _demon_check->SetValue( monster->_item->type & IT_DEMON );
    _illusion_check->SetValue( monster->_item->type & IT_ILLUSION );
    _undead_check->SetValue( monster->_item->type & IT_UNDEAD );

    // movements
    _walk_spin->SetValue( monster->_item->walk );
    _ride_spin->SetValue( monster->_item->ride );
    _fly_spin->SetValue( monster->_item->fly );
    _swim_spin->SetValue( monster->_item->swim );
    _speed_spin->SetValue( monster->_item->speed );

    // max inventory
    _maxinventory_spin->SetValue( monster->_item->max_inventory );

    // escape
    if( monster->_item->escape == 0 ) { // no escape
        _escape_check->SetValue( false );
        _escape_panel->Enable( false );
    }
    else { // escape
        _escape_check->SetValue( true );
        _escape_panel->Enable( true );
    }
    _loselinked_check->SetValue( monster->_item->escape & ItemType::LOSE_LINKED );
    _hasskill_check->SetValue( monster->_item->escape & ItemType::HAS_SKILL );
    _esclinear_check->SetValue( monster->_item->escape & ItemType::ESC_LEV_LINEAR );
    _escsquare_check->SetValue( monster->_item->escape & ItemType::ESC_LEV_SQUARE );
    _esccube_check->SetValue( monster->_item->escape & ItemType::ESC_LEV_CUBE );
    _escquad_check->SetValue( monster->_item->escape & ItemType::ESC_LEV_QUAD );
    _losschance_check->SetValue( monster->_item->escape & ItemType::LOSS_CHANCE );
    _escnum_check->SetValue( monster->_item->escape & ItemType::ESC_NUM_SQUARE );
    if( monster->_item->escape & ItemType::ESC_LEV_LINEAR ||
       monster->_item->escape & ItemType::ESC_LEV_SQUARE ||
       monster->_item->escape & ItemType::ESC_LEV_CUBE ||
       monster->_item->escape & ItemType::ESC_LEV_QUAD) {
        _esc_opt_panel->Enable( true );
    }

//        if( monster->_item->esc_skill != NULL ) {
//            _eSkill_text->Clear();
//            _eSkill_text->AppendText( wxString( monster->_item->esc_skill ));
//        }
//        else {
//            _eSkill_text->Clear();
//            _eSkill_text->AppendText( _("-") );
//        }
        // Magic production
    if( monster->_item->esc_skill != NULL ) {
        // Find proper Skill
        for( int i=0; i < NSKILLS; ++i ) {
            std::cout << "CMP " << SkillDefs[i].abbr << " with " << monster->_item->esc_skill << std::endl;
            if( strcmp( SkillDefs[i].abbr, monster->_item->mSkill) == 0 ) {
                std::cout << "  => FOUND" << std::endl;
                std::string eLabel = std::string( SkillDefs[i].name);
                eLabel += " [" + std::string( SkillDefs[i].abbr) + "]";
                _map_eSkill[eLabel] = i;
                _eSkill_combo->SetValue( wxString(eLabel));
                break;
            }
        }
    }
    else {
        _eSkill_combo->SetValue( wxString("NULL"));
    }
    _esc_spin->SetValue( monster->_item->esc_val );
    _esc_text->SetLabel( wxString( compute_escape() ));

    // Attack
    _atk_spin->SetValue( monster->_mtype->attackLevel );
    _numatk_spin->SetValue( monster->_mtype->numAttacks );
    _nbhits_spin->SetValue( monster->_mtype->hits );
    _regen_spin->SetValue( monster->_mtype->regen );
    // defense
    int idx_def = 0;
    _def_combat_spin->SetValue( monster->_mtype->defense[idx_def++]);
    _def_nrj_spin->SetValue( monster->_mtype->defense[idx_def++]);
    _def_spirit_spin->SetValue( monster->_mtype->defense[idx_def++]);
    _def_weather_spin->SetValue( monster->_mtype->defense[idx_def++]);
    _def_riding_spin->SetValue( monster->_mtype->defense[idx_def++]);
    _def_ranged_spin->SetValue( monster->_mtype->defense[idx_def++]);

    // skill
    _tactics_spin->SetValue( monster->_mtype->tactics );
    _stealth_spin->SetValue( monster->_mtype->stealth );
    _obs_spin->SetValue( monster->_mtype->obs );

    // special
//    if( monster->_mtype->special != NULL ) {
//        _special_text->Clear();
//        _special_text->AppendText( wxString( monster->_mtype->special ));
//    }
//    else {
//        _special_text->Clear();
//        _special_text->AppendText( _("-") );
//    }
    if( monster->_mtype->special != NULL ) {
        // Find proper Skill
        for( int i=1; i < NUMSPECIALS; ++i ) {
            std::cout << "CMP " << SpecialDefs[i].key << " with " << monster->_mtype->special << std::endl;
            if( strcmp( SpecialDefs[i].key, monster->_mtype->special) == 0 ) {
                std::cout << "  => FOUND" << std::endl;
                std::string sLabel = std::string( SpecialDefs[i].key);
                _map_special[sLabel] = i;
                _special_combo->SetValue( wxString(sLabel));
                break;
            }
        }
    }
    else {
//        _mSkill_text->Clear();
//        _mSkill_text->AppendText( _("-") );
        _special_combo->SetValue( wxString("NULL") );
    }
    _specialLevel_spin->SetValue( monster->_mtype->specialLevel );

    // spoil
    _silverspoil_spin->SetValue( monster->_mtype->silver);
    if( monster->_mtype->spoiltype == -1 ) {
        _spoil_combo->SetValue( wxString("NULL"));
    }
    else {
        for( auto& spoil : _map_itype) {
            if( spoil.second == monster->_mtype->spoiltype) {
                _spoil_combo->SetValue( wxString( spoil.first ) );
                break;
            }
        }
    }
//    _normalspoil_check->SetValue( monster->_mtype->spoiltype & IT_NORMAL );
//    _advancedspoil_check->SetValue( monster->_mtype->spoiltype & IT_ADVANCED );
//    _tradespoil_check->SetValue( monster->_mtype->spoiltype & IT_TRADE );
//    _manspoil_check->SetValue( monster->_mtype->spoiltype & IT_MAN );
//    _monsterspoil_check->SetValue( monster->_mtype->spoiltype & IT_MONSTER );
//    _magicspoil_check->SetValue( monster->_mtype->spoiltype & IT_MAGIC );
//    _weaponspoil_check->SetValue( monster->_mtype->spoiltype & IT_WEAPON );
//    _armorspoil_check->SetValue( monster->_mtype->spoiltype & IT_ARMOR );
//    _mountspoil_check->SetValue( monster->_mtype->spoiltype & IT_MOUNT );
//    _battlespoil_check->SetValue( monster->_mtype->spoiltype & IT_BATTLE );
//    _specialspoil_check->SetValue( monster->_mtype->spoiltype & IT_SPECIAL );
//    _toolspoil_check->SetValue( monster->_mtype->spoiltype & IT_TOOL );
//
//    _foodspoil_check->SetValue( monster->_mtype->spoiltype & IT_FOOD );
//    _illusionspoil_check->SetValue( monster->_mtype->spoiltype & IT_ILLUSION );
//    _undeadspoil_check->SetValue( monster->_mtype->spoiltype & IT_UNDEAD );
//    _demonspoil_check->SetValue( monster->_mtype->spoiltype & IT_DEMON );
//    _leaderspoil_check->SetValue( monster->_mtype->spoiltype & IT_LEADER );
//    _moneyspoil_check->SetValue( monster->_mtype->spoiltype & IT_MONEY );
//    _animalspoil_check->SetValue( monster->_mtype->spoiltype & IT_ANIMAL );
//    _shipspoil_check->SetValue( monster->_mtype->spoiltype & IT_SHIP );
//    _mageonlyspoil_check->SetValue( monster->_mtype->spoiltype & IT_MAGEONLY );
//    _alwaysspoil_check->SetValue( monster->_mtype->spoiltype & IT_ALWAYS_SPOIL );
//    _neverspoil_check->SetValue( monster->_mtype->spoiltype & IT_NEVER_SPOIL );

    // hostile
    _hostile_spin->SetValue( monster->_mtype->hostile );
    _number_spin->SetValue( monster->_mtype->number );

    // mdef name and abbrev
    _mdefname_text->Clear();
    _mdefname_text->AppendText( wxString( monster->_mtype->name));
    _mdefabbr_text->Clear();
    _mdefabbr_text->AppendText( wxString( monster->_mtype->abbr));
}

// ******************************************************************** mk_field
//void mk_field(wxWindow *parent, int id, wxBoxSizer *sizer, std::string title,
//              wxTextCtrl*& wxtext, int proportionnal, int width )
//{
//    wxStaticText *label = new wxStaticText( parent, wxID_ANY, _(title));
//    sizer->Add( label, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 2 );
//    wxtext = new wxTextCtrl( parent, id, "", wxDefaultPosition, wxSize(width,-1), wxTE_PROCESS_ENTER);
//    sizer->Add( wxtext, proportionnal, wxEXPAND | wxRIGHT | wxALIGN_CENTER_VERTICAL, 20);
//}
//void mk_check(wxWindow *parent, int id, wxBoxSizer *sizer, std::string title,
//              wxCheckBox*& wxcheck )
//{
//    wxcheck = new wxCheckBox( parent, id, _(title));
//    sizer->Add( wxcheck, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 20);
//}
//void mk_radio(wxWindow *parent, int id, wxBoxSizer *sizer, std::string title,
//              wxRadioButton*& wxradio )
//{
//    wxradio = new wxRadioButton( parent, id, _(title));
//    sizer->Add( wxradio, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 20);
//}
//void mk_title(wxWindow *parent, wxBoxSizer *sizer, std::string title)
//{
//    wxStaticText *label = new wxStaticText( parent, -1, _(title));
//    sizer->Add( label, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 2 );
//}
//void mk_spin(wxWindow *parent, int id, wxBoxSizer* sizer, std::string title,
//             wxSpinCtrl*& wxspin, int rmin, int rmax, int width)
//{
//    wxStaticText *label = new wxStaticText( parent, -1, _(title));
//    sizer->Add( label, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 2);
//    wxspin = new wxSpinCtrl( parent, id, "", wxDefaultPosition, wxSize(width,-1), wxTE_PROCESS_ENTER);
//    wxspin->SetRange(rmin,rmax);
//    wxspin->SetValue( 0 );
//    sizer->Add( wxspin, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 20);
//}

// ******************************************************************** mk_field
// ************************************************************** compute_escape
std::string MonsterView::compute_escape()
{
    std::stringstream msg;
    msg << "EFFET : ";
    if( !_monster) {
        msg << " - ";
        return msg.str();
    }
    if( _monster->_item->escape == 0) {
        msg << "Tous les monstres restent";
        return msg.str();
    }
    else {
        std::cout << "EFFET escape=" << _monster->_item->escape << std::endl;
        if( _monster->_item->escape & ItemType::LOSS_CHANCE) {
            if( _monster->_item->esc_val == 0) {
                msg << "ATTENTION, il faut Val > 0 !!! ";
                return msg.str();
            }
            if( _monster->_item->escape & ItemType::HAS_SKILL ) {
                msg << "Si lvl " << _monster->_item->esc_skill << " < " << _monster->_item->esc_val << " ";
            }
            int nb_max = (_monster->_mtype->number + _monster->_item->esc_val ) / _monster->_item->esc_val;
            msg << "entre 0 et " << nb_max << " disparaissent";
            return msg.str();
        }
        else if( _monster->_item->escape & ItemType::HAS_SKILL ) {
            msg << "Si lvl " << _monster->_item->esc_skill << " < " << _monster->_item->esc_val;
            msg << ", le(s) monstre(s) disparait/devient marauder";
            return msg.str();
        }
        else {
            if( !_monster->_item->esc_skill ) {
                msg << "ATTENTION, il faut un skill pour ce mode d'escape";
                return msg.str();
            }
            int top = _monster->_mtype->number;
            if( _monster->_item->escape & ItemType::ESC_NUM_SQUARE ) {
                top = top * top;
            }
            int chance[] = {10000, top, top, top, top, top};

            for( int lvl=1; lvl<6; ++lvl) {
                int bottom = 0;
                if (_monster->_item->escape & ItemType::ESC_LEV_LINEAR)
                    bottom = lvl;
                else if (_monster->_item->escape & ItemType::ESC_LEV_SQUARE)
                    bottom = lvl * lvl;
                else if (_monster->_item->escape & ItemType::ESC_LEV_CUBE)
                    bottom = lvl * lvl * lvl;
                else if (_monster->_item->escape & ItemType::ESC_LEV_QUAD)
                    bottom = lvl * lvl * lvl * lvl;
                else
                    bottom = 1;
                bottom = bottom * _monster->_item->esc_val;
                chance[lvl] = ( chance[lvl] * 10000) / bottom;
            }

            msg << "lvl(" << _monster->_item->esc_skill << ")=proba echapper : ";
            for( int lvl=0; lvl<6; ++lvl ) {
                float proba = (float) chance[lvl] / 100.0f;
                if( proba > 100.f ) proba = 100.f;
                msg << "  " << lvl << "=" << std::setprecision(3) << proba << "%";
            }
            return msg.str();
        }
    }
    msg << " inconnu... Bizarre !";
    return msg.str();
}
