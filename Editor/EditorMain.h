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

#define NB_MAX_REGION 20

class EditorFrame: public wxFrame
{
    public:
        EditorFrame(wxFrame *frame, const wxString& title);
        ~EditorFrame();

        Game* _game;
        MapAccess* _map_access;
        MapViewer* _map_viewer;
        RegViewer* _reg_viewer;
        RegionData* _region_data;

    private:
        wxMenu* _regionMenu;
        wxMenuItem* _subregionMenu[NB_MAX_REGION];
        void load_game();
        void save_game();

        enum
        {
//            idMenuSurface = 100,
//            idMenuUnderWorld = 101,
//            idMenuUnderDeep = 102,
            idMenuChangeName = 200,
            idMenuMoveGate = 201,
            idMenuAddShaft = 202,
            idMenuRemoveShaft = 203,
            idMenuSetTerrain = 204,
            idMenuSetRace = 205,
            idMenuReload = 500,
            idMenuSave = 501,
            idMenuQuit = 1000,
            idMenuAbout,
            idRegionBase = 2000, // No id AFTER Need at least NB_MAX_REGION id !!!!!!!
        };
        void OnReload( wxCommandEvent& event );
        void OnSave( wxCommandEvent& event );
        void OnRegion( wxCommandEvent& event);
        void OnClose(wxCloseEvent& event);
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);

        void OnChangeName(wxCommandEvent& event);
        void OnMoveGate(wxCommandEvent& event);
        void OnAddShaft( wxCommandEvent& event);
        void OnRemoveShaft( wxCommandEvent& event);
        void OnSetTerrain( wxCommandEvent& event);
        void OnSetRace( wxCommandEvent& event);
        DECLARE_EVENT_TABLE()
};


#endif // EDITORMAIN_H
