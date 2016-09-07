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

class QuestEditorFrame: public wxFrame
{
    public:
        QuestEditorFrame(wxFrame *frame, const wxString& title);
        ~QuestEditorFrame();
    private:
        enum
        {
            idMenuQuit = 1000,
            idMenuAbout
        };
        void OnClose(wxCloseEvent& event);
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        DECLARE_EVENT_TABLE()
};


#endif // QUESTEDITORMAIN_H
