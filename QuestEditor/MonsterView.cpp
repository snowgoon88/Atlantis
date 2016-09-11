#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "MonsterView.h"
#include "items.h"
#include <vector>


wxBEGIN_EVENT_TABLE(MonsterView, wxPanel)
    EVT_COMBOBOX(wxID_ANY, MonsterView::on_cbox_update)
    EVT_TEXT(wxID_ANY, MonsterView::on_cbox_update)
    EVT_TEXT_ENTER(wxID_ANY, MonsterView::on_cbox_update)
wxEND_EVENT_TABLE()

MonsterView::MonsterView(wxWindow *parent)
    : wxPanel( parent )
{
    // main Vertical
    wxBoxSizer *main_vbox = new wxBoxSizer( wxVERTICAL );

    // Identification
    wxBoxSizer *id_hbox = new wxBoxSizer( wxHORIZONTAL );
    wxComboBox *_abbr_combo = new wxComboBox(this, wxID_ANY, _T(""), wxDefaultPosition, wxDefaultSize,
   0, NULL, wxTE_PROCESS_ENTER | wxTE_PROCESS_TAB | wxCB_SORT);
    // all abbr
    std::vector<wxString> _list_abbr;
    for( int i=0; i<NITEMS; ++i) {
            if( ItemDefs[i].type & IT_MONSTER ) {
            _list_abbr.push_back( wxString(ItemDefs[i].abr));
            _abbr_combo->Append( wxString(ItemDefs[i].abr));
            }
    }
    id_hbox->Add( _abbr_combo, 0, wxEXPAND | wxRIGHT | wxALIGN_CENTER_VERTICAL, 20);
    wxStaticText *abbr_label = new wxStaticText( this, -1, _("abbr:"));
    id_hbox->Add( abbr_label, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 2 );
    _abbr_text = new wxTextCtrl( this, -1, "", wxDefaultPosition, wxSize(50,-1));
    id_hbox->Add( _abbr_text, 0, wxEXPAND | wxRIGHT | wxALIGN_CENTER_VERTICAL, 20);
    wxStaticText *name_label = new wxStaticText( this, -1, _("name:"));
    id_hbox->Add( name_label, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 2 );
    _name_text = new wxTextCtrl( this, -1, "", wxDefaultPosition, wxSize(300,-1));
    id_hbox->Add( _name_text, 1, wxEXPAND | wxRIGHT | wxALIGN_CENTER_VERTICAL, 20);
    wxStaticText *names_label = new wxStaticText( this, -1, _("names:"));
    id_hbox->Add( names_label, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 2 );
    _names_text = new wxTextCtrl( this, -1, "-", wxDefaultPosition, wxSize(300,-1));
    id_hbox->Add( _names_text, 1, wxGROW | wxRIGHT | wxALIGN_CENTER_VERTICAL, 20);
    main_vbox->Add( id_hbox, 0, wxEXPAND, 0 );

    // flags
    wxBoxSizer *flags_hbox = new wxBoxSizer( wxHORIZONTAL );
    wxStaticText *flags_label = new wxStaticText( this, -1, _("flags: "));
    flags_hbox->Add( flags_label, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 2 );
    _disabled_check = new wxCheckBox( this, -1, _("Disabled"));
    flags_hbox->Add( _disabled_check, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 20);
    _cantgive_check = new wxCheckBox( this, -1, _("Can't give"));
    flags_hbox->Add( _cantgive_check, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 20);
    main_vbox->Add( flags_hbox, 0, wxEXPAND, 0 );

    // magical skill
    wxBoxSizer *magic_hbox = new wxBoxSizer( wxHORIZONTAL );
    wxStaticText *magic_label = new wxStaticText( this, -1, _("Magical Production: "));
    magic_hbox->Add( magic_label, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 2 );
    wxStaticText *mskill_label = new wxStaticText( this, -1, _("Skill: "));
    magic_hbox->Add( mskill_label, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 20);
    _mSkill_text = new wxTextCtrl( this, -1, "", wxDefaultPosition, wxSize(50,-1));
    magic_hbox->Add( _mSkill_text, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 20);

    //magic_hbox->Add( _cantgive_check, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 20);
    main_vbox->Add( magic_hbox, 0, wxEXPAND, 0 );

    this->SetSizer( main_vbox );

}

// ************************************************ MonsterView::on_cbox_update
void MonsterView::on_cbox_update( wxCommandEvent& event )
{
    // Don't show messages for the log output window (it'll crash)
    if ( !event.GetEventObject()->IsKindOf(CLASSINFO(wxComboBox)) )
        return;

    if ( event.GetEventType() == wxEVT_COMBOBOX )
    {
        std::cout << "EVT_COMBOBOX(id="<< event.GetId() << ",selection=" << event.GetSelection() << std::endl;
    }
    else if ( event.GetEventType() == wxEVT_TEXT )
    {
        std::cout << "EVT_TEXT(id="<< event.GetId() << ",selection=" << event.GetSelection() << std::endl;
    }
    else if ( event.GetEventType() == wxEVT_TEXT_ENTER )
    {
        std::cout << "EVT_TEXT_ENTER(id="<< event.GetId() << ",string=" << event.GetString() << std::endl;
        //wxLogDebug("EVT_TEXT_ENTER(id=%i,string=\"%s\")",
        //            event.GetId(), event.GetString().c_str());
    }
}
// ****************************************************************************
// *************************************************** MonsterView::set_monster
void MonsterView::set_monster( int id )
{
    // Check valid
    if( id < I_LEADERS || id >= NITEMS ) {
        std::cout << "MonsterView::set_monster: " << id << " is not a valid id" << std::endl;
        return;
    }
    ItemType *monster = &(ItemDefs[id]);

    // abr, name, names
    _abbr_text->Clear();
    _abbr_text->AppendText( wxString( monster->abr));
    _name_text->Clear();
    _name_text->AppendText( wxString( monster->name));
    _names_text->Clear();
    _names_text->AppendText( wxString( monster->names));

    // flags
    _disabled_check->SetValue( monster->flags & ItemType::DISABLED );
    _cantgive_check->SetValue( monster->flags & ItemType::CANTGIVE );

    // Magic production
    if( monster->mSkill != NULL ) {
        _mSkill_text->Clear();
        _mSkill_text->AppendText( wxString( monster->mSkill ));
    }
    else {
        _mSkill_text->Clear();
        _mSkill_text->AppendText( _("-") );
    }

}
