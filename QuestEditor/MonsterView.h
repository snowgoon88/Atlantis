#ifndef MONSTERVIEW_H_INCLUDED
#define MONSTERVIEW_H_INCLUDED

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "gamedata.h"

/**
 * View/edit a Monster.
 */
 class MonsterView : public wxPanel
 {
    public:
        MonsterView( wxWindow *parent );

        /** set to an existing Monster */
        void set_monster( int num );

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

        void on_cbox_update( wxCommandEvent& event );

    private:
        wxDECLARE_EVENT_TABLE();

 };


#endif // MONSTERVIEW_H_INCLUDED
