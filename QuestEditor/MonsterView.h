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

/**
 * View/edit a Monster.
 */
 class MonsterView : public wxPanel
 {
    public:
        MonsterView( wxWindow *parent );

        /** Parse files */
        void parse_gamedata();
        /** set to an existing Monster */
        void set_monster( int num );

        std::map<std::string,int> _map_abbr;
//        std::vector<wxString> _list_abbr;
        // abr, name and names
        wxComboBox *_abbr_combo;
        wxTextCtrl *_abbr_text;
        wxTextCtrl *_name_text;
        wxTextCtrl *_names_text;
        // flags
        wxCheckBox *_disabled_check;
        wxCheckBox *_cantgive_check;
        // magic production
        wxTextCtrl *_mSkill_text;
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
        wxCheckBox *_loselinked_check, *_hasskill_check, *_esclinear_check,
            *_escsquare_check, *_esccube_check, *_escquad_check,
            *_losschance_check, *_escnum_check;
        wxTextCtrl *_eSkill_text;
        wxSpinCtrl *_esc_spin;
        // attack
        wxSpinCtrl *_atk_spin, *_numatk_spin, *_nbhits_spin, *_regen_spin;
        // defense
        wxSpinCtrl *_def_combat_spin, *_def_nrj_spin, *_def_spirit_spin,
            *_def_weather_spin, *_def_riding_spin, *_def_ranged_spin;
        // skills
        wxSpinCtrl *_tactics_spin, *_stealth_spin, *_obs_spin;
        // special
        wxTextCtrl *_special_text;
        wxSpinCtrl *_specialLevel_spin;
        // spoil
        wxSpinCtrl *_silverspoil_spin;
        wxCheckBox *_normalspoil_check, *_advancedspoil_check, *_tradespoil_check, *_manspoil_check,
            *_monsterspoil_check, *_magicspoil_check, *_weaponspoil_check, *_armorspoil_check,
            *_mountspoil_check, *_battlespoil_check, *_specialspoil_check, *_toolspoil_check,
            *_foodspoil_check, *_illusionspoil_check, *_undeadspoil_check, *_demonspoil_check,
            *_leaderspoil_check, *_moneyspoil_check, *_animalspoil_check, *_shipspoil_check,
            *_mageonlyspoil_check, *_alwaysspoil_check, *_neverspoil_check;
        // hostile, number
        wxSpinCtrl *_hostile_spin, *_number_spin;
        // mdef abr and name
        wxTextCtrl *_mdefname_text, *_mdefabbr_text;

        void on_cbox_update( wxCommandEvent& event );

        MonType *find_monsterdef(char const *abbr, int illusion);

    private:
        enum
        {
            idAbbrCombo = 100,
        };
        wxDECLARE_EVENT_TABLE();

 };

void mk_field(wxWindow *parent, int id, wxBoxSizer *sizer, std::string title,
              wxTextCtrl*& wxtext, int proportionnal, int width );
void mk_check(wxWindow *parent, int id, wxBoxSizer *sizer, std::string title,
              wxCheckBox*& wxcheck );
void mk_title(wxWindow *parent, wxBoxSizer *sizer, std::string title);
void mk_spin(wxWindow *parent, int id, wxBoxSizer* sizer, std::string title,
             wxSpinCtrl*& wxspin, int rmin, int rmax, int width);

#endif // MONSTERVIEW_H_INCLUDED
