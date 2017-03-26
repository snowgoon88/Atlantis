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
#include <utils.h>
#include <wx/textdlg.h>

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
    //EVT_MENU(idMenuFileOpen, QuestEditorFrame::OnOpenFile)
    EVT_MENU(idMenuMonsterAdd, QuestEditorFrame::OnMonsterAdd)
    //EVT_MENU(idMenuMonsterDebug, QuestEditorFrame::OnMonsterDebug)
    EVT_MENU(idMenuMonsterSave, QuestEditorFrame::OnWriteMonster)
    EVT_MENU(idMenuQuit, QuestEditorFrame::OnQuit)
    EVT_MENU(idMenuAbout, QuestEditorFrame::OnAbout)
END_EVENT_TABLE()

QuestEditorFrame::QuestEditorFrame(wxFrame *frame, const wxString& title, const wxSize& aSize)
    : wxFrame(frame, -1, title, wxDefaultPosition, aSize)
{
//#if wxUSE_MENUS
    // create a menu bar
    wxMenuBar* mbar = new wxMenuBar();
    wxMenu* fileMenu = new wxMenu(_T(""));
    //fileMenu->Append(idMenuFileOpen, _("&Open\tCtrl-O"), _("Open a game file"));
    fileMenu->Append(idMenuQuit, _("&Quit\tAlt-F4"), _("Quit the application"));
    mbar->Append(fileMenu, _("&File"));

    wxMenu* monsterMenu = new wxMenu(_T(""));
    monsterMenu->Append( idMenuMonsterAdd, _("&Add Monster"), ("Ajoute un nouveau Monstre... brrrr....") );
    //monsterMenu->Append( idMenuMonsterDebug, _("&Debug Monster"), ("Denude le Monstre :o)") );
    monsterMenu->Append( idMenuMonsterSave, _("&Write Monsters"), ("Sauvegarde les Monstres dans gamedata.cpp.new") );
    mbar->Append(monsterMenu, _("&Monster"));

    wxMenu* helpMenu = new wxMenu(_T(""));
    helpMenu->Append(idMenuAbout, _("&About\tF1"), _("Show info about this application"));
    mbar->Append(helpMenu, _("&Help"));

    SetMenuBar(mbar);
//#endif // wxUSE_MENUS

//#if wxUSE_STATUSBAR
    // create a status bar with some information about the used wxWidgets version
    CreateStatusBar(2);
    SetStatusText(_("Hello Code::Blocks user!"),0);
    SetStatusText(wxbuildinfo(short_f), 1);
//#endif // wxUSE_STATUSBAR

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

    // Read data about monsters
    read_itemtype_enum();
    //parse_gamedata();
    wxString path = "..";
    /** Look for proper path */
    std::cout << "Path=" << path << std::endl;
    while( !path.IsEmpty() && !wxFileExists( path+"/gamedata.h")) {
        path = wxDirSelector( "Dans quel repertoire est le code source d'Atlantis ?" );
        std::cout << "Path=" << path << std::endl;
    }
    if( path.IsEmpty() ) {
        exit(1);
    }
    _all_data.parse_gamedata( path.ToStdString() );

    _monster_view = new MonsterView( _panel, _all_data );
    vbox->Add( _monster_view );

    _panel->SetSizer( vbox );


}


QuestEditorFrame::~QuestEditorFrame()
{
}

void QuestEditorFrame::OnOpenFile(wxCommandEvent& event)
{
    _textCtrl->AppendText( _("Hop, on ouvre un fichier\n"));

    _monster_view->set_monster( _all_data.find_monster("I_BALROG") );

    //_monster_view->parse_gamedata();
}
void QuestEditorFrame::OnMonsterAdd(wxCommandEvent& event)
{

    wxString unique_str = wxGetTextFromUser( wxString("Il faut un identifiant unique de la forme I_TRUC"),
                                wxString("Nouveau Monstre"),
                                _("I_BIDULE"),
                               this );
    // detect Cancel
    if( unique_str.IsEmpty() ) {
        return;
    }
    bool fg_valid = false;
    while( fg_valid == false ) {
        if( unique_str.substr(0,2).compare("I_") != 0 ) {
            unique_str = wxGetTextFromUser( wxString("Damn: "+unique_str+"', ca commence pas par 'I_', ok ?"),
                                    wxString("Nouveau Monstre"),
                                    unique_str,
                                    this );
        }
        else {
            // look if already in
            for( auto& monster : _all_data._all_monsters ) {
                if( monster.second._item_enum.compare(unique_str) == 0) {
                unique_str = wxGetTextFromUser( wxString("Pas de chance : "+unique_str+" existe deja..."),
                                    wxString("Nouveau Monstre"),
                                    unique_str,
                                    this );
                break;
                }
            }
            fg_valid = true;
        }
    }
    std::cout << "ADD valid new monster" << std::endl;
    AMonster* newmonster = _all_data.make_new_monster( unique_str.ToStdString() );
    _monster_view->add_monster( newmonster );
}
void QuestEditorFrame::OnMonsterDebug(wxCommandEvent& event)
{
    _monster_view->_monster->write_debug();
}
void QuestEditorFrame::OnWriteMonster(wxCommandEvent& even)
{
    _all_data.write_gamedata();
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
