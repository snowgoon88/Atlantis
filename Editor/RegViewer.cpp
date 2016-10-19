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
    _obj_tree = new wxTreeListCtrl( this, wxID_ANY, wxDefaultPosition, wxSize(300,-1),
                                   wxTL_DEFAULT_STYLE);
    _obj_tree->AppendColumn( "Nom" );
    //_obj_tree->AppendColumn( "Type" );
    //_obj_tree->AppendColumn( "Faction" );
    main_vbox->Add( _obj_tree, 1, wxEXPAND | wxRIGHT | wxALIGN_CENTER_VERTICAL, 2 );

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
    _obj_tree->DeleteAllItems();
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
        _obj_tree->DeleteAllItems();
        wxTreeListItem tree_root = _obj_tree->GetRootItem();
        forlist (&(_reg->objects))
        {
            Object *o = (Object *) elem;
//            std::cout << "[" << o->num << "]";
//            if( o->name) std::cout << " N:" << *(o->name);
//            if( o->describe) std::cout << " D:" << *(o->describe);
//            if( o->region) {
//                std::cout << " R:" << *(o->region->name);
//                std::cout << " (" << o->region->xloc <<","<<o->region->yloc<<","<< o->region->zloc<<")";
//            }
//            if( o->type != -1) std::cout << " T:" << ObjectDefs[o->type].name;
//            std::cout << " U:" << o->units.Num();
//            std::cout << " S:" << o->ships.Num();
//            std::cout << std::endl;
            wxString obj_str, type_str;
            if( o->name )
                obj_str << "O:" << o->name->Str();
            else
                obj_str << "O:---";
            if( o->type != -1)
                obj_str << "[" << ObjectDefs[o->type].name << "]";
            else
                obj_str << "[????]";
            wxTreeListItem tree_obj = _obj_tree->AppendItem( tree_root, obj_str );
            //_obj_tree->SetItemText( tree_obj, 1, type_str);
            if( o->units.Num() > 0 ) {
                forlist (&(o->units)) {
                    Unit *u = (Unit *) elem;
//                    std::cout << "  + Unit:" << *u->name << " (" << u->faction->num << ")";
                    wxString unit_str, fac_str;
                    if( u->name )
                        unit_str << "U:" << u->name->Str();
                    else
                        unit_str << "U:---";
                    if( u->faction->name)
                        fac_str << "(" << u->faction->name->Str() << ")";// (" << u->faction->num << ")";
                    else
                        fac_str << " (????)";
                    wxTreeListItem tree_unit = _obj_tree->AppendItem( tree_obj, unit_str );
                    //_obj_tree->SetItemText( tree_unit, 2, fac_str);
//                    if( u->object ) {
//                        std::cout << " OBJ:";
//                        if( u->object->name) std::cout << " N:" << *(u->object->name);
//                        if( u->object->describe) std::cout << " D:" << *(u->object->describe);
//                        if( u->object->region) {
//                            std::cout << " R:" << *(u->object->region->name);
//                            std::cout << " (" << u->object->region->xloc <<","<<u->object->region->yloc<<","<< u->object->region->zloc<<")";
//                        }
//                        if( u->object->type != -1) std::cout << " T:" << ObjectDefs[u->object->type].name;
//                        std::cout << " U:" << u->object->units.Num();
//                        std::cout << " S:" << u->object->ships.Num();
//                        std::cout << std::endl;
//                    }
                    if( u->items.Num() > 0) {
                        Item *it = (Item *) u->items.First();
                        do {
//                            std::cout << "    + I:";
//                            if( it->type >= 0)
//                                std::cout << ItemDefs[it->type].abr;
//                            std::cout << " (" << it->num << ")";
//                            std::cout << std::endl;

                            wxString item_str, itype_str;
                            if( it->type >= 0 )
                                item_str << "I:" << ItemDefs[it->type].abr;
                            else
                                item_str << "I:???";
                            item_str << " (" << it->num << ")";

                            wxTreeListItem tree_item = _obj_tree->AppendItem( tree_unit, item_str );
                            //_obj_tree->SetItemText( tree_item, 1, itype_str);

                            it = (Item *) u->items.Next( it );
                        } while( it );
                    }
                    std::cout << std::endl;
                }
            }

            std::cout << std::endl;
        }
    }
    //_obj_text->SetLabel( wxString( obj.str() ));
}
Unit* RegViewer::getSelUnit()
{
    wxTreeListItem sel = _obj_tree->GetSelection();
    if( sel.IsOk() ) {
        std::cout << "Tree: sel=" << _obj_tree->GetItemText( sel, 0) << std::endl;
        wxString selstr = _obj_tree->GetItemText( sel, 0);
        if( !selstr.StartsWith("U") )
            return nullptr;

        // Objects
        forlist (&(_reg->objects))
        {
            Object *o = (Object *) elem;
            // Units
            if( o->units.Num() > 0 ) {
                forlist (&(o->units)) {
                    Unit *u = (Unit *) elem;
                    wxString unit_str;
                    if( u->name )
                        unit_str << "U:" << u->name->Str();
                    if( selstr.StartsWith( unit_str )) {
                        return u;
                    }
                }
            }
        }
    }
    return nullptr;
}
Object* RegViewer::getSelObject()
{
    wxTreeListItem sel = _obj_tree->GetSelection();
    if( sel.IsOk() ) {
        std::cout << "Tree: sel=" << _obj_tree->GetItemText( sel, 0) << std::endl;
        wxString selstr = _obj_tree->GetItemText( sel, 0);
        if( !selstr.StartsWith("O") )
            return nullptr;

        // Objects
        forlist (&(_reg->objects))
        {
            Object *o = (Object *) elem;
            wxString obj_str;
            obj_str << "O:" << o->name->Str();
            if( selstr.StartsWith( obj_str )) {
                return o;
            }
        }
    }
    return nullptr;
}
// ****************************************************************************
void mk_title(wxWindow *parent, wxBoxSizer *sizer, std::string title,
              wxStaticText*& wxtext)
{
    wxtext = new wxStaticText( parent, -1, _(title));
    sizer->Add( wxtext, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 2 );
}
