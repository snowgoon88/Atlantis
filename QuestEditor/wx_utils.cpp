#include <wx_utils.h>

// ******************************************************************** mk_field
void mk_field(wxWindow *parent, int id, wxBoxSizer *sizer, std::string title,
              wxTextCtrl*& wxtext, int proportionnal, int width )
{
    wxStaticText *label = new wxStaticText( parent, wxID_ANY, _(title));
    sizer->Add( label, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 2 );
    wxtext = new wxTextCtrl( parent, id, "", wxDefaultPosition, wxSize(width,-1), wxTE_PROCESS_ENTER);
    sizer->Add( wxtext, proportionnal, wxEXPAND | wxRIGHT | wxALIGN_CENTER_VERTICAL, 20);
}
void mk_check(wxWindow *parent, int id, wxBoxSizer *sizer, std::string title,
              wxCheckBox*& wxcheck )
{
    wxcheck = new wxCheckBox( parent, id, _(title));
    sizer->Add( wxcheck, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 20);
}
void mk_radio(wxWindow *parent, int id, wxBoxSizer *sizer, std::string title,
              wxRadioButton*& wxradio )
{
    wxradio = new wxRadioButton( parent, id, _(title));
    sizer->Add( wxradio, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 20);
}
void mk_title(wxWindow *parent, wxBoxSizer *sizer, std::string title)
{
    wxStaticText *label = new wxStaticText( parent, -1, _(title));
    sizer->Add( label, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 2 );
}
void mk_spin(wxWindow *parent, int id, wxBoxSizer* sizer, std::string title,
             wxSpinCtrl*& wxspin, int rmin, int rmax, int width)
{
    wxStaticText *label = new wxStaticText( parent, -1, _(title));
    sizer->Add( label, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 2);
    wxspin = new wxSpinCtrl( parent, id, "", wxDefaultPosition, wxSize(width,-1), wxTE_PROCESS_ENTER);
    wxspin->SetRange(rmin,rmax);
    wxspin->SetValue( 0 );
    sizer->Add( wxspin, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 20);
}
