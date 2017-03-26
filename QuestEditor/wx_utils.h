#ifndef WXUTILS_H_INCLUDED
#define WXUTILS_H_INCLUDED

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <wx/spinctrl.h>

void mk_field(wxWindow *parent, int id, wxBoxSizer *sizer, std::string title,
              wxTextCtrl*& wxtext, int proportionnal, int width );
void mk_check(wxWindow *parent, int id, wxBoxSizer *sizer, std::string title,
              wxCheckBox*& wxcheck );
void mk_radio(wxWindow *parent, int id, wxBoxSizer *sizer, std::string title,
              wxRadioButton*& wxradio );
void mk_title(wxWindow *parent, wxBoxSizer *sizer, std::string title);
void mk_spin(wxWindow *parent, int id, wxBoxSizer* sizer, std::string title,
             wxSpinCtrl*& wxspin, int rmin, int rmax, int width);

#endif // WXUTILS_H_INCUDED
