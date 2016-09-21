/***************************************************************
 * Name:      QuestEditorMain.h
 * Purpose:   Defines Application Frame
 * Author:     (snowgoon88@gmail.com)
 * Created:   2016-09-07
 * Copyright:  ()
 * License:
 **************************************************************/

#ifndef QUESTEDITORMAIN_H
#define QUESTEDITORMAIN_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "QuestEditorApp.h"
#include "MonsterView.h"

/**
 * Main Window
 */
class QuestEditorFrame: public wxFrame
{
    public:
        QuestEditorFrame(wxFrame *frame, const wxString& title, const wxSize& size = wxDefaultSize);
        ~QuestEditorFrame();


        MonsterData _monster_data;

        wxPanel *_panel;
        wxTextCtrl *_textCtrl;
        wxStaticText *_label;
        MonsterView *_monster_view;
    private:
        enum
        {
            idMenuFileOpen = 1,
            idMenuMonsterAdd = 100,
            idMenuMonsterDebug = 101,
            idMenuQuit = 1000,
            idMenuAbout
        };
        void OnOpenFile(wxCommandEvent& event);
        void OnMonsterAdd( wxCommandEvent& event);
        void OnMonsterDebug( wxCommandEvent& event);
        void OnClose(wxCloseEvent& event);
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        DECLARE_EVENT_TABLE()
};


#endif // QUESTEDITORMAIN_H
