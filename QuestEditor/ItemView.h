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
        std::map<std::string,int> _map_mSkill;
        std::map<std::string,int> _map_eSkill;
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
        wxCheckBox *_cantgive_check;
        // magic production
        wxComboBox *_mSkill_combo;
        //wxTextCtrl *_mSkill_text;
        wxSpinCtrl *_mLevel_spin;
        // weight, baseprice
        wxSpinCtrl *_weight_spin, *_baseprice_spin;
        // type
        wxCheckBox *_normal_check, *_advanced_check, *_trade_check, *_magic_check;
        wxCheckBox *_weapon_check, *_armor_check, *_mount_check, *_battle_check;
        wxCheckBox *_special_check, *_tool_check, *_food_check, *_ship_check;
        wxCheckBox *_mageonly_check, *_neverspoil_check;
        // movements
        wxSpinCtrl *_walk_spin, *_ride_spin, *_fly_spin, *_swim_spin, *_speed_spin;
        // max inventory
        wxSpinCtrl *_maxinventory_spin;

        // WEAPON
        std::map<std::string,int> _map_bSkill;
        wxPanel *_weapon_panel;
        wxCheckBox *_needskill_ch, *_alwaysready_ch, *_nodefense_ch, *_nofoot_ch, *_nomount_ch;
        wxCheckBox *_short_ch, *_long_ch, *_ranged_ch, *_noattackerskill_ch, *_riding_bonus_ch;
        wxCheckBox *_riding_bonus_defense_ch;

        wxComboBox *_bSkill_combo, *_oSkil_combo;
        wxSpinCtrl *_weap_class_combo; //, *_attack_type_combo; == ATTACK_COMBAT
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
        wxSpinCtrl *_atk_bonus_spin, *_def_bonus_spin, *_mount_bonus_spin;


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
        void on_maxinventoryspin_update( wxSpinEvent& event);
        void on_maxinventoryspin_updateenter( wxCommandEvent& event);


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

 }; // ItemView
#endif // ITEMVIEW_H_INCLUDED
