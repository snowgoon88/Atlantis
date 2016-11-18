#ifndef REGIONDATA_H
#define REGIONDATA_H

/**
 * Edit the various ARegion of Atlantis
 */
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <model.h>

#include <aregion.h>
#include <map_access.h>

#include <list>
#include <string>
#include <vector>

extern std::vector<int> _terrain_list;
// ****************************************************************************
class RegionData : public Model
{
public:
    /** Creation */
    RegionData( wxWindow* parent, ARegionList* regions, MapAccess* map_access );

    /** Actions */
    void set_name( std::list<ARegion*>& reg_list, const std::string& name);
    void move_gate( ARegion* reg, int dest_reg_num );
    void add_gate( ARegion* reg );
    void del_gate( ARegion* reg );
    void swap_gate( ARegion* reg, int gate_num );
    void add_shaft( ARegion* reg, int dest_reg_num );
    void remove_shaft( ARegion* reg );
    void set_terrain( std::list<ARegion*>& reg_list );
    void set_race( std::list<ARegion*>& reg_list );
    void renew_products( ARegion* reg );
    void set_wages( ARegion* reg, int wages );
    void add_town( ARegion* reg );
    void del_town( ARegion* reg);
    void rename_town( ARegion*reg);
    /** Unit */
    void create_unit( ARegion* reg );
    void move_unit( Unit* unit, Object* obj);
    void edit_unit();
    void del_unit( Unit* unit );
    /** Object */
    void create_object( ARegion* reg, int type);
    void del_object( Object *obj );

    /** Get data */
    ARegionArray* get_regionarray( int id_level );
    ARegion* find( int num );

    /** Display */
    std::string str_display( ARegionArray* pArr );
private:
    wxWindow* _parent;
    ARegionList* _regions;
    MapAccess* _map_access;

};

#endif // REGIONDATA_H
