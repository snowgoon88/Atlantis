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
    EVT_MENU(idMenuSurface, EditorFrame::OnRegion)
    EVT_MENU(idMenuUnderWorld, EditorFrame::OnRegion)
    EVT_MENU(idMenuUnderDeep, EditorFrame::OnRegion)
    EVT_MENU(idMenuChangeName, EditorFrame::OnChangeName)
END_EVENT_TABLE()

EditorFrame::EditorFrame(wxFrame *frame, const wxString& title)
    : wxFrame(frame, -1, title)
{
#if wxUSE_MENUS
    // create a menu bar
    wxMenuBar* mbar = new wxMenuBar();
    wxMenu* fileMenu = new wxMenu(_T(""));
    fileMenu->Append(idMenuSurface, _("&Surface"), _("Display region"));
    fileMenu->Append(idMenuUnderWorld, _("&UnderWorld"), _("Display region"));
    fileMenu->Append(idMenuUnderDeep, _("&UnderDeep"), _("Display region"));
    fileMenu->Append(idMenuQuit, _("&Quit\tAlt-F4"), _("Quit the application"));
    mbar->Append(fileMenu, _("&File"));

    wxMenu* actMenu = new wxMenu(_T(""));
    actMenu->Append(idMenuChangeName, _("&Change Name"), _("Change le nom de 1 ou plusieurs hex"));
    mbar->Append(actMenu, _("&Action"));

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

    // Essai pour game
    //Game game;

    initIO();

    _game.ModifyTablesPerRuleset();

    if ( !_game.OpenGame() ) {
        Awrite( "Couldn't open the game file!" );
        exit(1);
    }
    MapAccess mapAccess = MapAccess( &_game );
    ARegionList* regions = mapAccess.regions();
    _region_data = new RegionData( regions );

    // list all the levels
    for (int i = 0; i < regions->numLevels; i++) {
        ARegionArray *pArr = regions->pRegionArrays[i];
        std::cout << "{"<<i<<"} " << mapAccess.str_display( pArr, false) << std::endl;
        if( pArr->strName )
            std::cout << "          name=" << *(pArr->strName) << std::endl;
    }


    wxPanel* _main_panel = new wxPanel( this );
    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    _map_viewer = new MapViewer( _main_panel, *_region_data );
    _region_data->attach_observer( _map_viewer );
    sizer->Add( _map_viewer, 1, wxEXPAND);
    _reg_viewer = new RegViewer( _main_panel, *_region_data );
    _region_data->attach_observer( _reg_viewer );
    _map_viewer->attach_regviewer( _reg_viewer );
    sizer->Add( _reg_viewer, 0, wxEXPAND | wxLEFT | wxRIGHT, 5);

    _main_panel->SetSizer(sizer);
    //map_panel->attach( regions->pRegionArrays[1] );
    //frame->SetAutoLayout(true);

//    frame->Show();
//    return true;
}


EditorFrame::~EditorFrame()
{
}

void EditorFrame::OnRegion( wxCommandEvent& event)
{
    MapAccess map_access = MapAccess( &_game );
    ARegionList* regions = map_access.regions();
    ARegionArray *pArr;
    if( event.GetId() == idMenuSurface ) {
        pArr = regions->pRegionArrays[1];
    }
    if( event.GetId() == idMenuUnderWorld ) {
        pArr = regions->pRegionArrays[2];
    }
    if( event.GetId() == idMenuUnderDeep ) {
        pArr = regions->pRegionArrays[3];
    }
    if( pArr->strName )
        std::cout << "ATTACH name=" << *(pArr->strName) << std::endl;
    _map_viewer->attach( pArr );
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
