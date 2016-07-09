/* -*- coding: utf-8 -*- */

#ifndef MAP_ACCESS_H
#define MAP_ACCESS_H

/** 
 * Access the Game object to inspect and modify map.
 */

#include <string>
#include <sstream>
#include <aregion.h>
#include <gamedata.h>
#include <gameio.h>

#include <algorithm>    // std::random_shuffle
#include <vector>       // std::vector

void SetupNames();

class MapAccess;
class Game;
class ARegionList;
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
  // ******************************************* MapAccess::AddUnderWorldLevel
  /**
   * Si underworld_level == 0 => premier niveau de underworld.
   */
  void AddUnderWorldLevel( int level, int xSize, int ySize,
			   int underworld_level )
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
    // and Prepare named array... dans world.cpp
    SetupNames();

    // Create new region
    // ARegionList::MakeRegions(int level, int xSize, int ySize)
    _game->regions.MakeRegions( level, xSize, ySize );
    ARegionArray* aregarr = _game->regions.pRegionArrays[level];
    aregarr->SetName("underWorld");
    aregarr->levelType = ARegionArray::LEVEL_UNDERWORLD;
    // Set terrains
    _game->regions.SetRegTypes( aregarr, R_NUM);
    SetupAnchorsUnderworld( aregarr );
    GrowTerrainUnderworld( aregarr, 1);
    RandomTerrainUnderworld( aregarr );
    _game->regions.MakeUWMaze( aregarr );
    if (Globals->GROW_RACES)
      _game->regions.GrowRaces( aregarr );
    _game->regions.FinalSetup( aregarr );
    // std::cout << "__ END TERRAIN" << std::endl;
    // std::cout << str_display( aregarr ) << std::endl;
    // std::cout << list_regions( aregarr ) << std::endl;
    // std::cout << "__DEBUG WORLD" << std::endl;
    // for (int i = 0; i < _game->regions.numLevels; i++) {
    //   ARegionArray *pArr = _game->regions.pRegionArrays[i];
    //   std::cout << str_display( pArr ) << std::endl;
    //   std::cout << list_regions( pArr ) << std::endl;
    // }


    // Now create shafts
    if( underworld_level == 0 ) {
      // connect as first underworld
      // shafts from surface to underworld
      std::cout << "initMakeShafts from" << 2 << " to " << 1 << std::endl;
      _game->regions.MakeShaftLinks(2, 1, 8);
    }
    else {
      // already some underworld
      // shafts from surface to this underworld
      std::cout << "MakeShafts from " << level << " to " << 1 << std::endl;
      _game->regions.MakeShaftLinks( level, 1, 10*level-10 );
      // Shafts from underworld to underworld
      for( int i = 2; i < level; ++i) {
	// right above ?
	std::cout << "MakeShafts from" << level << " to " << i << std::endl;
	if( i == (level-1) ) {
	  _game->regions.MakeShaftLinks(level, i, 12);
	}
	else {
	  _game->regions.MakeShaftLinks(level, i, 24);
	}
      }
    }
    // Now add gates
    _game->regions.InitSetupGates( level );
    // And update number of levels
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
  // ********************************************** MapAccess::reshuffle_gates
  /** 
   * Every not set gates (-1) is set to a new index
   */
  void reshuffle_gates( int idx_max_init, int nb_new_gates )
  {
    // build a vector of indexes
    std::vector<int> idx_gate;
    for( unsigned int i = 0; i <= nb_new_gates; ++i) {
      idx_gate.push_back( i+idx_max_init+1 );
    }

    // shuffle
    // using built-in random generator:
    std::random_shuffle ( idx_gate.begin(), idx_gate.end() );

    // And assign new gates
     // list all the levels
    int idx_new = 0;
    for (int i = 0; i < _game->regions.numLevels; i++) {
      ARegionArray *pArr = _game->regions.pRegionArrays[i];
      for( int i=0; i < pArr->x * pArr->y /2; i++) {
	ARegion* reg = pArr->regions[i];
	if( reg->gate == -1 ) {
	  reg->gate = idx_gate[idx_new];
	  idx_new ++;
	}
      }
    }
  }
  // ****************************************** MapAccess::AddUnderWorldLevel
  void createUnderWorld()
  {
    // Game Init Game::NewGame() partial
    _game->factionseq = 1;
    _game->guardfaction = 0;
    _game->monfaction = 0;
    _game->unitseq = 1;
    _game->SetupUnitNums();
    _game->shipseq = 100;
    _game->year = 1;
    _game->month = -1;
    //_game->gameStatus = GAME_STATUS_NEW;
    seedrandomrandom();

    // ARegionList::CreateLevels(int n)
    _game->regions.CreateLevels( 1 );
    // Prepare named array... dans world.cpp
    SetupNames();

    // All the regions point to 0 (null)
    int xSize = 4;
    int ySize = 4;
    int level = 0;

     // ARegionList::MakeRegions(int level, int xSize, int ySize)
    _game->regions.MakeRegions( 0, xSize, ySize );
    ARegionArray* aregarr = _game->regions.pRegionArrays[0];

    aregarr->SetName("MyUnderWorld");
    aregarr->levelType = ARegionArray::LEVEL_UNDERWORLD;

     _game->regions.SetRegTypes( aregarr, R_NUM);
    std::cout << "__Set Type " << std::endl;
    std::cout << str_display( aregarr ) << std::endl;
    std::cout << list_regions( aregarr ) << std::endl;

    SetupAnchorsUnderworld( aregarr );
    std::cout << "__Set Anchors " << std::endl;
    std::cout << str_display( aregarr ) << std::endl;
    std::cout << list_regions( aregarr ) << std::endl;

    GrowTerrainUnderworld( aregarr, 1);
    std::cout << "__GrowTerrain " << std::endl;
    std::cout << str_display( aregarr ) << std::endl;
    std::cout << list_regions( aregarr ) << std::endl;

    RandomTerrainUnderworld( aregarr );
    std::cout << "__Random Terrain " << std::endl;
    std::cout << str_display( aregarr ) << std::endl;
    std::cout << list_regions( aregarr ) << std::endl;

    _game->regions.MakeUWMaze( aregarr );
    std::cout << "__Make UW Maze " << std::endl;
    std::cout << str_display( aregarr ) << std::endl;
    std::cout << list_regions( aregarr ) << std::endl;

    if (Globals->GROW_RACES)
      _game->regions.GrowRaces( aregarr );
    _game->regions.FinalSetup( aregarr );
  }
  void SetupAnchorsUnderworld(ARegionArray *ta)
  {
    // Now, setup the anchors
    Awrite("Setting up the anchors");
    int skip = 250;
    int f = 2;
    if (Globals->TERRAIN_GRANULARITY) {
      skip = Globals->TERRAIN_GRANULARITY;
      while (skip > 5) {
	f++;
	skip -= 5;
	if (skip < 1) skip = 1;
      }
      skip = 100 * ((skip+3) * f + 2) / (skip + f - 2);
    }
    int dotter = 0;
    for (int x=0; x<(ta->x)/f; x++) {
      for (int y=0; y<(ta->y)/(f*2); y++) {
	if (getrandom(1000) > skip) continue;
	ARegion *reg = 0;
	for (int i=0; i<4; i++) {
	  int tempx = x * f + getrandom(f);
	  int tempy = y * f * 2 + getrandom(f)*2 + tempx%2;
	  reg = ta->GetRegion(tempx, tempy);
	  if (!reg)
	    continue;
	  if (reg->type == R_NUM) {
	    reg->type = GetRegTypeUnderworld(reg, ta->y);
	    reg->population = 1;
	    if (TerrainDefs[reg->type].similar_type != R_OCEAN)
	      reg->wages = AGetName(0, reg);
	    break;
	  }
	}
	if (dotter++%30 == 0) Adot();
      }
    }
    
    Awrite("");
  }
  void GrowTerrainUnderworld(ARegionArray *pArr, int growOcean)
  {
    Awrite("Growing Terrain...");
    for (int j=0; j<30; j++) {
      int x, y;
      for (x = 0; x < pArr->x; x++) {
	for (y = 0; y < pArr->y; y++) {
	  ARegion *reg = pArr->GetRegion(x, y);
	  if (!reg) continue;
	  reg->population = 1;
	}
      }
      for (x = 0; x < pArr->x; x++) {
	for (y = 0; y < pArr->y; y++) {
	  ARegion *reg = pArr->GetRegion(x, y);
	  if (!reg) continue;
	  if ((j > 0) && (j < 21) && (getrandom(3) < 2)) continue;
	  if (reg->type == R_NUM) {
	    
	    // Check for Lakes
	    if (Globals->LAKES &&
		(getrandom(100) < (Globals->LAKES/10 + 1))) {
	      reg->type = R_LAKE;
	      break;
	    }
	    // Check for Odd Terrain
	    if (getrandom(1000) < Globals->ODD_TERRAIN) {
	      reg->type = GetRegTypeUnderworld(reg, pArr->y);
	      if (TerrainDefs[reg->type].similar_type != R_OCEAN)
		reg->wages = AGetName(0, reg);
	      break;
	    }
	    
	    int init = getrandom(6);
	    for (int i=0; i<NDIRS; i++) {
	      ARegion *t = reg->neighbors[(i+init) % NDIRS];
	      if (t) {
		if (t->population < 1) continue;
		if (t->type != R_NUM &&
		    (TerrainDefs[t->type].similar_type!=R_OCEAN ||
		     (growOcean && (t->type != R_LAKE)))) {
		  if (j==0) t->population = 0;
		  reg->population = 0;
		  reg->race = t->type;
		  reg->wages = t->wages;
		  break;
		}
	      }
	    }
	  }
	}
      }
      
      for (x = 0; x < pArr->x; x++) {
	for (y = 0; y < pArr->y; y++) {
	  ARegion *reg = pArr->GetRegion(x, y);
	  if (!reg) continue;
	  if (reg->type == R_NUM && reg->race != -1)
	    reg->type = reg->race;
	}
      }
    }
  }
  void RandomTerrainUnderworld(ARegionArray *pArr)
  {
    int x, y;
    for (x = 0; x < pArr->x; x++) {
      for (y = 0; y < pArr->y; y++) {
	ARegion *reg = pArr->GetRegion(x, y);
	if (!reg) continue;
	
	if (reg->type == R_NUM) {
	  int adjtype = 0;
	  int adjname = -1;
	  for (int d = 0; d < NDIRS; d++) {
	    ARegion *newregion = reg->neighbors[d];
	    if (!newregion) continue;
	    if ((TerrainDefs[newregion->type].similar_type !=
		 R_OCEAN) && (newregion->type != R_NUM) &&
		(newregion->wages > 0)) {
	      adjtype = newregion->type;
	      adjname = newregion->wages;
	    }
	  }
	  if (adjtype && !Globals->CONQUEST_GAME) {
	    reg->type = adjtype;
	    reg->wages = adjname;
	  } else {
	    reg->type = GetRegTypeUnderworld(reg, pArr->y);
	    reg->wages = AGetName(0, reg);
	  }
	}
      }
    }
  }
  int GetRegTypeUnderworld( ARegion *pReg, int yRegionArray )
  {
    //
    // Figure out the distance from the equator, from 0 to 3.
    //
    int lat = ( pReg->yloc * 8 ) / ( yRegionArray );
    if (lat > 3)
      {
	lat = (7 - lat);
      }
    

    // Underdeep region
    int r = getrandom(4);
    switch(r) {
    case 0:
      return R_OCEAN;
    case 1:
      return R_CAVERN;
    case 2:
      return R_UFOREST;
    case 3:
      return R_TUNNELS;
    default:
      return (0);
    }

    //
    // This really shouldn't get called either
    //
    return( R_OCEAN );
  }

  // ********************************************** MapAccess::createUnderDeep
  void createUnderDeep()
  {
    // Game Init Game::NewGame() partial
    _game->factionseq = 1;
    _game->guardfaction = 0;
    _game->monfaction = 0;
    _game->unitseq = 1;
    _game->SetupUnitNums();
    _game->shipseq = 100;
    _game->year = 1;
    _game->month = -1;
    //_game->gameStatus = GAME_STATUS_NEW;
    seedrandomrandom();

    // ARegionList::CreateLevels(int n)
    _game->regions.CreateLevels( 1 );
    // Prepare named array... dans world.cpp
    SetupNames();
    
    // All the regions point to 0 (null)
    int xSize = 4;
    int ySize = 4;
    int level = 0;
    //ARegionArray* aregarr = new ARegionArray(xSize, ySize);
    
    // ARegionList::MakeRegions(int level, int xSize, int ySize)
    _game->regions.MakeRegions( 0, xSize, ySize );
    ARegionArray* aregarr = _game->regions.pRegionArrays[0];
    //
    // Make the regions themselves
    //
    // int x, y;
    // for (y = 0; y < ySize; y++) {
    //   for (x = 0; x < xSize; x++) {
    // 	if (!((x + y) % 2)) {
    // 	  ARegion *reg = new ARegion;
    // 	  reg->SetLoc(x, y, level);
    // 	  reg->num = _game->regions.Num();
	  
    // 	  //
    // 	  // Some initial values; these will get reset
    // 	  //
    // 	  reg->type = -1;
    // 	  reg->race = -1;  
    // 	  reg->wages = -1; 
	  
    // 	  _game->regions.Add(reg);
    // 	  aregarr->SetRegion(x, y, reg);
    // 	}
    //   }
    // }
    // _game->regions.SetupNeighbors( aregarr );
    // for (x = 0; x < aregarr->x; x++) {
    //   for (y = 0; y < aregarr->y; y++) {
    // 	ARegion *reg = aregarr->GetRegion(x, y);
    // 	if (!reg) continue;
    // 	NeighSetup(reg, pRegs);
    //   }
    // }
    aregarr->SetName("MyUnderDeep");
    aregarr->levelType = ARegionArray::LEVEL_UNDERDEEP;
    // std::cout << "__NEW " << std::endl;
    // std::cout << str_display( aregarr ) << std::endl;
    // std::cout << list_regions( aregarr ) << std::endl;

    _game->regions.SetRegTypes( aregarr, R_NUM);
    std::cout << "__Set Type " << std::endl;
    std::cout << str_display( aregarr ) << std::endl;
    std::cout << list_regions( aregarr ) << std::endl;

    SetupAnchorsUnderdeep( aregarr );
    std::cout << "__Set Anchors " << std::endl;
    std::cout << str_display( aregarr ) << std::endl;
    std::cout << list_regions( aregarr ) << std::endl;

    GrowTerrainUnderdeep( aregarr, 1);
    std::cout << "__GrowTerrain " << std::endl;
    std::cout << str_display( aregarr ) << std::endl;
    std::cout << list_regions( aregarr ) << std::endl;

    RandomTerrainUnderdeep( aregarr );
    std::cout << "__Random Terrain " << std::endl;
    std::cout << str_display( aregarr ) << std::endl;
    std::cout << list_regions( aregarr ) << std::endl;

    _game->regions.MakeUWMaze( aregarr );
    std::cout << "__Make UW Maze " << std::endl;
    std::cout << str_display( aregarr ) << std::endl;
    std::cout << list_regions( aregarr ) << std::endl;

    if (Globals->GROW_RACES)
      _game->regions.GrowRaces( aregarr );
    _game->regions.FinalSetup( aregarr );
    
  }
  void SetupAnchorsUnderdeep(ARegionArray *ta)
  {
    // Now, setup the anchors
    Awrite("Setting up the anchors");
    int skip = 250;
    int f = 2;
    if (Globals->TERRAIN_GRANULARITY) {
      skip = Globals->TERRAIN_GRANULARITY;
      while (skip > 5) {
	f++;
	skip -= 5;
	if (skip < 1) skip = 1;
      }
      skip = 100 * ((skip+3) * f + 2) / (skip + f - 2);
    }
    int dotter = 0;
    for (int x=0; x<(ta->x)/f; x++) {
      for (int y=0; y<(ta->y)/(f*2); y++) {
	if (getrandom(1000) > skip) continue;
	ARegion *reg = 0;
	for (int i=0; i<4; i++) {
	  int tempx = x * f + getrandom(f);
	  int tempy = y * f * 2 + getrandom(f)*2 + tempx%2;
	  reg = ta->GetRegion(tempx, tempy);
	  if (!reg)
	    continue;
	  if (reg->type == R_NUM) {
	    reg->type = GetRegTypeUnderdeep(reg, ta->y);
	    reg->population = 1;
	    if (TerrainDefs[reg->type].similar_type != R_OCEAN)
	      reg->wages = AGetName(0, reg);
	    break;
	  }
	}
	if (dotter++%30 == 0) Adot();
      }
    }
    
    Awrite("");
  }
  void GrowTerrainUnderdeep(ARegionArray *pArr, int growOcean)
  {
    Awrite("Growing Terrain...");
    for (int j=0; j<30; j++) {
      int x, y;
      for (x = 0; x < pArr->x; x++) {
	for (y = 0; y < pArr->y; y++) {
	  ARegion *reg = pArr->GetRegion(x, y);
	  if (!reg) continue;
	  reg->population = 1;
	}
      }
      for (x = 0; x < pArr->x; x++) {
	for (y = 0; y < pArr->y; y++) {
	  ARegion *reg = pArr->GetRegion(x, y);
	  if (!reg) continue;
	  if ((j > 0) && (j < 21) && (getrandom(3) < 2)) continue;
	  if (reg->type == R_NUM) {
	    
	    // Check for Lakes
	    if (Globals->LAKES &&
		(getrandom(100) < (Globals->LAKES/10 + 1))) {
	      reg->type = R_LAKE;
	      break;
	    }
	    // Check for Odd Terrain
	    if (getrandom(1000) < Globals->ODD_TERRAIN) {
	      reg->type = GetRegTypeUnderdeep(reg, pArr->y);
	      if (TerrainDefs[reg->type].similar_type != R_OCEAN)
		reg->wages = AGetName(0, reg);
	      break;
	    }
					
	    
	    int init = getrandom(6);
	    for (int i=0; i<NDIRS; i++) {
	      ARegion *t = reg->neighbors[(i+init) % NDIRS];
	      if (t) {
		if (t->population < 1) continue;
		if (t->type != R_NUM &&
		    (TerrainDefs[t->type].similar_type!=R_OCEAN ||
		     (growOcean && (t->type != R_LAKE)))) {
		  if (j==0) t->population = 0;
		  reg->population = 0;
		  reg->race = t->type;
		  reg->wages = t->wages;
		  break;
		}
	      }
	    }
	  }
	}
      }
      
      for (x = 0; x < pArr->x; x++) {
	for (y = 0; y < pArr->y; y++) {
	  ARegion *reg = pArr->GetRegion(x, y);
	  if (!reg) continue;
	  if (reg->type == R_NUM && reg->race != -1)
	    reg->type = reg->race;
	}
      }
    }
  }
  void RandomTerrainUnderdeep(ARegionArray *pArr)
  {
    int x, y;
    for (x = 0; x < pArr->x; x++) {
      for (y = 0; y < pArr->y; y++) {
	ARegion *reg = pArr->GetRegion(x, y);
	if (!reg) continue;
	
	if (reg->type == R_NUM) {
	  int adjtype = 0;
	  int adjname = -1;
	  for (int d = 0; d < NDIRS; d++) {
	    ARegion *newregion = reg->neighbors[d];
	    if (!newregion) continue;
	    if ((TerrainDefs[newregion->type].similar_type !=
		 R_OCEAN) && (newregion->type != R_NUM) &&
		(newregion->wages > 0)) {
	      adjtype = newregion->type;
	      adjname = newregion->wages;
	    }
	  }
	  if (adjtype && !Globals->CONQUEST_GAME) {
	    reg->type = adjtype;
	    reg->wages = adjname;
	  } else {
	    reg->type = GetRegTypeUnderdeep(reg, pArr->y);
	    reg->wages = AGetName(0, reg);
	  }
	}
      }
    }
  }
  int GetRegTypeUnderdeep( ARegion *pReg, int yRegionArray )
  {
    //
    // Figure out the distance from the equator, from 0 to 3.
    //
    int lat = ( pReg->yloc * 8 ) / ( yRegionArray );
    if (lat > 3)
      {
	lat = (7 - lat);
      }
    

    // Underdeep region
    int r = getrandom(4);
    switch(r) {
    case 0:
      return R_OCEAN;
    case 1:
      return R_CHASM;
    case 2:
      return R_DFOREST;
    case 3:
      return R_GROTTO;
    default:
      return (0);
    }

    //
    // This really shouldn't get called either
    //
    return( R_OCEAN );
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
    disp << std::endl;
    disp << str_gates( regarr ) << std::endl;
    disp << str_shaft( regarr ) << std::endl;
	
    return disp.str();
  }
  std::string str_gates( ARegionArray* regarr )
  {
    std::stringstream disp;
    disp << "Gates:";
    for( int i=0; i < regarr->x * regarr->y /2; i++) {
      ARegion* reg = regarr->regions[i];
      if( reg->gate > 0 ) {
	disp << " "<< reg->gate << " in " << reg->num << ";";
      }
      else if( reg->gate == -1 ) {
	disp << " ?? in " << reg->num << ";";
      }
    }
    return disp.str();
  }
  std::string str_shaft( ARegionArray* regarr )
  {
    std::stringstream disp;
    disp << "Shaft:";
    for( int i=0; i < regarr->x * regarr->y /2; i++) {
      ARegion* reg = regarr->regions[i];
      // Any Shaft ? (only object with inner != -1)
      forlist (&(reg->objects)) {
	Object *o = (Object *) elem;
	if (o->inner != -1) {
	  disp << " " << reg->num << "->" << o->inner << ";";
        }
      }
    }
    return disp.str();
  }
      
  std::string str_display( ARegion* reg)
  {
    std::stringstream disp;
    disp << "Reg " << reg->num;
    if( reg->type >= 0 and reg->type < R_NUM ) 
      disp << " : " << TerrainDefs[reg->type].name;
    else
      disp << " : " << reg->type;
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
  // ***************************************************** MapAccess::nb_gates
  int nb_gate()
  {
    int nb_gates = 0;
    // list all the levels
    for (int i = 0; i < _game->regions.numLevels; i++) {
      ARegionArray *pArr = _game->regions.pRegionArrays[i];
      for( int i=0; i < pArr->x * pArr->y /2; i++) {
	ARegion* reg = pArr->regions[i];
	if( reg->gate > 0 ) {
	  nb_gates ++;
	}
      }
    }
    return nb_gates;
  }
};
#endif // MAP_ACCESS_H
