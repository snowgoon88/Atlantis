#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "MonsterView.h"
#include "items.h"


wxBEGIN_EVENT_TABLE(MonsterView, wxPanel)
    EVT_COMBOBOX(idAbbrCombo, MonsterView::on_cbox_update)
    EVT_TEXT(idAbbrCombo, MonsterView::on_cbox_update)
    EVT_TEXT_ENTER(idAbbrCombo, MonsterView::on_cbox_update)
wxEND_EVENT_TABLE()

MonsterView::MonsterView(wxWindow *parent)
    : wxPanel( parent )
{
    // read file
    parse_gamedata();

    // main Vertical
    wxBoxSizer *main_vbox = new wxBoxSizer( wxVERTICAL );

    // Identification
    wxBoxSizer *id_hbox = new wxBoxSizer( wxHORIZONTAL );
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
    std::map<std::string,int>::iterator it;
    for (it=_map_abbr.begin(); it!=_map_abbr.end(); ++it) {
        //_list_abbr.push_back( wxString( it->first));
        _abbr_combo->Append( wxString( it->first));
    }
    id_hbox->Add( _abbr_combo, 0, wxEXPAND | wxRIGHT | wxALIGN_CENTER_VERTICAL, 20);
    mk_field( this, wxID_ANY, id_hbox, "abbr:", _abbr_text, 0, 80);
    mk_field( this, wxID_ANY, id_hbox, "name:", _name_text, 1, 200);
    mk_field( this, wxID_ANY, id_hbox, "names:", _names_text, 1, 200);
    main_vbox->Add( id_hbox, 0, wxEXPAND, 0 );

    // flags
    wxBoxSizer *flags_hbox = new wxBoxSizer( wxHORIZONTAL );
    mk_title( this, flags_hbox, "Flags: ");
    mk_check( this, wxID_ANY, flags_hbox, "Disabled", _disabled_check );
    mk_check( this, wxID_ANY, flags_hbox, "Can't give", _cantgive_check );
    main_vbox->Add( flags_hbox, 0, wxEXPAND, 0 );

    // magical skill
    wxBoxSizer *magic_hbox = new wxBoxSizer( wxHORIZONTAL );
    mk_title( this, magic_hbox, "Magical Production: ");
    mk_field( this, wxID_ANY, magic_hbox, "Skill:", _mSkill_text, 0, 80);
    mk_spin( this, wxID_ANY, magic_hbox, "Level:", _mLevel_spin, 0, 10, 50);
    mk_title( this, magic_hbox, "Materials: TODO");
    main_vbox->Add( magic_hbox, 0, wxEXPAND, 0 );

    // weight, baseprice
    wxBoxSizer *weight_hbox = new wxBoxSizer( wxHORIZONTAL );
    mk_spin( this, wxID_ANY, weight_hbox, "Weight:", _weight_spin, 0, 1000, 200);
    mk_spin( this, wxID_ANY, weight_hbox, "Baseprice:", _baseprice_spin, 0, 1000, 200);
    main_vbox->Add( weight_hbox, 0, wxEXPAND, 0 );

    // type
    wxBoxSizer *type_hbox = new wxBoxSizer( wxHORIZONTAL );
    mk_title( this, type_hbox, "Type:");
    mk_check( this, wxID_ANY, type_hbox, "Animal", _animal_check );
    mk_check( this, wxID_ANY, type_hbox, "Demon", _demon_check );
    mk_check( this, wxID_ANY, type_hbox, "Illusion", _illusion_check );
    mk_check( this, wxID_ANY, type_hbox, "Undead", _undead_check );
    main_vbox->Add( type_hbox, 0, wxEXPAND, 0 );

    // Movements
    wxBoxSizer *move_hbox = new wxBoxSizer( wxHORIZONTAL );
    mk_title( this, move_hbox, "Movements: ");
    mk_spin( this, wxID_ANY, move_hbox, "Walk", _walk_spin, 0, 1000, 100);
    mk_spin( this, wxID_ANY, move_hbox, "Ride", _ride_spin, 0, 1000, 100);
    mk_spin( this, wxID_ANY, move_hbox, "Fly", _fly_spin, 0, 1000, 100);
    mk_spin( this, wxID_ANY, move_hbox, "Swim", _swim_spin, 0, 1000, 100);
    mk_spin( this, wxID_ANY, move_hbox, "Speed", _speed_spin, 0, 20, 80);
    main_vbox->Add( move_hbox, 0, wxEXPAND, 0 );

    // Max Inventory
    wxBoxSizer *inventory_hbox = new wxBoxSizer( wxHORIZONTAL );
    mk_spin( this, wxID_ANY, inventory_hbox, "Max Inventory:", _maxinventory_spin, 0, 1000, 150);
    main_vbox->Add( inventory_hbox, 0, wxEXPAND, 0 );

    // Escape
    wxBoxSizer *esc_hbox = new wxBoxSizer( wxHORIZONTAL );
    mk_title( this, esc_hbox, "Escape:");
    mk_check( this, wxID_ANY, esc_hbox, "Lose Linked", _loselinked_check );
    mk_check( this, wxID_ANY, esc_hbox, "Has skill", _hasskill_check );
    mk_check( this, wxID_ANY, esc_hbox, "Linear", _esclinear_check );
    mk_check( this, wxID_ANY, esc_hbox, "Square", _escsquare_check );
    mk_check( this, wxID_ANY, esc_hbox, "Cube", _esccube_check );
    mk_check( this, wxID_ANY, esc_hbox, "Quad", _escquad_check );
    mk_check( this, wxID_ANY, esc_hbox, "Loss chance", _losschance_check );
    mk_check( this, wxID_ANY, esc_hbox, "Esc num square", _escnum_check );
    mk_field( this, wxID_ANY, esc_hbox, "Skill:", _eSkill_text, 0, 80);
    mk_spin( this, wxID_ANY, esc_hbox, "Level:", _esc_spin, 0, 10, 50);
    main_vbox->Add( esc_hbox, 0, wxEXPAND, 0 );

    // Attack
    wxBoxSizer *atk_hbox = new wxBoxSizer( wxHORIZONTAL );
    mk_spin( this, wxID_ANY, atk_hbox, "Attack Level:", _atk_spin, -10, 10, 80);
    mk_spin( this, wxID_ANY, atk_hbox, "Num Attacks:", _numatk_spin, 0, 10, 50);
    mk_spin( this, wxID_ANY, atk_hbox, "Nb Hits:", _nbhits_spin, 0, 10, 200);
    mk_spin( this, wxID_ANY, atk_hbox, "Regen:", _regen_spin, 0, 10, 200);
    main_vbox->Add( atk_hbox, 0, wxEXPAND, 0 );

    // Defense
    wxBoxSizer *def_hbox = new wxBoxSizer( wxHORIZONTAL );
    mk_title( this, def_hbox, "Defense: ");
    mk_spin( this, wxID_ANY, def_hbox, "Combat:", _def_combat_spin, -10, 10, 80);
    mk_spin( this, wxID_ANY, def_hbox, "Energy:", _def_nrj_spin, -10, 10, 80);
    mk_spin( this, wxID_ANY, def_hbox, "Spirit:", _def_spirit_spin, -10, 10, 80);
    mk_spin( this, wxID_ANY, def_hbox, "Weather:", _def_weather_spin, -10, 10, 80);
    mk_spin( this, wxID_ANY, def_hbox, "Riding:", _def_riding_spin, -10, 10, 80);
    mk_spin( this, wxID_ANY, def_hbox, "Ranged:", _def_ranged_spin, -10, 10, 80);

    main_vbox->Add( def_hbox, 0, wxEXPAND, 0 );

    // skills
    wxBoxSizer *skill_hbox = new wxBoxSizer( wxHORIZONTAL );
    mk_spin( this, wxID_ANY, skill_hbox, "Tactics:", _tactics_spin, 0, 10, 50);
    mk_spin( this, wxID_ANY, skill_hbox, "Stealth:", _stealth_spin, 0, 10, 50);
    mk_spin( this, wxID_ANY, skill_hbox, "Observation:", _obs_spin, 0, 10, 50);
    main_vbox->Add( skill_hbox, 0, wxEXPAND, 0 );

    // special
    wxBoxSizer *special_hbox = new wxBoxSizer( wxHORIZONTAL );
    mk_field( this, wxID_ANY, special_hbox, "Special:", _special_text, 0, 200);
    mk_spin( this, wxID_ANY, special_hbox, "Level:", _specialLevel_spin, 0, 10, 50);
    main_vbox->Add( special_hbox, 0, wxEXPAND, 0 );

    // spoil
    wxBoxSizer *spoil_hbox = new wxBoxSizer( wxHORIZONTAL );
    mk_title( this, spoil_hbox, "Spoil : ");
    mk_spin( this, wxID_ANY, spoil_hbox, "Silver:", _silverspoil_spin, 0, 1000, 150);
    mk_check( this, wxID_ANY, spoil_hbox, "normal", _normalspoil_check );
    mk_check( this, wxID_ANY, spoil_hbox, "advanced", _advancedspoil_check );
    mk_check( this, wxID_ANY, spoil_hbox, "trade", _tradespoil_check );
    mk_check( this, wxID_ANY, spoil_hbox, "man", _manspoil_check );
    mk_check( this, wxID_ANY, spoil_hbox, "monster", _monsterspoil_check );
    mk_check( this, wxID_ANY, spoil_hbox, "magic", _magicspoil_check );
    mk_check( this, wxID_ANY, spoil_hbox, "weapon", _weaponspoil_check );
    mk_check( this, wxID_ANY, spoil_hbox, "armor", _armorspoil_check );
    mk_check( this, wxID_ANY, spoil_hbox, "mount", _mountspoil_check );
    mk_check( this, wxID_ANY, spoil_hbox, "battle", _battlespoil_check );
    mk_check( this, wxID_ANY, spoil_hbox, "special", _specialspoil_check );
    mk_check( this, wxID_ANY, spoil_hbox, "tool", _toolspoil_check );
    main_vbox->Add( spoil_hbox, 0, wxEXPAND, 0 );

    wxBoxSizer *spoil2_hbox = new wxBoxSizer( wxHORIZONTAL );
    mk_title( this, spoil2_hbox, "                     ");
    mk_check( this, wxID_ANY, spoil2_hbox, "food", _foodspoil_check );
    mk_check( this, wxID_ANY, spoil2_hbox, "illusion", _illusionspoil_check );
    mk_check( this, wxID_ANY, spoil2_hbox, "undead", _undeadspoil_check );
    mk_check( this, wxID_ANY, spoil2_hbox, "demon", _demonspoil_check );
    mk_check( this, wxID_ANY, spoil2_hbox, "leader", _leaderspoil_check );
    mk_check( this, wxID_ANY, spoil2_hbox, "money", _moneyspoil_check );
    mk_check( this, wxID_ANY, spoil2_hbox, "animal", _animalspoil_check );
    mk_check( this, wxID_ANY, spoil2_hbox, "ship", _shipspoil_check );
    mk_check( this, wxID_ANY, spoil2_hbox, "mage only", _mageonlyspoil_check );
    mk_check( this, wxID_ANY, spoil2_hbox, "always", _alwaysspoil_check );
    mk_check( this, wxID_ANY, spoil2_hbox, "never", _neverspoil_check );
    main_vbox->Add( spoil2_hbox, 0, wxEXPAND, 0 );

    wxBoxSizer *enc_hbox = new wxBoxSizer( wxHORIZONTAL );
    mk_spin( this, wxID_ANY, enc_hbox, "Hostile:", _hostile_spin, 0, 100, 150);
    mk_spin( this, wxID_ANY, enc_hbox, "Number:", _number_spin, 0, 1000, 150);
    main_vbox->Add( enc_hbox, 0, wxEXPAND, 0 );

    // mdef name and abbr
    wxBoxSizer *mdef_hbox = new wxBoxSizer( wxHORIZONTAL );
    mk_title( this, mdef_hbox, "Special for MonDefs ");
    mk_field( this, wxID_ANY, mdef_hbox, "abbr:", _mdefabbr_text, 0, 80);
    mk_field( this, wxID_ANY, mdef_hbox, "name:", _mdefname_text, 1, 200);
    main_vbox->Add( mdef_hbox, 0, wxEXPAND, 0 );


    this->SetSizer( main_vbox );

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
        set_monster( _map_abbr.find( _abbr_combo->GetStringSelection().ToStdString())->second );
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
// ****************************************************************************
// *************************************************** MonsterView::set_monster
void MonsterView::set_monster( int id )
{
    // Check valid
    if( id < I_LEADERS || id >= NITEMS ) {
        std::cout << "MonsterView::set_monster: " << id << " is not a valid id" << std::endl;
        return;
    }
    ItemType *monster = &(ItemDefs[id]);
    int is_illusion = monster->type & IT_ILLUSION;
    MonType *mdef = find_monsterdef( monster->abr, is_illusion );


    // abr, name, names
    _abbr_text->Clear();
    _abbr_text->AppendText( wxString( monster->abr));
    _name_text->Clear();
    _name_text->AppendText( wxString( monster->name));
    _names_text->Clear();
    _names_text->AppendText( wxString( monster->names));

    // flags
    _disabled_check->SetValue( monster->flags & ItemType::DISABLED );
    _cantgive_check->SetValue( monster->flags & ItemType::CANTGIVE );

    // Magic production
    if( monster->mSkill != NULL ) {
        _mSkill_text->Clear();
        _mSkill_text->AppendText( wxString( monster->mSkill ));
        _mLevel_spin->SetValue( monster->mLevel );
    }
    else {
        _mSkill_text->Clear();
        _mSkill_text->AppendText( _("-") );
    _mLevel_spin->SetValue( 0 );
    }

    // weight
    _weight_spin->SetValue( monster->weight );
    _baseprice_spin->SetValue( monster->baseprice );

    // type
    _animal_check->SetValue( monster->type & IT_ANIMAL );
    _demon_check->SetValue( monster->type & IT_DEMON );
    _illusion_check->SetValue( monster->type & IT_ILLUSION );
    _undead_check->SetValue( monster->type & IT_UNDEAD );

    // movements
    _walk_spin->SetValue( monster->walk );
    _ride_spin->SetValue( monster->ride );
    _fly_spin->SetValue( monster->fly );
    _swim_spin->SetValue( monster->swim );
    _speed_spin->SetValue( monster->speed );

    // max inventory
    _maxinventory_spin->SetValue( monster->max_inventory );

    // escape
    _loselinked_check->SetValue( monster->escape & ItemType::LOSE_LINKED );
    _hasskill_check->SetValue( monster->escape & ItemType::HAS_SKILL );
    _esclinear_check->SetValue( monster->escape & ItemType::ESC_LEV_LINEAR );
    _escsquare_check->SetValue( monster->escape & ItemType::ESC_LEV_SQUARE );
    _esccube_check->SetValue( monster->escape & ItemType::ESC_LEV_CUBE );
    _escquad_check->SetValue( monster->escape & ItemType::ESC_LEV_QUAD );
    _losschance_check->SetValue( monster->escape & ItemType::LOSS_CHANCE );
    _escnum_check->SetValue( monster->escape & ItemType::ESC_NUM_SQUARE );
    if( monster->esc_skill != NULL ) {
        _eSkill_text->Clear();
        _eSkill_text->AppendText( wxString( monster->esc_skill ));
    }
    else {
        _eSkill_text->Clear();
        _eSkill_text->AppendText( _("-") );
    }
    _esc_spin->SetValue( monster->esc_val );

    // Attack
    _atk_spin->SetValue( mdef->attackLevel );
    _numatk_spin->SetValue( mdef->numAttacks );
    _nbhits_spin->SetValue( mdef->hits );
    _regen_spin->SetValue( mdef->regen );
    // defense
    int idx_def = 0;
    _def_combat_spin->SetValue( mdef->defense[idx_def++]);
    _def_nrj_spin->SetValue( mdef->defense[idx_def++]);
    _def_spirit_spin->SetValue( mdef->defense[idx_def++]);
    _def_weather_spin->SetValue( mdef->defense[idx_def++]);
    _def_riding_spin->SetValue( mdef->defense[idx_def++]);
    _def_ranged_spin->SetValue( mdef->defense[idx_def++]);

    // skill
    _tactics_spin->SetValue( mdef->tactics );
    _stealth_spin->SetValue( mdef->stealth );
    _obs_spin->SetValue( mdef->obs );

    // special
    if( mdef->special != NULL ) {
        _special_text->Clear();
        _special_text->AppendText( wxString( mdef->special ));
    }
    else {
        _special_text->Clear();
        _special_text->AppendText( _("-") );
    }
    _specialLevel_spin->SetValue( mdef->specialLevel );

    // spoil
    _silverspoil_spin->SetValue( mdef->silver);
    _normalspoil_check->SetValue( mdef->spoiltype & IT_NORMAL );
    _advancedspoil_check->SetValue( mdef->spoiltype & IT_ADVANCED );
    _tradespoil_check->SetValue( mdef->spoiltype & IT_TRADE );
    _manspoil_check->SetValue( mdef->spoiltype & IT_MAN );
    _monsterspoil_check->SetValue( mdef->spoiltype & IT_MONSTER );
    _magicspoil_check->SetValue( mdef->spoiltype & IT_MAGIC );
    _weaponspoil_check->SetValue( mdef->spoiltype & IT_WEAPON );
    _armorspoil_check->SetValue( mdef->spoiltype & IT_ARMOR );
    _mountspoil_check->SetValue( mdef->spoiltype & IT_MOUNT );
    _battlespoil_check->SetValue( mdef->spoiltype & IT_BATTLE );
    _specialspoil_check->SetValue( mdef->spoiltype & IT_SPECIAL );
    _toolspoil_check->SetValue( mdef->spoiltype & IT_TOOL );

    _foodspoil_check->SetValue( mdef->spoiltype & IT_FOOD );
    _illusionspoil_check->SetValue( mdef->spoiltype & IT_ILLUSION );
    _undeadspoil_check->SetValue( mdef->spoiltype & IT_UNDEAD );
    _demonspoil_check->SetValue( mdef->spoiltype & IT_DEMON );
    _leaderspoil_check->SetValue( mdef->spoiltype & IT_LEADER );
    _moneyspoil_check->SetValue( mdef->spoiltype & IT_MONEY );
    _animalspoil_check->SetValue( mdef->spoiltype & IT_ANIMAL );
    _shipspoil_check->SetValue( mdef->spoiltype & IT_SHIP );
    _mageonlyspoil_check->SetValue( mdef->spoiltype & IT_MAGEONLY );
    _alwaysspoil_check->SetValue( mdef->spoiltype & IT_ALWAYS_SPOIL );
    _neverspoil_check->SetValue( mdef->spoiltype & IT_NEVER_SPOIL );

    // hostile
    _hostile_spin->SetValue( mdef->hostile );
    _number_spin->SetValue( mdef->number );

    // mdef name and abbrev
    _mdefname_text->Clear();
    _mdefname_text->AppendText( wxString( mdef->name));
    _mdefabbr_text->Clear();
    _mdefabbr_text->AppendText( wxString( mdef->abbr));
}
// ************************************************ MonsterView::parse_gamedata
void MonsterView::parse_gamedata()
{
    std::ifstream in( "mygamedata.h" );
    std::string line;
    int idx_monster = 0;
    while( std::getline(in, line )) {
        std::cout << "[" << line[0] << "] " << line << std::endl;
        if( line[0] == '\t' && line[1] == 'I' && line[2] == '_' ) {
            if( ItemDefs[idx_monster].type & IT_MONSTER ) {
                //std::cout << "+ found monster " << line.substr( 1, line.size()-2 ) << ":" << ItemDefs[idx_monster].abr << std::endl;
                _map_abbr[line.substr( 1, line.size()-2 )] = idx_monster;
            }
            idx_monster ++;
        }
    }
    std::map<std::string,int>::iterator it;
    for (it=_map_abbr.begin(); it!=_map_abbr.end(); ++it)
        std::cout << it->first << " => " << it->second << '\n';
}
// ************************************************ MonsterView::find_monsterdef
MonType *MonsterView::find_monsterdef(char const *abbr, int illusion)
{
	if (abbr == NULL) return NULL;
	std::string tag = (illusion ? "i" : "");
	tag += abbr;

	for (int i = 0; i < NUMMONSTERS; i++) {
		if (MonDefs[i].abbr == NULL) continue;
		if (tag == MonDefs[i].abbr)
			return &MonDefs[i];
	}
	return NULL;
}
// ******************************************************************** mk_field
void mk_field(wxWindow *parent, int id, wxBoxSizer *sizer, std::string title,
              wxTextCtrl*& wxtext, int proportionnal, int width )
{
    wxStaticText *label = new wxStaticText( parent, wxID_ANY, _(title));
    sizer->Add( label, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 2 );
    wxtext = new wxTextCtrl( parent, id, "", wxDefaultPosition, wxSize(width,-1));
    sizer->Add( wxtext, proportionnal, wxEXPAND | wxRIGHT | wxALIGN_CENTER_VERTICAL, 20);
}
void mk_check(wxWindow *parent, int id, wxBoxSizer *sizer, std::string title,
              wxCheckBox*& wxcheck )
{
    wxcheck = new wxCheckBox( parent, id, _(title));
    sizer->Add( wxcheck, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 20);
}
void mk_title(wxWindow *parent, wxBoxSizer *sizer, std::string title)
{
    wxStaticText *label = new wxStaticText( parent, -1, _(title));
    sizer->Add( label, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 2 );
}
void mk_spin(wxWindow *parent, int id, wxBoxSizer* sizer, std::string title,
             wxSpinCtrl*& wxspin, int rmin, int rmax, int width)
{
    wxStaticText *label = new wxStaticText( parent, -1, _(title));
    sizer->Add( label, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 2);
    wxspin = new wxSpinCtrl( parent, id, "", wxDefaultPosition, wxSize(width,-1));
    wxspin->SetRange(rmin,rmax);
    wxspin->SetValue( 0 );
    sizer->Add( wxspin, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 20);
}
