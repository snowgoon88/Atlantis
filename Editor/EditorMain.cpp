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

#include <UnitEditorDialog.h>
#include <LocalisationDialog.h>
#include <ObjectDialog.h>
#include <UnitDialog.h>

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
    EVT_MENU(idMenuAddGate, EditorFrame::OnAddGate)
    EVT_MENU(idMenuDelGate, EditorFrame::OnDelGate)
    EVT_MENU(idMenuSwapGate, EditorFrame::OnSwapGate)
    EVT_MENU(idMenuAddShaft, EditorFrame::OnAddShaft)
    EVT_MENU(idMenuRemoveShaft, EditorFrame::OnRemoveShaft)
    EVT_MENU(idMenuSetTerrain, EditorFrame::OnSetTerrain)
    EVT_MENU(idMenuSetRace, EditorFrame::OnSetRace)

    EVT_MENU(idMenuTownAdd, EditorFrame::OnTown)
    EVT_MENU(idMenuTownDel, EditorFrame::OnTown)
    EVT_MENU(idMenuTownRegenerate, EditorFrame::OnTown)
    EVT_MENU(idMenuTownRename, EditorFrame::OnTown)
    EVT_MENU(idMenuTownMarket, EditorFrame::OnTown)

    EVT_MENU(idMenuUnitEdit, EditorFrame::OnUnitEdit)
    EVT_MENU(idMenuUnitAdd, EditorFrame::OnUnitAdd)
    EVT_MENU(idMenuUnitMove, EditorFrame::OnUnitMove)
    EVT_MENU(idMenuUnitDel, EditorFrame::OnUnitDel)

    EVT_MENU(idMenuObjectAdd, EditorFrame::OnObjectAdd )
    EVT_MENU(idMenuObjectDel, EditorFrame::OnObjectDel)

    EVT_MENU(idMenuFindRegion, EditorFrame::OnFindRegion)
    EVT_MENU(idMenuFindUnit, EditorFrame::OnFindUnit)
END_EVENT_TABLE()

EditorFrame::EditorFrame(wxFrame *frame, const wxString& title)
    : wxFrame(frame, -1, title, wxDefaultPosition, wxSize(1000,600)), LocalisationListener()
{
    // Accelerator Key
    _accel_entries[0].Set( wxACCEL_CTRL, (int) 'U', idMenuUnitAdd );
    _accel_entries[1].Set( wxACCEL_CTRL, (int) 'K', idMenuUnitDel );
    _accel_entries[2].Set( wxACCEL_CTRL, (int) 'M', idMenuUnitMove);
    _accel_entries[3].Set( wxACCEL_CTRL, (int) 'E', idMenuUnitEdit);
    _accel_entries[4].Set( wxACCEL_CTRL, (int) 'O', idMenuObjectAdd);
    _accel_entries[5].Set( wxACCEL_CTRL, (int) 'D', idMenuObjectDel);

    _accel_entries[6].Set( wxACCEL_CTRL, (int) 'T', idMenuSetTerrain);
    _accel_entries[7].Set( wxACCEL_CTRL, (int) 'R', idMenuSetRace );

    _accel_entries[8].Set( wxACCEL_CTRL, (int) 'F', idMenuFindUnit );

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
    actMenu->Append(idMenuAddGate, _("&Add Gate"), _("Ajoute une GATE"));
    actMenu->Append(idMenuDelGate, _("&Del Gate"), _("Detruit une GATE"));
    actMenu->Append(idMenuSwapGate, _("&Echange Gate"), _("Echange les no de deux GATE"));
    actMenu->Append(idMenuAddShaft, _("&Add Shaft"), _("Ajoute un SHAFT entre 2 hex"));
    actMenu->Append(idMenuRemoveShaft, _("&Remove Shaft"), _("Enleve un des SHAFT de l'hex"));
    wxMenuItem* terrainMenu = actMenu->Append(idMenuSetTerrain, _("&Set Terrain"), _("Change le type de terrain de 1 ou plusieurs hex"));
    terrainMenu->SetAccel( &_accel_entries[6] );
    wxMenuItem* raceMenu = actMenu->Append(idMenuSetRace, _("&Set Race"), _("Change la race d'un hex"));
    raceMenu->SetAccel( &_accel_entries[7] );
    mbar->Append(actMenu, _("&Action"));

    wxMenu* townMenu = new wxMenu(_T(""));
    townMenu->Append( idMenuTownAdd, _("Add Town"), _("Ajoute une nouvelle ville"));
    townMenu->Append( idMenuTownDel, _("Del Town"), _("Enleve une ville"));
    townMenu->Append( idMenuTownRename, _("Rename Town"), _("Change le nom d'une ville"));
    townMenu->Append( idMenuTownRegenerate, _("Regenerate Town"), _("Recalcule les donnees d'une ville"));
    wxMenuItem* marketMenu = townMenu->Append( idMenuTownMarket, _("Edit Market"), _("Edite les produits du marche"));
    marketMenu->Enable(false);
    mbar->Append(townMenu, _("&Town"));

    wxMenu* treeMenu = new wxMenu(_T(""));
    _addUnitMenu = treeMenu->Append( idMenuUnitAdd, _("Add Unit"), _("Cree une Unit"));
    _addUnitMenu->SetAccel( &_accel_entries[0] );
    _editUnitMenu = treeMenu->Append( idMenuUnitEdit, _("Edit Unit"), _("Modifie une Unit"));
    _editUnitMenu->SetAccel( &_accel_entries[3] );
    _moveUnitMenu = treeMenu->Append( idMenuUnitMove, _("Move Unit"), _("Deplace une Unit"));
    _moveUnitMenu->SetAccel( &_accel_entries[2] );
    _delUnitMenu = treeMenu->Append( idMenuUnitDel, _("Del Unit"), _("Detruit une Unit"));
    _delUnitMenu->SetAccel( &_accel_entries[1] );
    _addObjectMenu = treeMenu->Append( idMenuObjectAdd, _("Add Object"), _("Cree un Object"));
    _addObjectMenu->SetAccel( &_accel_entries[4] );
    _delObjectMenu = treeMenu->Append( idMenuObjectDel, _("Del Object"), _("Detruit un Object (sauf Dummy[None))"));
    _delObjectMenu->SetAccel( &_accel_entries[5] );
    mbar->Append(treeMenu, _("&Objet/Unit"));

    wxMenu* findMenu = new wxMenu(_T(""));
    _findRegMenu = findMenu->Append( idMenuFindRegion, _("Find Region"), _("Trouve une Region"));
    _findUnitMenu = findMenu->Append( idMenuFindUnit, _("Find Unit"), _("Trouve une Unit"));
    mbar->Append( findMenu, _("&Find"));

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
    _moveable_unit = nullptr;
    _selected_reg = nullptr;

    wxAcceleratorTable _accel(9, _accel_entries);
    this->SetAcceleratorTable(_accel);

}
void EditorFrame::enable_treeMenu( bool state )
{
    _editUnitMenu->Enable( state );
    _addUnitMenu->Enable( state );
    _moveUnitMenu->Enable( state );
    _delUnitMenu->Enable( state );
    _addObjectMenu->Enable( state );
    _delObjectMenu->Enable( state );
    _findRegMenu->Enable( state );
    _findUnitMenu->Enable( state );
}

void EditorFrame::load_game()
{
    char chiffres[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
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
void EditorFrame::OnAddGate(wxCommandEvent& event)
{
    if( _map_viewer->_selected_list.size() != 1 ) {
        wxMessageBox("Il faut choisir UNE region", "Add Gate", wxOK, this);
    }
    else {
        _region_data->add_gate( _map_viewer->_selected_list.front() );
    }
}
void EditorFrame::OnDelGate(wxCommandEvent& event)
{
    if( _map_viewer->_selected_list.size() != 1 ) {
        wxMessageBox("Il faut choisir UNE region avec GATE ", "Del Gate", wxOK, this);
    }
    else {
        _region_data->del_gate( _map_viewer->_selected_list.front() );
    }
}
void EditorFrame::OnSwapGate(wxCommandEvent& event)
{
    if( _map_viewer->_selected_list.size() != 1 ) {
        wxMessageBox("Il faut choisir UNE region avec GATE ", "Move Gate", wxOK, this);
    }
    else {
        int gate_num = wxGetNumberFromUser( wxString("Nouveau numero de la porte ?"),
            wxString("Prompt"),
            wxString("Echange Gate"),
            0, 0, 100000, this);
		_region_data->swap_gate( _map_viewer->_selected_list.front(), gate_num );
    }
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
void EditorFrame::OnUnitEdit(wxCommandEvent& event)
{
    Unit* sel_unit = _reg_viewer->getSelUnit();
    if( sel_unit != nullptr) {
        std::cout << "OnTree: got Unit" << std::endl;
        enable_treeMenu( false );
        UnitEditorDialog* dialog = new UnitEditorDialog( this, sel_unit, _map_access );
        //int answer = dialog->ShowModal();
        dialog->Show(true);
    }
    else {
        std::cout << "OnTree: no Unit" << std::endl;
        wxMessageBox("Il faut choisir UNE unit ", "Edit Unit", wxOK, this);
        return;
    }
}
void EditorFrame::OnUnitMove( wxCommandEvent& event )
{
    Unit* sel_unit = _reg_viewer->getSelUnit();
    if( sel_unit != nullptr ) {
        _moveable_unit = sel_unit;
        enable_treeMenu(false);
        LocalisationDialog* loc_d = new LocalisationDialog( this, _map_access, this );
        loc_d->Show(true);
    }
    else {
        _moveable_unit = nullptr;
        wxMessageBox("Il faut choisir UNE unit ", "Move Unit", wxOK, this);
        return;
    }
}
void EditorFrame::OnUnitAdd( wxCommandEvent& event )
{
    if( _map_viewer->_selected_list.size() != 1 ) {
        wxMessageBox("Il faut choisir UNE region", "Add Unit", wxOK, this);
        return;
    }
    else {
        _region_data->create_unit( _map_viewer->_selected_list.front() );
    }
}
void EditorFrame::OnUnitDel( wxCommandEvent& event )
{
    Unit* sel_unit = _reg_viewer->getSelUnit();
    if( sel_unit != nullptr ) {
        int choice = wxMessageBox("Ca va detuire l'unite et ses Items", "Del Unit", wxOK|wxCANCEL, this);
        if( choice == wxOK ) {
            _region_data->del_unit( sel_unit );
        }
    }
    else {
        wxMessageBox("Il faut choisir UNE unit", "Del Unit", wxOK, this);
        return;
    }
}
//{
//    enable_treeMenu(false);
//    LocalisationDialog* loc_d = new LocalisationDialog( this, _map_access, this );
//    loc_d->Show(true);
//}
void EditorFrame::OnObjectAdd( wxCommandEvent& event )
{
    if( _map_viewer->_selected_list.size() != 1 ) {
        wxMessageBox("Il faut choisir UNE region", "Add Object", wxOK, this);
        return;
    }
    else {
        _selected_reg = _map_viewer->_selected_list.front();
        ObjectDialog* dialog = new ObjectDialog( this, _map_access, _selected_reg, this);
        dialog->Show( true );
    }
}
void EditorFrame::OnObjectDel( wxCommandEvent& event )
{
    Object* sel_object = _reg_viewer->getSelObject();
    if( sel_object != nullptr ) {
        int choice = wxMessageBox("Ca va detruire l'object et deplacer ses Units dans O:Dummy[None]",
                                   "Del Object", wxOK|wxCANCEL, this);
        if( choice == wxOK ) {
            _region_data->del_object( sel_object );
        }
    }
    else {
        wxMessageBox("Il faut choisir UN object", "Del Object", wxOK, this);
        return;
    }
}

void EditorFrame::OnFindRegion( wxCommandEvent& event )
{
    _moveable_unit = nullptr;
    enable_treeMenu(false);
    LocalisationDialog* loc_d = new LocalisationDialog( this, _map_access, this );
    loc_d->Show(true);
}
void EditorFrame::OnFindUnit( wxCommandEvent& event )
{
    enable_treeMenu(false);
    UnitDialog* unit_d = new UnitDialog( this, _map_access, this );
    unit_d->Show(true);
}

void EditorFrame::receive_localisation(Localisation& loc)
{
    std::cout << "EditorFrame::receive_localisation" << std::endl;
    // Check not null
    if( loc.first != nullptr ) {
        if( _moveable_unit ) {
            std::cout << "Received loc => should move Unit there" << std::endl;
            _region_data->move_unit( _moveable_unit, loc.second );
            enable_treeMenu(true);
        }
        else {
            std::cout << "Received loc => center on given location" << std::endl;
            // Set up the right region
            //    MapAccess map_access = MapAccess( _game );
            ARegionList* regions = _map_access->regions();
            ARegionArray *pArr = nullptr;
            if( loc.first->zloc < regions->numLevels) {
                pArr = regions->pRegionArrays[loc.first->zloc];
            }
            if( pArr ) {
                if( pArr->strName )
                    std::cout << "ATTACH name=" << *(pArr->strName) << std::endl;
            _map_viewer->attach( pArr );
            }
            // Focus on given hex
            _map_viewer->FocusOn( loc.first );

            enable_treeMenu(true);
        }
//        Unit* newunit = _map_access->create_unit( loc.first, loc.second );
//        enable_treeMenu( false );
//        UnitEditorDialog* dialog = new UnitEditorDialog( this, newunit, _map_access );
//        //int answer = dialog->ShowModal();
//        dialog->Show(true);
    }
    else {
        std::cout << "Received nullLOC " << std::endl;
        wxMessageBox("Pas de destination choisie", "Move Unit", wxOK, this);
        enable_treeMenu(true);
    }
}
void EditorFrame::receive_unitedit()
{
    _region_data->edit_unit();
    enable_treeMenu(true);
}
void EditorFrame::receive_object( int type )
{
    if( type > 0 ) {
        _region_data->create_object( _selected_reg, type );
    }
    _selected_reg = nullptr;
}
void EditorFrame::receive_unit(Unit* unit)
{
    if( unit != nullptr ) {
        ARegion* reg = unit->object->region;
        ARegionList* regions = _map_access->regions();
        ARegionArray *pArr = nullptr;
        if( reg->zloc < regions->numLevels) {
            pArr = regions->pRegionArrays[reg->zloc];
        }
        if( pArr ) {
            if( pArr->strName )
                std::cout << "ATTACH name=" << *(pArr->strName) << std::endl;
            _map_viewer->attach( pArr );
        }
        // Focus on given hex
        _map_viewer->FocusOn( reg );

        enable_treeMenu(true);
    }
    else {
        std::cout << "Received nullUnit " << std::endl;
        wxMessageBox("Pas de Unit trouvee", "Find Unit", wxOK, this);
        enable_treeMenu(true);
    }
}
