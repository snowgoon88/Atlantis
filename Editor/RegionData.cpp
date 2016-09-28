/**
 * Edit the various ARegion of Atlantis
 */
 #ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#include <RegionData.h>
#include <sstream>
#include <gamedata.h>
#include <vector>

std::vector<int> _terrain_list = {R_OCEAN,R_PLAIN,R_FOREST,R_MOUNTAIN,R_SWAMP,
        R_JUNGLE,R_DESERT,R_TUNDRA,R_CAVERN,R_UFOREST,R_TUNNELS,
        R_GROTTO,R_DFOREST,R_CHASM,R_LAKE};

// ******************************************************* RegionData::creation
RegionData::RegionData( wxWindow* parent, ARegionList* regions, MapAccess* map_access ) :
    _parent(parent), _regions(regions), _map_access(map_access)
{
}
// ****************************************************************************
// ******************************************************* RegionData::set_name
void RegionData::set_name( std::list<ARegion*>& reg_list,
                           const std::string& name)
{
    for( auto& reg : reg_list ) {
        reg->SetName(name.c_str() );
    }
    notify_observers();
}
// ****************************************************** RegionData::move_gate
void RegionData::move_gate( ARegion* reg, int dest_reg_num )
{
    // A Gate to move
    if( reg->gate < 1 ) {
        wxMessageBox("Pas de GATE dans region selectionnee ", "Move Gate", wxOK, _parent);
    }
    else {
        ARegion* reg_dest = find( dest_reg_num );
        if( reg_dest == nullptr ) {
            std::stringstream msg;
            msg << "Je ne trouve pas la region no " << dest_reg_num;
            wxMessageBox(wxString(msg.str()), "Move Gate", wxOK, _parent);
            return;
        }
        else if( TerrainDefs[reg_dest->type].similar_type == R_OCEAN ) {
            std::stringstream msg;
            msg << "La region no " << dest_reg_num << " est dans l'eau... Pas de Gate !!!";
            wxMessageBox(wxString(msg.str()), "Move Gate", wxOK, _parent);
            return;
        }
        else if( reg_dest->gate > 0) {
            std::stringstream msg;
            msg << "La region no " << dest_reg_num << " a deja une Gate. Dommage !!!";
            wxMessageBox(wxString(msg.str()), "Move Gate", wxOK, _parent);
            return;
        }
        else {
            reg_dest->gate = reg->gate;
            reg->gate = 0;
            notify_observers();

            std::stringstream msg;
            msg << "Tranfert reussi... Bravo !";
            wxMessageBox(wxString(msg.str()), "Move Gate", wxOK, _parent);
            return;
        }
    }
}
void RegionData::add_shaft( ARegion* reg, int dest_reg_num )
{
    // Region is not ocean
    if (TerrainDefs[reg->type].similar_type == R_OCEAN) {
        std::stringstream msg;
        msg << "La region initiale no " << reg->num << " est dans l'eau... Pas de Shaft !!!";
        wxMessageBox(wxString(msg.str()), "Add Shaft", wxOK, _parent);
        return;
    }
    // Look for other region
    ARegion* reg_dest = find( dest_reg_num );
    if( reg_dest == nullptr ) {
        std::stringstream msg;
        msg << "Je ne trouve pas la region no " << dest_reg_num;
        wxMessageBox(wxString(msg.str()), "Add Shaft", wxOK, _parent);
        return;
    }
    else if( TerrainDefs[reg_dest->type].similar_type == R_OCEAN ) {
        std::stringstream msg;
        msg << "La region no " << dest_reg_num << " est dans l'eau... Pas de Shaft !!!";
        wxMessageBox(wxString(msg.str()), "Add Shaft", wxOK, _parent);
        return;
    }
    Object *o = new Object(reg);
	o->num = reg->buildingseq++;
	o->name = new AString(AString("Shaft [") + o->num + "]");
	o->type = O_SHAFT;
	o->incomplete = 0;
	o->inner = reg_dest->num;
	reg->objects.Add(o);

	o = new Object(reg);
	o->num = reg_dest->buildingseq++;
	o->name = new AString(AString("Shaft [") + o->num + "]");
	o->type = O_SHAFT;
	o->incomplete = 0;
	o->inner = reg->num;
	reg_dest->objects.Add(o);

	notify_observers();

    std::stringstream msg;
    msg << "Creation reussie... Ca rocke !";
    wxMessageBox(wxString(msg.str()), "Add Shaft", wxOK, _parent);
}
void RegionData::remove_shaft( ARegion* reg )
{
    if( !reg->HasShaft() ) {
        std::stringstream msg;
        msg << "La region no " << reg->num << " n'a pas de SHAFT => mission accomplie :o)";
        wxMessageBox(wxString(msg.str()), "Remove Shaft", wxOK, _parent);
        return;
    }
    // list of Shaft
    std::vector<Object*> _shaft_list;
    forlist (&(reg->objects))
    {
        Object *o = (Object *) elem;
        if (o->inner != -1) {
            _shaft_list.push_back(o);
        }
    }
//    std::cout << "FOUND " << _shaft_list.size() << " shafts" << std::endl;
    // Choose which one to delete
    wxString _obj_str[_shaft_list.size()];
    //wsString _choice_str[_shaft_list.size()];
    for( int i=0; i<_shaft_list.size(); ++i ) {
        _obj_str[i] << "SHAFT["<<i<<"] de "<<reg->num<<" a "<<_shaft_list[i]->inner;
    }
    int choice = wxGetSingleChoiceIndex( wxString("Quel SHAFT veux-tu detruire?"),
                        wxString("Remove Shaft"),
                        (int)_shaft_list.size(), _obj_str, _parent);
    if( choice >= 0) {
        std::cout << "Destroying " << _obj_str[choice].ToStdString() << std::endl;
        // find other end of shaft
        ARegion* reg_dest = find( _shaft_list[choice]->inner );
        if( reg_dest == nullptr ) {
            std::stringstream msg;
            msg << "Je ne trouve pas la region a l'autre bout du Shaft, region no " << _shaft_list[choice]->inner;
            wxMessageBox(wxString(msg.str()), "Remove Shaft", wxOK, _parent);
            return;
        }
        Object* other_shaft = nullptr;
        // find other shaft
        forlist (&(reg_dest->objects))
        {
            Object *o = (Object *) elem;
            if (o->inner == reg->num) {
                other_shaft = o;
            }
        }
        if( other_shaft == nullptr ) {
            std::stringstream msg;
            msg << "Je ne trouve pas bout du Shaft dans la region no " << _shaft_list[choice]->inner;
            wxMessageBox(wxString(msg.str()), "Remove Shaft", wxOK, _parent);
            return;
        }
        reg->objects.Remove(_shaft_list[choice]);
        reg_dest->objects.Remove( other_shaft );
        notify_observers();

        std::stringstream msg;
        msg << "Destruction sans faille de " << _obj_str[choice].ToStdString();
        wxMessageBox(wxString(msg.str()), "Remove Shaft", wxOK, _parent);
    }

}
void RegionData::set_terrain( std::list<ARegion*>& reg_list )
{
    // Which kind of terrain
    wxString _ter_str[_terrain_list.size()];
    //wsString _choice_str[_shaft_list.size()];
    for( int i=0; i<_terrain_list.size(); ++i ) {
        _ter_str[i] << TerrainDefs[_terrain_list[i]].type;
    }
    int choice = wxGetSingleChoiceIndex( wxString("Quel type de terrain ?\nATTENTION, on recalcule tout sur les hex (race,ville,products...)"),
                        wxString("Set Terrain"),
                        (int)_terrain_list.size(), _ter_str, _parent);
    if( choice != -1 ) {
        std::cout << "SET TERRAIN to " << _ter_str[choice].ToStdString() << std::endl;
        for( auto& reg : reg_list ) {
            reg->type = _terrain_list[choice];
            _map_access->recompute_hex( reg );
        }
        notify_observers();
    }
}
void RegionData::set_race( ARegion* reg )
{
    // Which kind of race
    int nb_race=0;
    wxString _race_str[NITEMS];
    //wsString _choice_str[_shaft_list.size()];
    for( int id_item=0; id_item<NITEMS; ++id_item ) {
        if( (ItemDefs[id_item].type & IT_MAN) && !(ItemDefs[id_item].flags & ItemType::DISABLED)) {
            _race_str[nb_race] << ItemDefs[id_item].name << " [" << ItemDefs[id_item].abr << "]";
            nb_race++;
        }
    }
    int choice = wxGetSingleChoiceIndex( wxString("Quel type de race ?"),
                        wxString("Set Race"),
                        nb_race, _race_str, _parent);
    if( choice != -1 ) {
        reg->race = choice;
        reg->UpdateEditRegion();
        notify_observers();
    }
//    if (!(ItemDefs[prace].type & IT_MAN) || (ItemDefs[prace].flags & ItemType::DISABLED) ) {
//						if (!(*pToken == "none" || *pToken == "None" || *pToken == "0")) {
//							Awrite( "No such race." );
//							break;
//						} else {
//							prace = -1;
//						}
//					}
//					if (prace != 0) pReg->race = prace;
//					pReg->UpdateEditRegion();
}
// ************************************************ RegionData::get_regionarray
ARegionArray* RegionData::get_regionarray( int id_level )
{
    if( id_level < _regions->numLevels )
        return _regions->pRegionArrays[id_level];
    return nullptr;
}
ARegion* RegionData::find( int num )
{
    for( int id_level=0; id_level<_regions->numLevels; ++id_level ) {
        ARegionArray* pArr = get_regionarray( id_level );
        for( int i=0; i<(pArr->x * pArr->y /2); i++ ) {
            ARegion* reg = pArr->regions[i];
            if( reg->num == num ) {
                return reg;
            }
        }
    }
    return nullptr;
}
// **************************************************** RegionData::str_display
std::string RegionData::str_display( ARegionArray* pArr )
{
    return std::string( "RegionData::ARegionArray::str_display");
}

