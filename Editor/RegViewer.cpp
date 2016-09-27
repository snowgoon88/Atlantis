#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include <RegViewer.h>
#include <sstream>

wxBEGIN_EVENT_TABLE(RegViewer, wxPanel)
wxEND_EVENT_TABLE()

// ****************************************************************************
// ******************************************************** MapViewer::creation
RegViewer::RegViewer(wxWindow* parent) :
    wxPanel(parent), _reg(nullptr)
{
    // main Vertical
    wxBoxSizer *main_vbox = new wxBoxSizer( wxVERTICAL );

    mk_title( this, main_vbox, "Region XXX [-,-,-]", _info_text);
    mk_title( this, main_vbox, "NAME:", _name_text );
    mk_title( this, main_vbox, "TERRAIN:", _terrain_text);
    mk_title( this, main_vbox, "GATE:", _gate_text);
    mk_title( this, main_vbox, "SHAFT:", _shaft_text);
    mk_title( this, main_vbox, "TOWN:", _town_text);
    mk_title( this, main_vbox, "POP:", _pop_text);
    mk_title( this, main_vbox, "OBJ:", _obj_text);

    this->SetSizer( main_vbox );
}
// ********************************************************** RegViewer::attach
void RegViewer::attach( ARegion* reg )
{
    _reg = reg;

    std::stringstream info;
    info << "Region " << reg->num << " ";
    info << "[" << reg->xloc << "," << reg->yloc << "," << reg->zloc << "]";
    _info_text->SetLabel( wxString(info.str() ));

    std::stringstream name;
    name << "NAME: " <<  *(reg->name);
    _name_text->SetLabel( wxString(name.str() ));

    std::stringstream terrain;
    terrain << "TERRAIN: " << TerrainDefs[reg->type].type;
    _terrain_text->SetLabel( wxString(terrain.str() ));

    std::stringstream gate;
    gate << "GATE: ";
    if( reg->gate > 0 ) {
        gate << "G_" << reg->gate;
    }
    else {
        gate << "no";
    }
    _gate_text->SetLabel( wxString( gate.str() ));

    std::stringstream shaft;
    shaft << "SHAFT: ";
    if( reg->HasShaft() ) {
        // Any Shaft ? (only object with inner != -1)
        forlist (&(reg->objects)) {
            Object *o = (Object *) elem;
            if (o->inner != -1) {
                shaft << " S_" << o->inner;
            }
        }
    }
    else {
        shaft << "no";
    }
    _shaft_text->SetLabel( wxString( shaft.str() ));

    std::stringstream town;
    town << "TOWN: ";
    if( reg->town ) {
        town << *(reg->town->name);
    }
    else {
        town << "no";
    }
    _town_text->SetLabel( wxString( town.str() ));

    std::stringstream pop;
    pop << "POP: ";
    if( reg->race != -1 and reg->population > 0 ) {
        pop << reg->population << " "<< ItemDefs[reg->race].names << "(" << reg->race << ")";
    }
    else {
        pop << "-";
    }
	_pop_text->SetLabel( wxString( pop.str() ));

	//std::stringstream obj;
	forlist (&(reg->objects)) {
        Object *o = (Object *) elem;
        std::cout << "[" << o->num << "]";
        if( o->name) std::cout << " N:" << *(o->name);
        if( o->describe) std::cout << " D:" << *(o->describe);
        if( o->type != -1) std::cout << " T:" << ObjectDefs[o->type].name;

        std::cout << std::endl;
    }
    //_obj_text->SetLabel( wxString( obj.str() ));
}
// ****************************************************************************
void mk_title(wxWindow *parent, wxBoxSizer *sizer, std::string title,
              wxStaticText*& wxtext)
{
    wxtext = new wxStaticText( parent, -1, _(title));
    sizer->Add( wxtext, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 2 );
}
