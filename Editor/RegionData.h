#ifndef REGIONDATA_H
#define REGIONDATA_H

/**
 * Edit the various ARegion of Atlantis
 */
#include <model.h>

#include <aregion.h>

#include <list>
#include <string>

// ****************************************************************************
class RegionData : public Model
{
public:
    /** Creation */
    RegionData( ARegionList* regions );

    /** Actions */
    void set_name( std::list<ARegion*>& reg_list, const std::string& name);

    /** Get data */
    ARegionArray* get_regionarray( int id_level );

    /** Display */
    std::string str_display( ARegionArray* pArr );
private:
    ARegionList* _regions;
};

#endif // REGIONDATA_H
