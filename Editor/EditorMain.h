/***************************************************************
 * Name:      EditorMain.h
 * Purpose:   Defines Application Frame
 * Author:     (snowgoon88@gmail.com)
 * Created:   2016-09-25
 * Copyright:  ()
 * License:
 **************************************************************/

#ifndef EDITORMAIN_H
#define EDITORMAIN_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "EditorApp.h"

class EditorFrame: public wxFrame
{
    public:
        EditorFrame(wxFrame *frame, const wxString& title);
        ~EditorFrame();
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


#endif // EDITORMAIN_H
