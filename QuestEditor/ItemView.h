#ifndef ITEMVIEW_H_INCLUDED
#define ITEMVIEW_H_INCLUDED

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx_utils.h>
//#include <wx/spinctrl.h>

#include "gamedata.h"
#include "items.h"
#include <vector>
#include <fstream>
#include <string>
#include <map>
#include <item_data.h>
#include <all_data.h>


/**
 * View/edit an Item.
 */
 class ItemView : public wxPanel
 {
    public:
        ItemView( wxWindow *parent, AllData& data );

        /** set to an existing Item */
        void set_item( AItem* item );
        void add_item( AItem* new_item );

        AllData& _data;
        AItem* _item;
        std::map<std::string,int> _map_abbr;
        std::map<std::string,int> _map_pSkill;
        std::map<std::string,int> _map_mSkill;
        std::map<std::string,int> _map_allSkill;
        std::map<std::string,int> _map_special;
        //std::map<std::string,int> _map_spoil;
//        std::vector<wxString> _list_abbr;
        // abr, name and names
        wxComboBox *_abbr_combo;
        wxCheckBox *_edit_check;

        wxPanel* _item_panel;

        wxTextCtrl *_abbr_text;
        wxTextCtrl *_name_text;
        wxTextCtrl *_names_text;
        // flags
        wxCheckBox *_disabled_check;
        wxCheckBox *_nomarket_check, *_orinput_check, *_skillout_check, *_notransport_check;
        //wxCheckBox *_cantgive_check;
        // normal production
        wxComboBox *_pSkill_combo;
        //wxTextCtrl *_mSkill_text;
        wxSpinCtrl *_pLevel_spin, *_pMonth_spin, *_pOut_spin;
        // Materials
        wxComboBox *_pMat_combo[4];
        wxSpinCtrl *_pMat_spin[4];
        // magic production
        wxComboBox *_mSkill_combo;
        //wxTextCtrl *_mSkill_text;
        wxSpinCtrl *_mLevel_spin;
        // Materials
        wxComboBox *_mMat_combo[4];
        wxSpinCtrl *_mMat_spin[4];
        // weight, baseprice
        wxSpinCtrl *_weight_spin, *_baseprice_spin, *_combat_spin;

        // type
        wxCheckBox *_normal_check, *_advanced_check, *_trade_check, *_magic_check;
        wxCheckBox *_weapon_check, *_armor_check, *_mount_check, *_battle_check;
        wxCheckBox *_special_check, *_tool_check, *_food_check, *_ship_check;
        wxCheckBox *_mageonly_check, *_neverspoil_check;
        // movements
        wxSpinCtrl *_walk_spin, *_ride_spin, *_fly_spin, *_swim_spin, *_speed_spin;
        // max inventory
        wxSpinCtrl *_hitchitem_spin, *_hitchwalk_spin, *_mult_item_spin, *_mult_val_spin;
        wxSpinCtrl *_maxinventory_spin;

        // Grant skill
        wxComboBox *_gSkill_combo;
        wxComboBox *_fSkill_combo[4];
        wxSpinCtrl *_min_grant_spin, *_max_grant_spin;

        // WEAPON
        std::map<std::string,int> _map_bSkill;
        std::map<std::string,int> _map_wClass;
        wxPanel *_weapon_panel;
        wxCheckBox *_needskill_ch, *_alwaysready_ch, *_nodefense_ch, *_nofoot_ch, *_nomount_ch;
        wxCheckBox *_short_ch, *_long_ch, *_ranged_ch, *_noattackerskill_ch, *_riding_bonus_ch;
        wxCheckBox *_riding_bonus_defense_ch;

        wxComboBox *_bSkill_combo, *_oSkill_combo;
        wxComboBox *_weap_class_combo; //, *_attack_type_combo; == ATTACK_COMBAT
//     enum {
//	ATTACK_COMBAT,
//	ATTACK_ENERGY,
//	ATTACK_SPIRIT,
//	ATTACK_WEATHER,
//	ATTACK_RIDING,
//	ATTACK_RANGED,
//	NUM_ATTACK_TYPES
//};
        wxSpinCtrl *_num_attack_spin;
        wxStaticText *_num_attack_text;
        wxSpinCtrl *_atk_bonus_spin, *_def_bonus_spin, *_mount_bonus_spin;

        // ARMOR
        wxPanel *_armor_panel;
        wxCheckBox *_use_in_assassinate_check;
        wxSpinCtrl *_from_spin;
        wxSpinCtrl *_chances_spin[NUM_WEAPON_CLASSES];
        wxStaticText *_saves_armor_text;

        // MOUNT
        wxPanel *_mount_panel;
        wxComboBox *_mount_skill_combo;
        wxSpinCtrl *_mount_min_spin, *_mount_max_spin, *_mount_hampered_spin;
        wxComboBox *_mount_special_combo;
        wxSpinCtrl *_mount_special_lvl_spin;

        // BATTLE ITEM
        std::map<std::string,int> _map_specSkill;
        wxPanel *_battle_panel;
        wxCheckBox *_battle_mageonly_check, *_battle_special_check, *_battle_shield_check, *_battle_exclusive_check;
        wxComboBox *_battle_special_combo; // SpecialDefs
        wxSpinCtrl *_battle_lvl_spin;

        // Callbacks
        void on_cbox_update( wxCommandEvent& event );
        void on_edit_update( wxCommandEvent& event );
        void on_abbrtext_update( wxCommandEvent& event );
        void on_nametext_update( wxCommandEvent& event );
        void on_namestext_update( wxCommandEvent& event );
        void on_disabled_update( wxCommandEvent& event );
        void on_nomarket_update( wxCommandEvent& event );
        void on_orinput_update( wxCommandEvent& event );
        void on_skillout_update( wxCommandEvent& event );
        void on_notransport_update( wxCommandEvent& event );
        void on_pSkillcombo_update( wxCommandEvent& event);
        void on_pLevelspin_update(wxSpinEvent& event);
        void on_pLevelspin_updateenter(wxCommandEvent& event);
        void on_pMonthspin_update(wxSpinEvent& event);
        void on_pMonthspin_updateenter(wxCommandEvent& event);
        void on_pOutspin_update(wxSpinEvent& event);
        void on_pOutspin_updateenter(wxCommandEvent& event);
        void on_pMatcombo_update( wxCommandEvent& event);
        void on_pMatspin_update(wxSpinEvent& event);
        void on_pMatspin_updateenter(wxCommandEvent& event);

        void on_mSkillcombo_update( wxCommandEvent& event);
        void on_mLevelspin_update(wxSpinEvent& event);
        void on_mLevelspin_updateenter(wxCommandEvent& event);
        void on_mMatcombo_update( wxCommandEvent& event);
        void on_mMatspin_update(wxSpinEvent& event);
        void on_mMatspin_updateenter(wxCommandEvent& event);

        void on_weightspin_update( wxSpinEvent& event);
        void on_weightspin_updateenter( wxCommandEvent& event);
        void on_basepricespin_update( wxSpinEvent& event);
        void on_basepricespin_updateenter( wxCommandEvent& event);
        void on_combatspin_update( wxSpinEvent& event);
        void on_combatspin_updateenter( wxCommandEvent& event);
        // type IT_NORMAL, IT_ADVANCED, IT_TRADE, IT_MAGIC, IT_WEAPON, IT_ARMOR, IT_MOUNT
    //       IT_BATTLE, IT_SPECIAL, IT_TOOL, IT_FOOD, IT_SHIP, MAGEONLY, IT_NEVER_SPOIL
        void on_normal_update( wxCommandEvent& event );
        void on_advanced_update( wxCommandEvent& event );
        void on_trade_update( wxCommandEvent& event );
        void on_magic_update( wxCommandEvent& event );
        void on_weapon_update( wxCommandEvent& event );
        void on_armor_update( wxCommandEvent& event );
        void on_mount_update( wxCommandEvent& event );
        void on_battle_update( wxCommandEvent& event );
        void on_special_update( wxCommandEvent& event );
        void on_tool_update( wxCommandEvent& event );
        void on_food_update( wxCommandEvent& event );
        void on_ship_update( wxCommandEvent& event );
        void on_mageonly_update( wxCommandEvent& event );
        void on_neverspoil_update( wxCommandEvent& event );

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
        void on_hitchitemspin_update( wxSpinEvent& event);
        void on_hitchitemspin_updateenter( wxCommandEvent& event);
        void on_hitchwalkspin_update( wxSpinEvent& event);
        void on_hitchwalkspin_updateenter( wxCommandEvent& event);
        void on_mult_itemspin_update( wxSpinEvent& event);
        void on_mult_itemspin_updateenter( wxCommandEvent& event);
        void on_mult_valspin_update( wxSpinEvent& event);
        void on_mult_valspin_updateenter( wxCommandEvent& event);
        void on_maxinventoryspin_update( wxSpinEvent& event);
        void on_maxinventoryspin_updateenter( wxCommandEvent& event);

        void on_gSkillcombo_update( wxCommandEvent& event);
        void on_fSkillcombo_update( wxCommandEvent& event);
        void on_min_grantspin_update( wxSpinEvent& event);
        void on_min_grantspin_updateenter( wxCommandEvent& event);
        void on_max_grantspin_update( wxSpinEvent& event);
        void on_max_grantspin_updateenter( wxCommandEvent& event);

        void on_weaponcheck_update( wxCommandEvent& event);
        void on_bSkillcombo_update( wxCommandEvent& event);
        void on_oSkillcombo_update( wxCommandEvent& event);
        void on_weapClasscombo_update( wxCommandEvent& event);
        void on_numattackspin_update( wxSpinEvent& event);
        void on_numattackspin_updateenter( wxCommandEvent& event);
        void on_atk_bonusspin_update( wxSpinEvent& event);
        void on_atk_bonusspin_updateenter( wxCommandEvent& event);
        void on_def_bonusspin_update( wxSpinEvent& event);
        void on_def_bonusspin_updateenter( wxCommandEvent& event);
        void on_mount_bonusspin_update( wxSpinEvent& event);
        void on_mount_bonusspin_updateenter( wxCommandEvent& event);

        void on_use_in_assassinatecheck_update( wxCommandEvent& event);
        void on_fromspin_update( wxSpinEvent& event);
        void on_fromspin_updateenter( wxCommandEvent& event);
        void on_chancesspin_update( wxSpinEvent& event);
        void on_chancesspin_updateenter( wxCommandEvent& event);

        // MOUNT
        void on_mount_skillcombo_update( wxCommandEvent& event);
        void on_mount_maxspin_update( wxSpinEvent& event);
        void on_mount_maxspin_updateenter( wxCommandEvent& event);
        void on_mount_minspin_update( wxSpinEvent& event);
        void on_mount_minspin_updateenter( wxCommandEvent& event);
        void on_mount_hamperedspin_update( wxSpinEvent& event);
        void on_mount_hamperedspin_updateenter( wxCommandEvent& event);
        void on_mount_specialcombo_update( wxCommandEvent& event);
        void on_mount_special_lvlspin_update( wxSpinEvent& event);
        void on_mount_special_lvlspin_updateenter( wxCommandEvent& event);

        // BATTLE ITEM
        void on_bat_mageonly_check_update( wxCommandEvent& event);
        void on_bat_special_check_update( wxCommandEvent& event);
        void on_bat_shield_check_update( wxCommandEvent& event);
        void on_bat_exclusive_check_update( wxCommandEvent& event);
        void on_bat_specialcombo_update( wxCommandEvent& event);
        void on_bat_specialspin_update( wxSpinEvent& event);
        void on_bat_specialspin_updateenter( wxCommandEvent& event);



        std::string compute_numattack();
        std::string compute_armorsaves();

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
            idWeapNeedSkill = 300,
            idWeapAlwaysReady = 301,
            idWeapNoDefense = 302,
            idWeapNoFoot = 303,
            idWeapNoMount = 304,
            idWeapShort = 305,
            idWeapLong = 306,
            idWeapRanged = 307,
            idWeapNoAttackerSkill = 308,
            idWeapRidingBonus = 309,
            idWeapRidingBonusDefense = 310,
            idArmorBonusBase = 400,
            idGrantSkill = 500,
            idProdMat = 600,
            idMagMat = 700
        };
        wxDECLARE_EVENT_TABLE();

 }; // ItemView
#endif // ITEMVIEW_H_INCLUDED
