/***************************************************************
 * Name:      EditorMain.cpp
 * Purpose:   Code for Application Frame
 * Author:     (snowgoon88@gmail.com)
 * Created:   2016-09-25
 * Copyright:  ()
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "EditorMain.h"

#include <wx/numdlg.h>
#include <wx/splitter.h>

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__WXMAC__)
        wxbuild << _T("-Mac");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

BEGIN_EVENT_TABLE(EditorFrame, wxFrame)
    EVT_CLOSE(EditorFrame::OnClose)
    EVT_MENU(idMenuQuit, EditorFrame::OnQuit)
    EVT_MENU(idMenuAbout, EditorFrame::OnAbout)
    EVT_MENU(idMenuReload, EditorFrame::OnReload)
    EVT_MENU(idMenuSave, EditorFrame::OnSave)
//    EVT_MENU(idMenuSurface, EditorFrame::OnRegion)
//    EVT_MENU(idMenuUnderWorld, EditorFrame::OnRegion)
//    EVT_MENU(idMenuUnderDeep, EditorFrame::OnRegion)
    EVT_MENU(idMenuChangeName, EditorFrame::OnChangeName)
    EVT_MENU(idMenuMoveGate, EditorFrame::OnMoveGate)
    EVT_MENU(idMenuAddShaft, EditorFrame::OnAddShaft)
    EVT_MENU(idMenuRemoveShaft, EditorFrame::OnRemoveShaft)
    EVT_MENU(idMenuSetTerrain, EditorFrame::OnSetTerrain)
    EVT_MENU(idMenuSetRace, EditorFrame::OnSetRace)

    EVT_MENU(idMenuTownAdd, EditorFrame::OnTown)
    EVT_MENU(idMenuTownDel, EditorFrame::OnTown)
    EVT_MENU(idMenuTownRegenerate, EditorFrame::OnTown)
    EVT_MENU(idMenuTownRename, EditorFrame::OnTown)
    EVT_MENU(idMenuTownMarket, EditorFrame::OnTown)
END_EVENT_TABLE()

EditorFrame::EditorFrame(wxFrame *frame, const wxString& title)
    : wxFrame(frame, -1, title, wxDefaultPosition, wxSize(1000,600))
{
#if wxUSE_MENUS
    // create a menu bar
    wxMenuBar* mbar = new wxMenuBar();
    wxMenu* fileMenu = new wxMenu(_T(""));
//    fileMenu->Append(idMenuReload, _("&Reload"), _("Reload game.in and reset..."));
    fileMenu->Append(idMenuSave, _("&Save"), _("Save into game.out"));
//    fileMenu->Append(idMenuSurface, _("&Surface"), _("Display region"));
//    fileMenu->Append(idMenuUnderWorld, _("&UnderWorld"), _("Display region"));
//    fileMenu->Append(idMenuUnderDeep, _("&UnderDeep"), _("Display region"));
    fileMenu->Append(idMenuQuit, _("&Quit\tAlt-F4"), _("Quit the application"));
    mbar->Append(fileMenu, _("&File"));

    _regionMenu = new wxMenu(_T(""));
    mbar->Append(_regionMenu, _("&Region"));

    wxMenu* actMenu = new wxMenu(_T(""));
    actMenu->Append(idMenuChangeName, _("&Change Name"), _("Change le nom de 1 ou plusieurs hex"));
    actMenu->Append(idMenuMoveGate, _("&Move Gate"), _("Change une GATE de place"));
    actMenu->Append(idMenuAddShaft, _("&Add Shaft"), _("Ajoute un SHAFT entre 2 hex"));
    actMenu->Append(idMenuRemoveShaft, _("&Remove Shaft"), _("Enleve un des SHAFT de l'hex"));
    actMenu->Append(idMenuSetTerrain, _("&Set Terrain"), _("Change le type de terrain de 1 ou plusieurs hex"));
    actMenu->Append(idMenuSetRace, _("&Set Race"), _("Change la race d'un hex"));
    mbar->Append(actMenu, _("&Action"));

    wxMenu* townMenu = new wxMenu(_T(""));
    townMenu->Append( idMenuTownAdd, _("Add Town"), _("Ajoute une nouvelle ville"));
    townMenu->Append( idMenuTownDel, _("Del Town"), _("Enleve une ville"));
    townMenu->Append( idMenuTownRename, _("Rename Town"), _("Change le nom d'une ville"));
    townMenu->Append( idMenuTownRegenerate, _("Regenerate Town"), _("Recalcule les donnees d'une ville"));
    wxMenuItem* marketMenu = townMenu->Append( idMenuTownMarket, _("Edit Market"), _("Edite les produits du marche"));
    marketMenu->Enable(false);
    mbar->Append(townMenu, _("&Town"));

    wxMenu* helpMenu = new wxMenu(_T(""));
    helpMenu->Append(idMenuAbout, _("&About\tF1"), _("Show info about this application"));
    mbar->Append(helpMenu, _("&Help"));

    SetMenuBar(mbar);
#endif // wxUSE_MENUS

#if wxUSE_STATUSBAR
    // create a status bar with some information about the used wxWidgets version
    CreateStatusBar(2);
    SetStatusText(_("Hello Code::Blocks user!"),0);
    SetStatusText(wxbuildinfo(short_f), 1);
#endif // wxUSE_STATUSBAR

    for( int i=0; i<NB_MAX_REGION; ++i ) {
        _subregionMenu[i] = nullptr;
    }
    // Essai pour game
    //Game game;


    load_game();

    wxSplitterWindow* split_win = new wxSplitterWindow( this );

//    wxPanel* _main_panel = new wxPanel( this );
//    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    _map_viewer = new MapViewer( split_win, *_region_data );
    _region_data->attach_observer( _map_viewer );
//    sizer->Add( _map_viewer, 1, wxEXPAND);
    _reg_viewer = new RegViewer( split_win, *_region_data );
    _region_data->attach_observer( _reg_viewer );
    _map_viewer->attach_regviewer( _reg_viewer );
//    sizer->Add( _reg_viewer, 0, wxEXPAND | wxLEFT | wxRIGHT, 5);

//    _main_panel->SetSizer(sizer);
    split_win->SplitVertically( _map_viewer, _reg_viewer);
    //map_panel->attach( regions->pRegionArrays[1] );
    //frame->SetAutoLayout(true);

//    frame->Show();
//    return true;
}
void EditorFrame::load_game()
{
    _game = new Game();
    initIO();
    _game->ModifyTablesPerRuleset();
    if ( !_game->OpenGame() ) {
        wxMessageBox("Impossible de charger 'game.in'. EXIT ", "Load Game", wxOK, this);
        exit(1);
    }
    _map_access = new MapAccess( _game );
    ARegionList* regions = _map_access->regions();
    if( _region_data ) delete _region_data;
    _region_data = new RegionData( this, regions, _map_access );

    // list all the levels and make subMenu
    for( int i=0; i<NB_MAX_REGION; ++i ) {
        if( _subregionMenu[i] ) _regionMenu->Delete( _subregionMenu[i] );
        if( _subregionMenu[i]) delete _subregionMenu[i];
    }
    for (int i = 1; i < regions->numLevels; i++) {
        std::stringstream label;
        label << "Region " << i;
        _subregionMenu[i] = _regionMenu->Append( idRegionBase+i, wxString(label.str()), _("Display region"));
        Bind(wxEVT_MENU, &EditorFrame::OnRegion, this, idRegionBase+i);

//        ARegionArray *pArr = regions->pRegionArrays[i];
//        std::cout << "{"<<i<<"} " << mapAccess.str_display( pArr, false) << std::endl;
//        if( pArr->strName )
//            std::cout << "          name=" << *(pArr->strName) << std::endl;
    }
}
void EditorFrame::save_game()
{
    if ( !_game->SaveGame() ) {
        wxMessageBox("Impossible de sauver dans 'game.out'. Bizarre ", "Save Game", wxOK, this);
    }
    wxMessageBox("Sauvegarde reussie.", "Save Game", wxOK, this);
}


EditorFrame::~EditorFrame()
{
}

void EditorFrame::OnReload(     wxCommandEvent& event)
{
    std::cout << "RELOAD id=" << event.GetId() << std::endl;
    int res = wxMessageBox("Recharger va ANNULER toutes les modifications.Ok ?", "Load Game", wxOK|wxCANCEL, this);
    if( res == wxOK ) {
        if( _game ) delete _game;
        load_game();
    }
}
void EditorFrame::OnSave( wxCommandEvent& event )
{
    save_game();
}

void EditorFrame::OnRegion( wxCommandEvent& event)
{
    std::cout << "REGION id=" << event.GetId() << std::endl;

//    MapAccess map_access = MapAccess( _game );
    ARegionList* regions = _map_access->regions();
    ARegionArray *pArr = nullptr;
    if( event.GetId() > idRegionBase and event.GetId() < idRegionBase+NB_MAX_REGION+1) {
        pArr = regions->pRegionArrays[event.GetId()-idRegionBase];
    }

//    if( event.GetId() == idMenuSurface ) {
//        pArr = regions->pRegionArrays[1];
//    }
//    if( event.GetId() == idMenuUnderWorld ) {
//        pArr = regions->pRegionArrays[2];
//    }
//    if( event.GetId() == idMenuUnderDeep ) {
//        pArr = regions->pRegionArrays[3];
//    }
    if( pArr ) {
        if( pArr->strName )
            std::cout << "ATTACH name=" << *(pArr->strName) << std::endl;
        _map_viewer->attach( pArr );
    }
}

void EditorFrame::OnClose(wxCloseEvent &event)
{
    Destroy();
}

void EditorFrame::OnQuit(wxCommandEvent &event)
{
    Destroy();
}

void EditorFrame::OnAbout(wxCommandEvent &event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}

void EditorFrame::OnChangeName(wxCommandEvent& event)
{
    if( _map_viewer->_selected_list.size() > 0 ) {
        wxString name = wxGetTextFromUser( wxString("Nouveau nom:"),
                            wxString("Change Name"),
                            _("--nom--"),
                            this );
        _region_data->set_name( _map_viewer->_selected_list, name.ToStdString());
    }
}
void EditorFrame::OnMoveGate(wxCommandEvent& event)
{
    if( _map_viewer->_selected_list.size() != 1 ) {
        wxMessageBox("Il faut choisir UNE region avec GATE ", "Move Gate", wxOK, this);
    }
    else {
        int reg_num = wxGetNumberFromUser( wxString("Numero de la region destinatrice?"),
		wxString("Prompt"),
		wxString("Move Gate"),
		0, 0, 100000, this);
		_region_data->move_gate( _map_viewer->_selected_list.front(), reg_num );
    }
    //wxMessageBox("Tu veux vraiment modifier ce monstre ?? ", "Editer", wxOK | wxCANCEL, this);
}
void EditorFrame::OnAddShaft(wxCommandEvent& event)
{
    if( _map_viewer->_selected_list.size() != 1 ) {
        wxMessageBox("Il faut choisir UNE region de depart ", "Add Shaft", wxOK, this);
    }
    else {
        int reg_num = wxGetNumberFromUser( wxString("Numero de la region destinatrice?"),
		wxString("Prompt"),
		wxString("Add Shaft"),
		0, 0, 100000, this);
		_region_data->add_shaft( _map_viewer->_selected_list.front(), reg_num );
    }
}
void EditorFrame::OnRemoveShaft(wxCommandEvent& event)
{
    if( _map_viewer->_selected_list.size() != 1 ) {
        wxMessageBox("Il faut choisir UNE region de depart ", "Remove Shaft", wxOK, this);
    }
    else {
        _region_data->remove_shaft( _map_viewer->_selected_list.front() );
    }
}
void EditorFrame::OnSetTerrain(wxCommandEvent& event)
{
    if( _map_viewer->_selected_list.size() > 0 ) {
        _region_data->set_terrain( _map_viewer->_selected_list );
    }
    else {
       wxMessageBox("Il faut au moins selectionner un hex ", "Set Terrain", wxOK, this);
    }
}
void EditorFrame::OnSetRace(wxCommandEvent& event)
{
    if( _map_viewer->_selected_list.size() == 0 ) {
        wxMessageBox("Il faut choisir au moins une region ", "Set Race", wxOK, this);
    }
    else {
        _region_data->set_race( _map_viewer->_selected_list );
    }
}
void EditorFrame::OnTown(wxCommandEvent& event)
{
    if( _map_viewer->_selected_list.size() != 1 ) {
        wxMessageBox("Il faut choisir UNE region ", "Town", wxOK, this);
        return;
    }

    switch( event.GetId()) {
    case idMenuTownAdd:
        _region_data->add_town( _map_viewer->_selected_list.front() );
        break;
    case idMenuTownDel:
        _region_data->del_town( _map_viewer->_selected_list.front() );
        break;
    case idMenuTownRename:
        _region_data->rename_town( _map_viewer->_selected_list.front() );
        break;
    case idMenuTownRegenerate:
        _region_data->add_town( _map_viewer->_selected_list.front() );
        break;
    case idMenuTownMarket:
        wxMessageBox("TODO", "Edit Market", wxOK, this);
        break;
    }
}
