#ifndef MONSTERVIEW_H_INCLUDED
#define MONSTERVIEW_H_INCLUDED

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <wx/spinctrl.h>

#include "gamedata.h"
#include "items.h"
#include <vector>
#include <fstream>
#include <string>
#include <map>
#include <monster_data.h>

/**
 * View/edit a Monster.
 */
 class MonsterView : public wxPanel
 {
    public:
        MonsterView( wxWindow *parent, MonsterData& data );

        /** Parse files */
        //void parse_gamedata();
        /** set to an existing Monster */
        //void set_monster( int num );
        void set_monster( AMonster* monster );
        void add_monster( AMonster* new_monster );

        MonsterData& _data;
        AMonster* _monster;
        std::map<std::string,int> _map_abbr;
        std::map<std::string,int> _map_mSkill;
        std::map<std::string,int> _map_eSkill;
        std::map<std::string,int> _map_special;
        //std::map<std::string,int> _map_spoil;
//        std::vector<wxString> _list_abbr;
        // abr, name and names
        wxComboBox *_abbr_combo;
        wxCheckBox *_edit_check;

        wxPanel* _monster_panel;


        wxTextCtrl *_abbr_text;
        wxTextCtrl *_name_text;
        wxTextCtrl *_names_text;
        // flags
        wxCheckBox *_disabled_check;
        wxCheckBox *_cantgive_check;
        // magic production
        wxComboBox *_mSkill_combo;
        //wxTextCtrl *_mSkill_text;
        wxSpinCtrl *_mLevel_spin;
        // weight, baseprice
        wxSpinCtrl *_weight_spin, *_baseprice_spin;
        // type
        wxCheckBox *_animal_check, *_demon_check, *_illusion_check, *_undead_check;
        // movements
        wxSpinCtrl *_walk_spin, *_ride_spin, *_fly_spin, *_swim_spin, *_speed_spin;
        // max inventory
        wxSpinCtrl *_maxinventory_spin;
        // escape
        wxCheckBox *_escape_check;
        wxPanel* _escape_panel;
        wxCheckBox *_loselinked_check, *_hasskill_check, *_losschance_check,
                *_esclinear_check, *_escsquare_check, *_esccube_check, *_escquad_check;
//        wxRadioButton  *_esclinear_radio,
//            *_escsquare_radio, *_esccube_radio, *_escquad_radio;
        wxCheckBox *_escnum_check;
        wxComboBox *_eSkill_combo;
        wxSpinCtrl *_esc_spin;
        wxStaticText *_esc_text;
        // attack
        wxSpinCtrl *_atk_spin, *_numatk_spin, *_nbhits_spin, *_regen_spin;
        // defense
        wxSpinCtrl *_def_combat_spin, *_def_nrj_spin, *_def_spirit_spin,
            *_def_weather_spin, *_def_riding_spin, *_def_ranged_spin;
        // skills
        wxSpinCtrl *_tactics_spin, *_stealth_spin, *_obs_spin;
        // special
        //wxTextCtrl *_special_text;
        wxComboBox *_special_combo;
        wxSpinCtrl *_specialLevel_spin;
        // spoil
        wxSpinCtrl *_silverspoil_spin;
        wxComboBox *_spoil_combo;
//        wxCheckBox *_normalspoil_check, *_advancedspoil_check, *_tradespoil_check, *_manspoil_check,
//            *_monsterspoil_check, *_magicspoil_check, *_weaponspoil_check, *_armorspoil_check,
//            *_mountspoil_check, *_battlespoil_check, *_specialspoil_check, *_toolspoil_check,
//            *_foodspoil_check, *_illusionspoil_check, *_undeadspoil_check, *_demonspoil_check,
//            *_leaderspoil_check, *_moneyspoil_check, *_animalspoil_check, *_shipspoil_check,
//            *_mageonlyspoil_check, *_alwaysspoil_check, *_neverspoil_check;
        // hostile, number
        wxSpinCtrl *_hostile_spin, *_number_spin;
        // mdef abr and name
        wxTextCtrl *_mdefname_text, *_mdefabbr_text;

        // Callbacks
        void on_cbox_update( wxCommandEvent& event );
        void on_edit_update( wxCommandEvent& event );
        void on_abbrtext_update( wxCommandEvent& event );
        void on_nametext_update( wxCommandEvent& event );
        void on_namestext_update( wxCommandEvent& event );
        void on_disabled_update( wxCommandEvent& event );
        void on_cantgive_update( wxCommandEvent& event );
        void on_mSkillcombo_update( wxCommandEvent& event);
        void on_mLevelspin_update(wxSpinEvent& event);
        void on_mLevelspin_updateenter(wxCommandEvent& event);
        void on_weightspin_update( wxSpinEvent& event);
        void on_weightspin_updateenter( wxCommandEvent& event);
        void on_basepricespin_update( wxSpinEvent& event);
        void on_basepricespin_updateenter( wxCommandEvent& event);
        void on_animal_update( wxCommandEvent& event );
        void on_demon_update( wxCommandEvent& event );
        void on_illusion_update( wxCommandEvent& event );
        void on_undead_update( wxCommandEvent& event );
        void on_walkspin_update( wxSpinEvent& event);
        void on_walkspin_updateenter( wxCommandEvent& event);
        void on_ridespin_update( wxSpinEvent& event);
        void on_ridespin_updateenter( wxCommandEvent& event);
        void on_flyspin_update( wxSpinEvent& event);
        void on_flyspin_updateenter( wxCommandEvent& event);
        void on_swimspin_update( wxSpinEvent& event);
        void on_swimspin_updateenter( wxCommandEvent& event);
        void on_speedspin_update( wxSpinEvent& event);
        void on_speedspin_updateenter( wxCommandEvent& event);
        void on_maxinventoryspin_update( wxSpinEvent& event);
        void on_maxinventoryspin_updateenter( wxCommandEvent& event);
        void on_escape_update( wxCommandEvent& event );
        void on_esccheck_update( wxCommandEvent& event );
//        void on_hasskill_update( wxCommandEvent& event );
//        void on_esclinear_update( wxCommandEvent& event );
//        void on_escsquare_update( wxCommandEvent& event );
//        void on_esccube_update( wxCommandEvent& event );
//        void on_escquad_update( wxCommandEvent& event );
//        void on_losschance_update( wxCommandEvent& event );
//        void on_escnum_update( wxCommandEvent& event );
        //void on_esctext_update( wxCommandEvent& event );
        void on_eSkillcombo_update( wxCommandEvent& event );
        void on_escspin_update( wxSpinEvent& event);
        void on_escspin_updateenter( wxCommandEvent& event);

        void on_atkspin_update( wxSpinEvent& event);
        void on_atkspin_updateenter( wxCommandEvent& event);
        void on_numatkspin_update( wxSpinEvent& event);
        void on_numatkspin_updateenter( wxCommandEvent& event);
        void on_nbhitsspin_update( wxSpinEvent& event);
        void on_nbhitsspin_updateenter( wxCommandEvent& event);
        void on_regenspin_update( wxSpinEvent& event);
        void on_regenspin_updateenter( wxCommandEvent& event);

        void on_defcombatspin_update( wxSpinEvent& event);
        void on_defcombatspin_updateenter( wxCommandEvent& event);
        void on_defnrjspin_update( wxSpinEvent& event);
        void on_defnrjspin_updateenter( wxCommandEvent& event);
        void on_defspiritspin_update( wxSpinEvent& event);
        void on_defspiritspin_updateenter( wxCommandEvent& event);
        void on_defweatherspin_update( wxSpinEvent& event);
        void on_defweatherspin_updateenter( wxCommandEvent& event);
        void on_defridingspin_update( wxSpinEvent& event);
        void on_defridingspin_updateenter( wxCommandEvent& event);
        void on_defrangedspin_update( wxSpinEvent& event);
        void on_defrangedspin_updateenter( wxCommandEvent& event);

        void on_tacticsspin_update( wxSpinEvent& event);
        void on_tacticsspin_updateenter( wxCommandEvent& event);
        void on_stealthspin_update( wxSpinEvent& event);
        void on_stealthspin_updateenter( wxCommandEvent& event);
        void on_obsspin_update( wxSpinEvent& event);
        void on_obsspin_updateenter( wxCommandEvent& event);

        void on_special_update( wxCommandEvent& event );
        void on_specialspin_update( wxSpinEvent& event);
        void on_specialspin_updateenter( wxCommandEvent& event);

        void on_silverspoilspin_update( wxSpinEvent& event);
        void on_silverspoilspin_updateenter( wxCommandEvent& event);
        void on_spoilcombo_update( wxCommandEvent& event );

        void on_hostilespin_update( wxSpinEvent& event);
        void on_hostilespin_updateenter( wxCommandEvent& event);
        void on_numberspin_update( wxSpinEvent& event);
        void on_numberspin_updateenter( wxCommandEvent& event);

        void on_mdefnametext_update( wxCommandEvent& event );

        std::string compute_escape();
        //MonType *find_monsterdef(char const *abbr, int illusion);

    private:
        enum
        {
            idAbbrCombo = 100,
            idAbbrText  = 101,
            idWeightUpdate = 150,
            idBasepriceUpdate = 151,
            idEscLossChance = 200,
            idEscHasSkill = 201,
            idEscLinear = 202,
            idEscSquare = 203,
            idEscCube = 204,
            idEscQuad = 205,
            idEscNumSquare = 206,
            idEscLinked = 207,
        };
        wxDECLARE_EVENT_TABLE();

 };

void mk_field(wxWindow *parent, int id, wxBoxSizer *sizer, std::string title,
              wxTextCtrl*& wxtext, int proportionnal, int width );
void mk_check(wxWindow *parent, int id, wxBoxSizer *sizer, std::string title,
              wxCheckBox*& wxcheck );
void mk_radio(wxWindow *parent, int id, wxBoxSizer *sizer, std::string title,
              wxRadioButton*& wxradio );
void mk_title(wxWindow *parent, wxBoxSizer *sizer, std::string title);
void mk_spin(wxWindow *parent, int id, wxBoxSizer* sizer, std::string title,
             wxSpinCtrl*& wxspin, int rmin, int rmax, int width);

#endif // MONSTERVIEW_H_INCLUDED
