#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include <RegViewer.h>
#include <sstream>
#include <ProductDialog.h>
#include <EditorMain.h>

wxBEGIN_EVENT_TABLE(RegViewer, wxPanel)
wxEND_EVENT_TABLE()

// ****************************************************************************
// ******************************************************** MapViewer::creation
RegViewer::RegViewer(wxWindow* parent, RegionData& model, MapAccess* map_access, EditorFrame* frame) :
    wxPanel(parent), _parent(frame), _model(model), _map_access(map_access), _reg(nullptr)
{
    // main Vertical
    wxBoxSizer *main_vbox = new wxBoxSizer( wxVERTICAL );
    mk_title( this, main_vbox, "Region XXX [-,-,-] Name: ", _info_text);
    //mk_title( this, reg_hbox, "NAME:", _name_text );
    mk_title( this, main_vbox, "TERRAIN: - POP: -", _terrain_text);
    //mk_title( this, ter_hbox, "basePOP:", _pop_text);
    mk_title( this, main_vbox, "GATE: - SHAFT:-", _gate_text);
    //mk_title( this, port_hbox, "SHAFT:", _shaft_text);
    mk_title( this, main_vbox, "Wages: - (max) ENTERTAIN: -", _earn_text);

    wxBoxSizer* prod_hbox = new wxBoxSizer( wxHORIZONTAL );
    mk_title( this, prod_hbox, "Prod: ", _prod_text);
    wxButton* rnd_btn = new wxButton( this, wxID_ANY, "Rand");
    rnd_btn->Bind( wxEVT_BUTTON, &RegViewer::OnRenewProducts, this);
    prod_hbox->Add( rnd_btn, 0, wxALIGN_CENTER_VERTICAL, 0);
    wxButton* edit_btn = new wxButton( this, wxID_ANY, "Edit");
    edit_btn->Bind( wxEVT_BUTTON, &RegViewer::OnEditProducts, this);
    prod_hbox->Add( edit_btn, 0, wxALIGN_CENTER_VERTICAL, 0);
    main_vbox->Add( prod_hbox, 0, wxALIGN_CENTER_VERTICAL, 2);

    _prod_scrollpane = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                    wxTAB_TRAVERSAL|wxVSCROLL);
    _prod_scrollpane->SetMinSize( wxSize(50,100));
    _prod_vbox = new wxBoxSizer( wxVERTICAL );
    _prod_scrollpane->SetSizer( _prod_vbox );

    _prod_scrollpane->SetScrollRate(5, 5);
    main_vbox->Add( _prod_scrollpane, 0, wxEXPAND | wxLEFT, 25);
    mk_title( this, main_vbox, "TOWN:", _town_text);
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
    _info_text->SetLabel( wxString("Region XXX [-,-,-] Name: "));
    //_name_text->SetLabel( wxString("NAME:"));
    _terrain_text->SetLabel( wxString("TERRAIN: - POP: -"));
    _gate_text->SetLabel( wxString("GATE: - SHAFT:-"));
    //_shaft_text->SetLabel( wxString("SHAFT:"));
    _earn_text->SetLabel( wxString("Wages: - (max) ENTERTAIN: -"));
    //_prod_text->SetLabel( wxString("Prod: -"));
    _prod_vbox->Clear( true );
    _town_text->SetLabel( wxString("TOWN:"));
    //_pop_text->SetLabel( wxString("POP:"));
    //_obj_text->SetLabel( wxString("OBJ:"));
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
        info << " Name: " << *(_reg->name);
        _info_text->SetLabel( wxString(info.str() ));

//        std::stringstream name;
//        name << "NAME: " <<  *(_reg->name);
//        _name_text->SetLabel( wxString(name.str() ));

        std::stringstream terrain;
        terrain << "TERRAIN: " << TerrainDefs[_reg->type].type;
        terrain << " POP: ";
        if( _reg->race != -1 and _reg->population > 0 )
        {
            terrain << _reg->population << " "<< ItemDefs[_reg->race].names << "(" << _reg->race << ")";
        }
        else
        {
            terrain << "-";
        }
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
        gate << " SHAFT: ";
        if( _reg->HasShaft() )
        {
            // Any Shaft ? (only object with inner != -1)
            forlist (&(_reg->objects))
            {
                Object *o = (Object *) elem;
                if (o->inner != -1)
                {
                    gate << " S_" << o->inner;
                }
            }
        }
        else
        {
            gate << "no";
        }
        _gate_text->SetLabel( wxString( gate.str() ));

//        std::stringstream shaft;
//        shaft << "SHAFT: ";
//        if( _reg->HasShaft() )
//        {
//            // Any Shaft ? (only object with inner != -1)
//            forlist (&(_reg->objects))
//            {
//                Object *o = (Object *) elem;
//                if (o->inner != -1)
//                {
//                    shaft << " S_" << o->inner;
//                }
//            }
//        }
//        else
//        {
//            shaft << "no";
//        }
//        _shaft_text->SetLabel( wxString( shaft.str() ));

        std::stringstream wages;
        wages << "Wages: " << _reg->WagesForReport();

        _prod_vbox->Clear( true );
        forlist( &(_reg->products)) {
			Production * p = ((Production *) elem);
			if (ItemDefs[p->itemtype].type & IT_ADVANCED) {
			    std::stringstream prod;
                prod << p->WriteReport();
                wxStaticText* p_text;
                mk_title( _prod_scrollpane, _prod_vbox, prod.str(), p_text );
			} else {
				if (p->itemtype == I_SILVER) {
					if (p->skill == S_ENTERTAINMENT) {
                        wages << " ENTERTAIN: " << p->amount << "$";
					}
				} else {
					std::stringstream prod;
                prod << p->WriteReport();
                wxStaticText* p_text;
                mk_title( _prod_scrollpane, _prod_vbox, prod.str(), p_text );
				}
			}
		}
		this->Layout();
        this->FitInside();
    //_elem_vbox->Fit(this);
        this->PostSizeEvent();
		_earn_text->SetLabel( wxString( wages.str() ));
		//_prod_text->SetLabel( wxString( prod.str() ));

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
        if (_reg->town) {
            town << " [Pop: " << _reg->town->pop << " " << ItemDefs[_reg->race].names << "]";
        }
        _town_text->SetLabel( wxString( town.str() ));

//        std::stringstream pop;
//        pop << "POP: ";
//        if( _reg->race != -1 and _reg->population > 0 )
//        {
//            pop << _reg->population << " "<< ItemDefs[_reg->race].names << "(" << _reg->race << ")";
//        }
//        else
//        {
//            pop << "-";
//        }
//        _pop_text->SetLabel( wxString( pop.str() ));

        //std::stringstream obj;
        _obj_tree->DeleteAllItems();
        wxTreeListItem tree_root = _obj_tree->GetRootItem();
        forlist_reuse (&(_reg->objects))
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
                        unit_str << " =>F:" << u->faction->name->Str();// (" << u->faction->num << ")";
                    else
                        unit_str << " =>F:????";
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

void RegViewer::OnRenewProducts(wxCommandEvent& event)
{
    if( _reg )
        _model.renew_products( _reg );
}
void RegViewer::OnEditProducts( wxCommandEvent& event )
{
    if( _reg ) {
        _parent->enable_treeMenu( false );
        ProductDialog* pdiag = new ProductDialog( _parent, _reg, _map_access );
        pdiag->Show(true);
    }

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
