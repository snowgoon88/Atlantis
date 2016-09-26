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
#include <MapViewer.h>

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
END_EVENT_TABLE()

EditorFrame::EditorFrame(wxFrame *frame, const wxString& title)
    : wxFrame(frame, -1, title)
{
#if wxUSE_MENUS
    // create a menu bar
    wxMenuBar* mbar = new wxMenuBar();
    wxMenu* fileMenu = new wxMenu(_T(""));
    fileMenu->Append(idMenuQuit, _("&Quit\tAlt-F4"), _("Quit the application"));
    mbar->Append(fileMenu, _("&File"));

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

//    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
//    frame = new wxFrame((wxFrame *)NULL, -1,  wxT("Hello wxDC"), wxPoint(50,50), wxSize(800,600));

    wxPanel* _main_panel = new wxPanel( this );
    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    MapViewer* map_panel = new MapViewer( _main_panel );

    sizer->Add( map_panel, 1, wxEXPAND);

    _main_panel->SetSizer(sizer);
    //frame->SetAutoLayout(true);

//    frame->Show();
//    return true;
}


EditorFrame::~EditorFrame()
{
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
