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
#include <MapViewer.h>
#include <RegViewer.h>
#include <game.h>
#include <map_access.h>

class EditorFrame: public wxFrame
{
    public:
        EditorFrame(wxFrame *frame, const wxString& title);
        ~EditorFrame();

        Game _game;
        MapViewer* _map_viewer;
        RegViewer* _reg_viewer;
        RegionData* _region_data;

    private:
        enum
        {
            idMenuSurface = 100,
            idMenuUnderWorld = 101,
            idMenuUnderDeep = 102,
            idMenuChangeName = 200,
            idMenuQuit = 1000,
            idMenuAbout
        };
        void OnRegion( wxCommandEvent& event);
        void OnClose(wxCloseEvent& event);
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);

        void OnChangeName(wxCommandEvent& event);
        DECLARE_EVENT_TABLE()
};


#endif // EDITORMAIN_H
