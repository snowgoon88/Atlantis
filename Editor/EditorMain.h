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
#include <LocalisationListener.h>
#include <UnitEditListener.h>
#include <ObjectListener.h>
#include <UnitListener.h>

#define NB_MAX_REGION 20

class EditorFrame: public wxFrame,
    public LocalisationListener, public UnitEditListener, public ObjectListener,
    public UnitListener
{
    public:
        EditorFrame(wxFrame *frame, const wxString& title);
        ~EditorFrame();

        Game* _game;
        MapAccess* _map_access;
        MapViewer* _map_viewer;
        RegViewer* _reg_viewer;
        RegionData* _region_data;

        void enable_treeMenu( bool state );

        /** Overide Listeners */
        virtual void receive_localisation(Localisation& loc);
        virtual void receive_unitedit();
        virtual void receive_object( int type );
        virtual void receive_unit(Unit* unit);

    private:
        wxMenu* _regionMenu;
        wxMenuItem* _subregionMenu[NB_MAX_REGION];
        wxMenuItem* _editUnitMenu;
        wxMenuItem* _addUnitMenu;
        wxMenuItem* _moveUnitMenu;
        wxMenuItem* _delUnitMenu;
        wxMenuItem* _addObjectMenu;
        wxMenuItem* _delObjectMenu;
        wxMenuItem* _findRegMenu;
        wxMenuItem* _findUnitMenu;
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
            idMenuAddGate = 210,
            idMenuDelGate = 211,
            idMenuSwapGate = 212,
            idMenuTownAdd = 300,
            idMenuTownDel = 301,
            idMenuTownRegenerate = 302,
            idMenuTownRename = 303,
            idMenuTownMarket = 304,
            idMenuUnitEdit = 400,
            idMenuUnitAdd = 401,
            idMenuUnitMove = 402,
            idMenuUnitDel = 403,
            idMenuObjectAdd = 410,
            idMenuObjectDel= 411,
            idMenuFindRegion = 450,
            idMenuFindUnit = 451,
            idMenuReload = 500,
            idMenuSave = 501,
            idMenuQuit = 1000,
            idMenuAbout,
            idRegionBase = 2000, // No id AFTER Need at least NB_MAX_REGION id !!!!!!!
        };
        wxAcceleratorEntry _accel_entries[9]; // After 20->SetRegion
        void OnReload( wxCommandEvent& event );
        void OnSave( wxCommandEvent& event );
        void OnRegion( wxCommandEvent& event);
        void OnClose(wxCloseEvent& event);
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);

        void OnChangeName(wxCommandEvent& event);
        void OnMoveGate(wxCommandEvent& event);
        void OnAddGate( wxCommandEvent& event);
        void OnDelGate( wxCommandEvent& event);
        void OnSwapGate( wxCommandEvent& event);

        void OnAddShaft( wxCommandEvent& event);
        void OnRemoveShaft( wxCommandEvent& event);
        void OnSetTerrain( wxCommandEvent& event);
        void OnSetRace( wxCommandEvent& event);

        void OnTown( wxCommandEvent& event);

        void OnUnitEdit( wxCommandEvent& event);
        void OnUnitAdd( wxCommandEvent& event);
        void OnUnitMove( wxCommandEvent& event);
        void OnUnitDel( wxCommandEvent& event);
        Unit* _moveable_unit;

        void OnObjectAdd( wxCommandEvent& event );
        void OnObjectDel( wxCommandEvent& event );
        ARegion* _selected_reg;

        void OnFindRegion( wxCommandEvent& event );/** uses also _moveable_unit */
        void OnFindUnit( wxCommandEvent& event );


        DECLARE_EVENT_TABLE()
};


#endif // EDITORMAIN_H
