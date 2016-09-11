/***************************************************************
 * Name:      QuestEditorMain.cpp
 * Purpose:   Code for Application Frame
 * Author:     (snowgoon88@gmail.com)
 * Created:   2016-09-07
 * Copyright:  ()
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "QuestEditorMain.h"
#include "MonsterView.h"

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

BEGIN_EVENT_TABLE(QuestEditorFrame, wxFrame)
    EVT_CLOSE(QuestEditorFrame::OnClose)
    EVT_MENU(idMenuFileOpen, QuestEditorFrame::OnOpenFile)
    EVT_MENU(idMenuQuit, QuestEditorFrame::OnQuit)
    EVT_MENU(idMenuAbout, QuestEditorFrame::OnAbout)
END_EVENT_TABLE()

QuestEditorFrame::QuestEditorFrame(wxFrame *frame, const wxString& title, const wxSize& aSize)
    : wxFrame(frame, -1, title, wxDefaultPosition, aSize)
{
#if wxUSE_MENUS
    // create a menu bar
    wxMenuBar* mbar = new wxMenuBar();
    wxMenu* fileMenu = new wxMenu(_T(""));
    fileMenu->Append(idMenuFileOpen, _("&Open\tCtrl-O"), _("Open a game file"));
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

    _panel = new wxPanel( this );
    wxBoxSizer *vbox = new wxBoxSizer( wxVERTICAL );

    // And add a Text - multiline and readonlu
//    _textCtrl = new wxTextCtrl( _panel, -1, _("exemple de texte"),
//                               wxDefaultPosition, wxDefaultSize,
//                               wxTE_MULTILINE | wxTE_READONLY );
//    vbox->Add( _textCtrl, 1, wxEXPAND );
//    vbox->Add( -1, 20); // Some space
//    wxBoxSizer *hbox = new wxBoxSizer( wxHORIZONTAL );
//    _label = new wxStaticText( _panel, -1, _("LABEL"));
//    hbox->Add( _label, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 10 );
//    hbox->Add( new wxTextCtrl(_panel, -1, _("A modifier")), 1, wxEXPAND | wxALIGN_CENTER_VERTICAL);
//    vbox->Add( hbox );

    _monster_view = new MonsterView( _panel );
    vbox->Add( _monster_view );

    _panel->SetSizer( vbox );
}


QuestEditorFrame::~QuestEditorFrame()
{
}

void QuestEditorFrame::OnOpenFile(wxCommandEvent& event)
{
    _textCtrl->AppendText( _("Hop, on ouvre un fichier\n"));

    _monster_view->set_monster( I_BALROG );

    _monster_view->parse_gamedata();
}

void QuestEditorFrame::OnClose(wxCloseEvent &event)
{
    Destroy();
}

void QuestEditorFrame::OnQuit(wxCommandEvent &event)
{
    Destroy();
}

void QuestEditorFrame::OnAbout(wxCommandEvent &event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}
