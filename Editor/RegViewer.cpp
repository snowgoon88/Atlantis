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
RegViewer::RegViewer(wxWindow* parent, RegionData& model) :
    wxPanel(parent), _model(model), _reg(nullptr)
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
// ********************************************************** RegViewer::update
void RegViewer::update( int signal )
{
    update_values();
}
// *********************************************************** RegViewer::reset
void RegViewer::reset()
{
    _info_text->SetLabel( wxString("Region XXX [-,-,-]"));
    _name_text->SetLabel( wxString("NAME:"));
    _terrain_text->SetLabel( wxString("TERRAIN:"));
    _gate_text->SetLabel( wxString("GATE:"));
    _shaft_text->SetLabel( wxString("SHAFT:"));
    _town_text->SetLabel( wxString("TOWN:"));
    _pop_text->SetLabel( wxString("POP:"));
    _obj_text->SetLabel( wxString("OBJ:"));
}
// ********************************************************** RegViewer::attach
void RegViewer::attach( ARegion* reg )
{
    _reg = reg;
    update_values();
}
void RegViewer::update_values()
{
    if( _reg )
    {
        std::stringstream info;
        info << "Region " << _reg->num << " ";
        info << "[" << _reg->xloc << "," << _reg->yloc << "," << _reg->zloc << "]";
        _info_text->SetLabel( wxString(info.str() ));

        std::stringstream name;
        name << "NAME: " <<  *(_reg->name);
        _name_text->SetLabel( wxString(name.str() ));

        std::stringstream terrain;
        terrain << "TERRAIN: " << TerrainDefs[_reg->type].type;
        _terrain_text->SetLabel( wxString(terrain.str() ));

        std::stringstream gate;
        gate << "GATE: ";
        if( _reg->gate > 0 )
        {
            gate << "G_" << _reg->gate;
        }
        else
        {
            gate << "no";
        }
        _gate_text->SetLabel( wxString( gate.str() ));

        std::stringstream shaft;
        shaft << "SHAFT: ";
        if( _reg->HasShaft() )
        {
            // Any Shaft ? (only object with inner != -1)
            forlist (&(_reg->objects))
            {
                Object *o = (Object *) elem;
                if (o->inner != -1)
                {
                    shaft << " S_" << o->inner;
                }
            }
        }
        else
        {
            shaft << "no";
        }
        _shaft_text->SetLabel( wxString( shaft.str() ));

        std::stringstream town;
        town << "TOWN: ";
        if( _reg->town )
        {
            town << *(_reg->town->name);
        }
        else
        {
            town << "no";
        }
        _town_text->SetLabel( wxString( town.str() ));

        std::stringstream pop;
        pop << "POP: ";
        if( _reg->race != -1 and _reg->population > 0 )
        {
            pop << _reg->population << " "<< ItemDefs[_reg->race].names << "(" << _reg->race << ")";
        }
        else
        {
            pop << "-";
        }
        _pop_text->SetLabel( wxString( pop.str() ));

        //std::stringstream obj;
        forlist (&(_reg->objects))
        {
            Object *o = (Object *) elem;
            std::cout << "[" << o->num << "]";
            if( o->name) std::cout << " N:" << *(o->name);
            if( o->describe) std::cout << " D:" << *(o->describe);
            if( o->type != -1) std::cout << " T:" << ObjectDefs[o->type].name;

            std::cout << std::endl;
        }
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
