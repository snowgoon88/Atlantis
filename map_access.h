/* -*- coding: utf-8 -*- */

#ifndef MAP_ACCESS_H
#define MAP_ACCESS_H

/** 
 * Access the Game object to inspect and modify map.
 */

#include<string>
#include<sstream>
#include<aregion.h>

class MapAccess;
class Game;
// ***************************************************************************
// ***************************************************************** MapAccess
// ***************************************************************************
class MapAccess
{
public:
  // ***************************************************** MapAccess::creation
  MapAccess( Game* game ) : _game(game)
  {
  }
  // ************************************************ MapAccess::AddAbyssLevel
  void AddAbyssLevel( int level )
  {
    // Need more room in ARegionArray
    ARegionArray** tmp_pRegionArray = new ARegionArray *[regions()->numLevels+1];
    // Copy previous ARegionArray
    for (int i=0; i < level; i++) {
      tmp_pRegionArray[i] = _game->regions.pRegionArrays[i];
    }
    for (int i=level; i < _game->regions.numLevels; i++) {
      tmp_pRegionArray[i+1] = _game->regions.pRegionArrays[i];
      // Change the zloc of the regions
      for( int j=0; j < tmp_pRegionArray[i+1]->x * tmp_pRegionArray[i+1]->y /2; j++) {
	ARegion* reg = tmp_pRegionArray[i+1]->regions[j];
	reg->zloc = reg->zloc+1;
      }
    }
    // Set up the new game pRegionArray
    _game->regions.pRegionArrays = tmp_pRegionArray;

    // And add an new abyssal level
    //  - numberofgates++
    //  - one region with gate = -1 (new gate)
    _game->regions.CreateAbyssLevel( level, "abyss" );
    // Set up the new gate.
    // Find the region of the new abyss with the gate
    // and set its number to numberofgates-1
    ARegionArray* regarr = _game->regions.pRegionArrays[level];
    for( int i=0; i < regarr->x * regarr->y /2; i++) {
      ARegion* reg = regarr->regions[i];
      if( reg->gate == -1 ) {
	reg->gate = _game->regions.numberofgates;
      }
    }
    _game->regions.numLevels += 1;
  }
  // ******************************************** MapAccess::AddUnderDeepLevel
  void AddUnderDeepLevel( int level, int sizex, int sizey,
			  int underground_lvl )
  {
    // Need more room in ARegionArray
    ARegionArray** tmp_pRegionArray = new ARegionArray *[regions()->numLevels+1];
    // Copy previous ARegionArray
    for (int i=0; i < level; i++) {
      tmp_pRegionArray[i] = _game->regions.pRegionArrays[i];
    }
    for (int i=level; i < _game->regions.numLevels; i++) {
      tmp_pRegionArray[i+1] = _game->regions.pRegionArrays[i];
      // Change the zloc of the regions
      for( int j=0; j < tmp_pRegionArray[i+1]->x * tmp_pRegionArray[i+1]->y /2; j++) {
	ARegion* reg = tmp_pRegionArray[i+1]->regions[j];
	reg->zloc = reg->zloc+1;
      }
    }
    // Set up the new game pRegionArray
    _game->regions.pRegionArrays = tmp_pRegionArray;

    // And add an new abyssal level
    // from Game::CreateWorld()
    std::cout << "  +CreateUnderdeep" << std::endl;
    _game->regions.CreateUnderdeepLevel(level, sizex, sizey, "underdeep");
    int nbShaft = 0;
    int nbTry = 0;
    while( nbShaft == 0 and nbTry < 2) {
    // shaft to underground only if first underdeep
    if( level == underground_lvl+1 ) {
      std::cout << "  +Connect with Underground" << std::endl;
      _game->regions.MakeShaftLinks( level, level-1, 12);
    }
    // connect to other Underdeep
    for( int i=underground_lvl+2; i<level; i++ ) {
      std::cout << "  +Connect with UnderDeep at " << i << std::endl;
      if( i == level-1 )
	_game->regions.MakeShaftLinks( level, i, 12);
      else
	_game->regions.MakeShaftLinks( level, i, 25);
    }
    nbShaft = nb_shaft(_game->regions.pRegionArrays[level]);
    std::cout << "  +Created " << nb_shaft(_game->regions.pRegionArrays[level])  << " shaft" << std::endl;
    nbTry++;
    }
    // Underdeep has no gates, only the possible shafts above.
    _game->regions.numLevels += 1;

    std::cout << list_regions(_game->regions.pRegionArrays[level]) << std::endl;
  }

  // **************************************************** MapAccess::attributs
  ARegionList* regions() {return &(_game->regions);}
  Game* _game;
  
  // ********************************************************** MapAccess::str
  std::string str_display( ARegionArray* regarr )
  {
    std::stringstream disp;
    disp << "RegionArray ";
    if (regarr->strName) {
      disp << *(regarr->strName);
    } else {
      disp << "none";
    }
    disp << " type=" << regarr->levelType;
    disp << " size=" << regarr->x << " x " << regarr->y << std::endl;
    disp << "  => ";
    // special nexus with only one region
    if( regarr->x * regarr->y /2 == 0 ) {
      ARegion* reg = regarr->regions[0];
      disp << "reg " << reg->num << " (" << reg->xloc << ", " << reg->yloc << ", " << reg->zloc << "),";
    }
    else {
      ARegion* reg = regarr->regions[0];
      disp << "reg " << reg->num << " (" << reg->xloc << ", " << reg->yloc << ", " << reg->zloc << "),";
      reg = regarr->regions[(regarr->x * regarr->y /2)-1];
      disp << "to reg " << reg->num << " (" << reg->xloc << ", " << reg->yloc << ", " << reg->zloc << "),";
    }

    return disp.str();
  }

  std::string str_display( ARegion* reg)
    {
      std::stringstream disp;
      disp << "Reg " << reg->num;
      disp << " : " << TerrainDefs[reg->type].name;
      disp << " (" << reg->xloc << ", " << reg->yloc << ", " << reg->zloc << ")";
      // Any gates ?
      if( reg->gate > 0 ) {
        disp << " G_" << reg->gate;
      }
      // Any Shaft ? (only object with inner != -1)
      forlist (&(reg->objects)) {
        Object *o = (Object *) elem;
        if (o->inner != -1) {
	  disp << " S_" << o->inner;
        }
      }
  
      disp << " =>";
      // neigbors
      for (int i = 0; i < NDIRS; i++) {
        if (reg->neighbors[i]) {
	  disp << reg->neighbors[i]->num << ", ";
        }
        else {
	  disp << -1 << ", ";
        }
      }
      return disp.str();
    }
  // ************************************************* MapAccess::list_regions
  std::string list_regions( ARegionArray* regarr )
  {
    std::stringstream disp;
    
    for( int i=0; i<(regarr->x * regarr->y /2); i++ ) {
      ARegion* reg = regarr->regions[i];
      disp << str_display( reg ) << std::endl;
    }
    return disp.str();
  }
  // ***************************************************** MapAccess::nb_shaft
  int nb_shaft( ARegionArray* regarr )
  {
    int count = 0;
    for( int i=0; i<(regarr->x * regarr->y /2); i++ ) {
      ARegion* reg = regarr->regions[i];
      if( reg->HasShaft() ) count += 1;
    }
    return count;
  }
};
#endif // MAP_ACCESS_H
