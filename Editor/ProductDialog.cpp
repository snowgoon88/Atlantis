
/**
 * Edit a list of Products.
 */
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <ProductDialog.h>
#include <EditorMain.h>
#include <ListElement.h>
#include <astring.h>

BEGIN_EVENT_TABLE(ProductDialog, wxFrame)
    EVT_CLOSE(ProductDialog::OnClose)
END_EVENT_TABLE()

ProductDialog::ProductDialog( wxWindow* parent, ARegion* reg, MapAccess* map_access)
     : wxFrame( parent, wxID_ANY, "Editor de Products", wxDefaultPosition, wxSize(650,400) ),
     _parent((EditorFrame*)parent), _reg(reg), _map_access(map_access)
{
    wxBoxSizer* main_vbox = new wxBoxSizer( wxVERTICAL );
    // Num
    std::stringstream regstr;
    regstr << "Products de Region " << _reg->num << " ";
    regstr << "[" << _reg->xloc << "," << _reg->yloc << "," << _reg->zloc << "]";
    regstr << " Name: " << *(_reg->name);
    mk_title( this, main_vbox, regstr.str() );

    // Les differents types de produits
    _vec_prod.clear();
    _vec_prod.push_back( {"grain",I_GRAIN});
    //std::cout << "Choice grain with " << I_GRAIN << std::endl;
    _vec_prod.push_back( {"livestock",I_LIVESTOCK});
    //std::cout << "Choice livestock with " << I_LIVESTOCK << std::endl;
    _vec_prod.push_back( {"fish",I_FISH});
    TerrainType* typer = &(TerrainDefs[_reg->type]);
    for (unsigned int c= 0; c < (sizeof(typer->prods)/sizeof(Product)); c++) {
		int item = typer->prods[c].product;
		int chance = typer->prods[c].chance;
		int amt = typer->prods[c].amount;
		if (item != -1) {
			if( !(ItemDefs[item].flags & ItemType::DISABLED)) {
                //std::cout << "Choice " << ItemDefs[item].name << " with " << item << std::endl;
                _vec_prod.push_back( {ItemDefs[item].name,item} );
			}
		}
	}
	// Products
    _list_prod = new ListChooser(this, "Items x number", _vec_prod);
    forlist( (&_reg->products)) {
        Production * p = ((Production *) elem);
        if (p->itemtype != I_SILVER) {
            //std::cout << "Add " << p->amount << " " << ItemDefs[p->itemtype].name << " with " << p->itemtype << std::endl;
            _list_prod->AddElement( p->itemtype, p->amount );
        }
    }
    main_vbox->Add( _list_prod, 1, wxEXPAND | wxRIGHT | wxALIGN_CENTER_VERTICAL, 2);


    // Buttons
    wxBoxSizer* btn_hbox = new wxBoxSizer( wxHORIZONTAL );
    wxButton* save_btn = new wxButton(this, wxOK, "Save");
    save_btn->Bind( wxEVT_BUTTON, &ProductDialog::OnSave, this);
    btn_hbox->Add( save_btn, 0, wxRIGHT|wxLEFT, 20);
    wxButton* cancel_btn = new wxButton(this, wxCANCEL, "Cancel");
    cancel_btn->Bind( wxEVT_BUTTON, &ProductDialog::OnCancel, this);
    btn_hbox->Add( cancel_btn, 0, wxRIGHT|wxLEFT, 20);
    main_vbox->Add( btn_hbox, 0, wxALIGN_CENTER_VERTICAL, 2);


    this->SetSizer( main_vbox );
}

/*************************************************** ProductDialog::Events */
void ProductDialog::OnSave( wxCommandEvent& event)
{
    // Change products
    forlist((&_reg->products)) {
        Production * p = ((Production *) elem);
        if (p->itemtype!=I_SILVER) {
            _reg->products.Remove(p);
            delete p;
        }
    }
//    forlist_reuse((&_reg->products)) {
//        Production * p = ((Production *) elem);
//        std::cout << "Products " << ItemDefs[p->itemtype].name << " : " << p->amount << std::endl;
//    }
    for( auto& item : _list_prod->_elements ) {
        int inum = item->get_type_int();
        int amount = item->get_number();
        //std::cout << "Production of " << amount << " "<< ItemDefs[inum].name << " with " << inum << std::endl;
        Production* p = new Production( inum, amount );
        // as amount is randomly changed, change it back
        p->amount = amount;
        _reg->products.Add(p);
    }

    _parent->_region_data->notify_observers();
    _parent->enable_treeMenu( true );
    Destroy();
}
void ProductDialog::OnCancel( wxCommandEvent& event)
{
    _parent->enable_treeMenu( true );
    Destroy();
}
void ProductDialog::OnClose( wxCloseEvent& event )
{
    _parent->enable_treeMenu( true );
    Destroy();
}
/****************************************** ProductDialog::getUnitLocation */
/****************************************** ProductDialog::getUnitLocations */
