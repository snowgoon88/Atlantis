/**
 * Edit the various ARegion of Atlantis
 */
#include <RegionData.h>

// ******************************************************* RegionData::creation
RegionData::RegionData( ARegionList* regions ) : _regions(regions)
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
// ************************************************ RegionData::get_regionarray
ARegionArray* RegionData::get_regionarray( int id_level )
{
    if( id_level < _regions->numLevels )
        return _regions->pRegionArrays[id_level];
    return nullptr;
}
// **************************************************** RegionData::str_display
std::string RegionData::str_display( ARegionArray* pArr )
{
    return std::string( "RegionData::ARegionArray::str_display");
}

