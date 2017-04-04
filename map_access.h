/* -*- coding: utf-8 -*- */

#ifndef MAP_ACCESS_H
#define MAP_ACCESS_H

/**
 * Access the Game object to inspect and modify map.
 */
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <string>
#include <sstream>
#include <aregion.h>
#include <gamedata.h>
#include <gameio.h>
#include <game.h>
#include <faction.h>

#include <algorithm>    // std::random_shuffle
#include <vector>       // std::vector

void SetupNames();

class MapAccess;
class Game;
class ARegionList;

// ******************************************************************* Globals
using RefItem = std::pair<std::string,int>;
// ***************************************************************************
// ***************************************************************** MapAccess
// ***************************************************************************
class MapAccess
{
public:
  // ***************************************************** MapAccess::creation
  MapAccess( Game* game ) : _game(game)
  {
    init_random();
	// Try to read all factions
	_faction_enum.clear();
	forlist (&(_game->factions)) {
	  Faction *fac = (Faction *) elem;
	  //std::cout << *(fac->name) << std::endl;
	  std::string fac_name = std::string( fac->name->Str() );
	  _faction_enum.push_back( {fac_name, fac->num} );
	}

	// All Items from gamedata.cpp
	_item_names.clear();
	for( int id=0; id < NITEMS; ++id ) {
        ItemType item = ItemDefs[id];
        if( (item.flags & ItemType::DISABLED) == false ) {
            std::stringstream stritem;
            if( item.type & IT_ILLUSION )
                stritem << "i_";
            stritem << (item.name);
            _item_names.push_back( {stritem.str(), id} );
        }
	}

	// All Skills from gamedata.cpp
	_skill_names.clear();
	for( int id=0; id < NSKILLS; ++id ) {
        SkillType skill = SkillDefs[id];
        if( (skill.flags & SkillType::DISABLED) == false) {
            _skill_names.push_back( {skill.name, id} );
        }
	}

	// Unit type
	_unit_types.clear();
	_unit_types.push_back( {"NORMAL",0});
	_unit_types.push_back( {"MAGE",1});
	_unit_types.push_back( {"GUARD",2});
	_unit_types.push_back( {"WMON",3});
	_unit_types.push_back( {"GUARDMAGE",4});
	_unit_types.push_back( {"APPRENTICE",5});

	// Object Types
    _object_types.clear();
    for( int id=0; id < NOBJECTS; ++id ) {
        ObjectType obj = ObjectDefs[id];
        if( (obj.flags & ObjectType::DISABLED) == false ) {
            _object_types.push_back( {obj.name, id} );
        }
    }

	// Guard Types
	_guard_types.clear();
	_guard_types.push_back( {"NONE", 0} );
	_guard_types.push_back( {"GUARD", 1} );
	_guard_types.push_back( {"AVOID", 2} );
	_guard_types.push_back( {"SET", 3} );
	_guard_types.push_back( {"ADVANCE", 4} );

	// Reveal Types
	_reveal_types.clear();
	_reveal_types.push_back( {"NONE", 0} );
	_reveal_types.push_back( {"UNIT", 1} );
	_reveal_types.push_back( {"FACTION", 2} );
  }
  // **************************************************** MapAccess::Attributs
  std::vector<RefItem> _faction_enum;
  std::vector<RefItem> _item_names;
  std::vector<RefItem> _skill_names;
  std::vector<RefItem> _unit_types;
  std::vector<RefItem> _object_types;
  std::vector<RefItem> _guard_types;
  std::vector<RefItem> _reveal_types;
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
    /* // Set up the new gate. */
    /* // Find the region of the new abyss with the gate */
    /* // and set its number to numberofgates-1 */
    /* ARegionArray* regarr = _game->regions.pRegionArrays[level]; */
    /* for( int i=0; i < regarr->x * regarr->y /2; i++) { */
    /*   ARegion* reg = regarr->regions[i]; */
    /*   if( reg->gate == -1 ) { */
    /* 	reg->gate = _game->regions.numberofgates; */
    /*   }     */
    /* } */
    // Update number of levels

    // TODO : create Monsters
    /* if (Globals->CITY_MONSTERS_EXIST) */
    /*   CreateCityMons(); */
    /* if (Globals->WANDERING_MONSTERS_EXIST) */
    /*   CreateWMons(); */
    /* if (Globals->LAIR_MONSTERS_EXIST) */
    /*   CreateLMons(); */

    /* if (Globals->LAIR_MONSTERS_EXIST) */
    /*   CreateVMons(); */

    // Now add City and Lair Monsters
    // Regions of the level
    int rate = 50;
    ARegionArray *pArr = _game->regions.pRegionArrays[level];
    for( int i=0; i < pArr->x * pArr->y /2; i++) {
      ARegion* reg = pArr->regions[i];
      if( Globals->CITY_MONSTERS_EXIST ) {
	if ((reg->type == R_NEXUS) || reg->IsStartingCity() || reg->town) {
	  _game->CreateCityMon(reg, 100, 1);
	  std::cout << "MONSTER CITY in " << *reg->name << " at " << reg->xloc << ", " << reg->yloc << ", " << reg->zloc << std::endl;
	}
      }
      if( Globals->LAIR_MONSTERS_EXIST ) {
	// npc.cpp GrowLMons(50);
	if (reg->IsGuarded()) continue;

	forlist(&reg->objects) {
	  Object *obj = (Object *) elem;
	  if (obj->units.Num()) continue;
	  int montype = ObjectDefs[obj->type].monster;
	  int grow=!(ObjectDefs[obj->type].flags&ObjectType::NOMONSTERGROWTH);
	  if ((montype != -1) && grow) {
	    if (getrandom(100) < rate) {
	      std::cout << "MONSTER LAIR in " << *reg->name << " at " << reg->xloc << ", " << reg->yloc << ", " << reg->zloc << std::endl;
	      _game->MakeLMon(obj);
	    }
	  }
	}
      }
      if( Globals->LAIR_MONSTERS_EXIST ) {
	forlist(&reg->objects) {
	  Object * obj = (Object *) elem;
	  if (obj->type != O_BKEEP) continue;
	  std::cout << "MONSTER BKEEP in " << *reg->name << " at " << reg->xloc << ", " << reg->yloc << ", " << reg->zloc << std::endl;
	  Faction *monfac = GetFaction( &(_game->factions), 2 );
	  Unit *u = _game->GetNewUnit( monfac, 0 );
	  u->MakeWMon( "Elder Demons", I_BALROG, 200);
	  u->MoveUnit(obj);
	}
      }
    }
    if( Globals->WANDERING_MONSTERS_EXIST ) {
      int xsec;
      for (xsec=0; xsec< pArr->x / 8; xsec++) {
	for (int ysec=0; ysec< pArr->y / 16; ysec++) {
	  /* OK, we have a sector. Count mons, and wanted */
	  int mons=0;
	  int wanted=0;
	  for (int x=0; x<8; x++) {
	    for (int y=0; y<16; y+=2) {
	      ARegion *reg = pArr->GetRegion(x+xsec*8, y+ysec*16+x%2);
	      if (reg && !reg->IsGuarded()) {
		mons += reg->CountWMons();
		/*
		 * Make sure there is at least one monster type
		 * enabled for this region
		 */
		int avail = 0;
		int mon = TerrainDefs[reg->type].smallmon;
		if (!((mon == -1) ||
		      (ItemDefs[mon].flags & ItemType::DISABLED)))
		  avail = 1;
		mon = TerrainDefs[reg->type].bigmon;
		if (!((mon == -1) ||
		      (ItemDefs[mon].flags & ItemType::DISABLED)))
		  avail = 1;
		mon = TerrainDefs[reg->type].humanoid;
		if (!((mon == -1) ||
		      (ItemDefs[mon].flags & ItemType::DISABLED)))
		  avail = 1;

		if (avail)
		  wanted += TerrainDefs[reg->type].wmonfreq;
	      }
	    }
	  }

	  wanted /= 10;
	  wanted -= mons;
	  wanted = (wanted*rate + getrandom(100))/100;
	  if (wanted > 0) {
	    for (int i=0; i< wanted;) {
	      int m=getrandom(8);
	      int n=getrandom(8)*2+m%2;
	      ARegion *reg = pArr->GetRegion(m+xsec*8, n+ysec*16);
	      if (reg && !reg->IsGuarded() && _game->MakeWMon(reg)) {
		std::cout << "MONSTER WANDER in " << *reg->name << " at " << reg->xloc << ", " << reg->yloc << ", " << reg->zloc << std::endl;
		i++;
	      }
	    }
	  }
	}
      }
    }


    _game->regions.numLevels += 1;
  }
  // ******************************************* MapAccess::AddUnderWorldLevel
  /**
   * Si underworld_level == 0 => premier niveau de underworld.
   */
  void AddUnderWorldLevel( int level, int xSize, int ySize,
			   int wanted_nb_shaft,
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


    // Crée un shaft
    // random level above
    ARegionArray *pFrom = _game->regions.pRegionArrays[level];
    ARegionArray *pTo;
    int nb_max_try_shaft = wanted_nb_shaft * 100;
    int try_shaft = 0;
    while( nb_shaft( pFrom ) < wanted_nb_shaft && try_shaft < nb_max_try_shaft) {
      int from_x = get_random( pFrom->x );
      int from_y = get_random( pFrom->y );
      int level_to = get_random( level-1) + 1;
      pTo = _game->regions.pRegionArrays[level_to];
      ARegion *reg = pFrom->GetRegion( from_x, from_y );
      // Essaye de créer un nouveau Shaft
      if( reg ) {
	if( !reg->HasShaft() ) {
	  //std::cout << "  MS try dans reg="<< reg->num << " x="<<from_x << " y="<<from_y << " to level=" << level_to << std::endl;
	  _game->regions.MakeShaft(reg, pFrom, pTo);
	}
	else {
	  //std::cout << "  MS try dans reg="<< reg->num << " x="<<from_x << " y="<<from_y << " HAS SHAFT ";
	}
      }

      try_shaft++;
    }

    /* // Now create shafts */
    /* if( underworld_level == 0 ) { */
    /*   // connect as first underworld */
    /*   // shafts from surface to underworld */
    /*   std::cout << "initMakeShafts from" << 2 << " to " << 1 << std::endl; */
    /*   _game->regions.MakeShaftLinks(2, 1, 8); */
    /* } */
    /* else { */
    /*   // already some underworld */
    /*   // shafts from surface to this underworld */
    /*   std::cout << "MakeShafts from " << level << " to " << 1 << std::endl; */
    /*   _game->regions.MakeShaftLinks( level, 1, 10*level-10 ); */
    /*   // Shafts from underworld to underworld */
    /*   for( int i = 2; i < level; ++i) { */
    /* 	// right above ? */
    /* 	std::cout << "MakeShafts from" << level << " to " << i << std::endl; */
    /* 	if( i == (level-1) ) { */
    /* 	  _game->regions.MakeShaftLinks(level, i, 12); */
    /* 	} */
    /* 	else { */
    /* 	  _game->regions.MakeShaftLinks(level, i, 24); */
    /* 	} */
    /*   } */
    /* } */

    // Now add City and Lair Monsters
    // Regions of the level
    int rate = 50;
    ARegionArray *pArr = _game->regions.pRegionArrays[level];
    for( int i=0; i < pArr->x * pArr->y /2; i++) {
      ARegion* reg = pArr->regions[i];
      if( Globals->CITY_MONSTERS_EXIST ) {
	if ((reg->type == R_NEXUS) || reg->IsStartingCity() || reg->town) {
	  _game->CreateCityMon(reg, 100, 1);
	  std::cout << "MONSTER CITY in " << *reg->name << " at " << reg->xloc << ", " << reg->yloc << ", " << reg->zloc << std::endl;
	}
      }
      if( Globals->LAIR_MONSTERS_EXIST ) {
	// npc.cpp GrowLMons(50);
	if (reg->IsGuarded()) continue;

	forlist(&reg->objects) {
	  Object *obj = (Object *) elem;
	  if (obj->units.Num()) continue;
	  int montype = ObjectDefs[obj->type].monster;
	  int grow=!(ObjectDefs[obj->type].flags&ObjectType::NOMONSTERGROWTH);
	  if ((montype != -1) && grow) {
	    if (getrandom(100) < rate) {
	      std::cout << "MONSTER LAIR in " << *reg->name << " at " << reg->xloc << ", " << reg->yloc << ", " << reg->zloc << std::endl;
	      _game->MakeLMon(obj);
	    }
	  }
	}
      }
    }
    if( Globals->WANDERING_MONSTERS_EXIST ) {
      int xsec;
      for (xsec=0; xsec< pArr->x / 8; xsec++) {
	for (int ysec=0; ysec< pArr->y / 16; ysec++) {
	  /* OK, we have a sector. Count mons, and wanted */
	  int mons=0;
	  int wanted=0;
	  for (int x=0; x<8; x++) {
	    for (int y=0; y<16; y+=2) {
	      ARegion *reg = pArr->GetRegion(x+xsec*8, y+ysec*16+x%2);
	      if (reg && !reg->IsGuarded()) {
		mons += reg->CountWMons();
		/*
		 * Make sure there is at least one monster type
		 * enabled for this region
		 */
		int avail = 0;
		int mon = TerrainDefs[reg->type].smallmon;
		if (!((mon == -1) ||
		      (ItemDefs[mon].flags & ItemType::DISABLED)))
		  avail = 1;
		mon = TerrainDefs[reg->type].bigmon;
		if (!((mon == -1) ||
		      (ItemDefs[mon].flags & ItemType::DISABLED)))
		  avail = 1;
		mon = TerrainDefs[reg->type].humanoid;
		if (!((mon == -1) ||
		      (ItemDefs[mon].flags & ItemType::DISABLED)))
		  avail = 1;

		if (avail)
		  wanted += TerrainDefs[reg->type].wmonfreq;
	      }
	    }
	  }

	  wanted /= 10;
	  wanted -= mons;
	  wanted = (wanted*rate + getrandom(100))/100;
	  if (wanted > 0) {
	    for (int i=0; i< wanted;) {
	      int m=getrandom(8);
	      int n=getrandom(8)*2+m%2;
	      ARegion *reg = pArr->GetRegion(m+xsec*8, n+ysec*16);
	      if (reg && !reg->IsGuarded() && _game->MakeWMon(reg)) {
		std::cout << "MONSTER WANDER in " << *reg->name << " at " << reg->xloc << ", " << reg->yloc << ", " << reg->zloc << std::endl;
		i++;
	      }
	    }
	  }
	}
      }
    }

    // Now add gates
    _game->regions.InitSetupGates( level );
    // And update number of levels
    _game->regions.numLevels += 1;
  }
  // ******************************************** MapAccess::AddUnderDeepLevel
  void AddUnderDeepLevel( int level, int xSize, int ySize, int wanted_nb_shaft,
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
    // and Prepare named array... dans world.cpp
    SetupNames();

    // Create new region
    // ARegionList::MakeRegions(int level, int xSize, int ySize)
    _game->regions.MakeRegions( level, xSize, ySize );
    ARegionArray* aregarr = _game->regions.pRegionArrays[level];
    aregarr->SetName("underdeep");
    aregarr->levelType = ARegionArray::LEVEL_UNDERDEEP;
    // Set terrains
    _game->regions.SetRegTypes( aregarr, R_NUM);
    SetupAnchorsUnderdeep( aregarr );
    GrowTerrainUnderdeep( aregarr, 1);
    RandomTerrainUnderdeep( aregarr );
    _game->regions.MakeUWMaze( aregarr );
    if (Globals->GROW_RACES)
      _game->regions.GrowRaces( aregarr );
    _game->regions.FinalSetup( aregarr );


    // Crée un shaft
    // random level above
    ARegionArray *pFrom = _game->regions.pRegionArrays[level];
    ARegionArray *pTo;
    int nb_max_try_shaft = wanted_nb_shaft * 100;
    int try_shaft = 0;
    while( nb_shaft( pFrom ) < wanted_nb_shaft && try_shaft < nb_max_try_shaft) {
      int from_x = get_random( pFrom->x );
      int from_y = get_random( pFrom->y );
      int level_to = get_random( level-1) + 1;
      pTo = _game->regions.pRegionArrays[level_to];
      ARegion *reg = pFrom->GetRegion( from_x, from_y );
      // Essaye de créer un nouveau Shaft
      if( reg ) {
	if( !reg->HasShaft() ) {
	  //std::cout << "  MS try dans reg="<< reg->num << " x="<<from_x << " y="<<from_y << " to level=" << level_to << std::endl;
	  _game->regions.MakeShaft(reg, pFrom, pTo);
	}
	else {
	  //std::cout << "  MS try dans reg="<< reg->num << " x="<<from_x << " y="<<from_y << " HAS SHAFT ";
	}
      }

      try_shaft++;
    }

    // // Now create shafts
    // if( level == underground_lvl+1 ) {
    //   // connect as first underdeep
    //   _game->regions.MakeShaftLinks( level, level-1, 12);
    // }
    // else {
    //   for( int i=underground_lvl+1; i<level; i++ ) {
    // 	std::cout << "  +Connect with UnderDeep at " << i << std::endl;
    // 	if( i == (level-1) )
    // 	  _game->regions.MakeShaftLinks( level, i, 12);
    // 	else
    // 	  _game->regions.MakeShaftLinks( level, i, 25);
    //   }
    // }

    // Now add City and Lair Monsters
    // Regions of the level
    int rate = 50;
    ARegionArray *pArr = _game->regions.pRegionArrays[level];
    for( int i=0; i < pArr->x * pArr->y /2; i++) {
      ARegion* reg = pArr->regions[i];
      if( Globals->CITY_MONSTERS_EXIST ) {
	if ((reg->type == R_NEXUS) || reg->IsStartingCity() || reg->town) {
	  _game->CreateCityMon(reg, 100, 1);
	  std::cout << "MONSTER CITY in " << *reg->name << " at " << reg->xloc << ", " << reg->yloc << ", " << reg->zloc << std::endl;
	}
      }
      if( Globals->LAIR_MONSTERS_EXIST ) {
	// npc.cpp GrowLMons(50);
	if (reg->IsGuarded()) continue;

	forlist(&reg->objects) {
	  Object *obj = (Object *) elem;
	  if (obj->units.Num()) continue;
	  int montype = ObjectDefs[obj->type].monster;
	  int grow=!(ObjectDefs[obj->type].flags&ObjectType::NOMONSTERGROWTH);
	  if ((montype != -1) && grow) {
	    if (getrandom(100) < rate) {
	      std::cout << "MONSTER LAIR in " << *reg->name << " at " << reg->xloc << ", " << reg->yloc << ", " << reg->zloc << std::endl;
	      _game->MakeLMon(obj);
	    }
	  }
	}
      }
    }
    if( Globals->WANDERING_MONSTERS_EXIST ) {
      int xsec;
      for (xsec=0; xsec< pArr->x / 8; xsec++) {
	for (int ysec=0; ysec< pArr->y / 16; ysec++) {
	  /* OK, we have a sector. Count mons, and wanted */
	  int mons=0;
	  int wanted=0;
	  for (int x=0; x<8; x++) {
	    for (int y=0; y<16; y+=2) {
	      ARegion *reg = pArr->GetRegion(x+xsec*8, y+ysec*16+x%2);
	      if (reg && !reg->IsGuarded()) {
		mons += reg->CountWMons();
		/*
		 * Make sure there is at least one monster type
		 * enabled for this region
		 */
		int avail = 0;
		int mon = TerrainDefs[reg->type].smallmon;
		if (!((mon == -1) ||
		      (ItemDefs[mon].flags & ItemType::DISABLED)))
		  avail = 1;
		mon = TerrainDefs[reg->type].bigmon;
		if (!((mon == -1) ||
		      (ItemDefs[mon].flags & ItemType::DISABLED)))
		  avail = 1;
		mon = TerrainDefs[reg->type].humanoid;
		if (!((mon == -1) ||
		      (ItemDefs[mon].flags & ItemType::DISABLED)))
		  avail = 1;

		if (avail)
		  wanted += TerrainDefs[reg->type].wmonfreq;
	      }
	    }
	  }

	  wanted /= 10;
	  wanted -= mons;
	  wanted = (wanted*rate + getrandom(100))/100;
	  if (wanted > 0) {
	    for (int i=0; i< wanted;) {
	      int m=getrandom(8);
	      int n=getrandom(8)*2+m%2;
	      ARegion *reg = pArr->GetRegion(m+xsec*8, n+ysec*16);
	      if (reg && !reg->IsGuarded() && _game->MakeWMon(reg)) {
		std::cout << "MONSTER WANDER in " << *reg->name << " at " << reg->xloc << ", " << reg->yloc << ", " << reg->zloc << std::endl;
		i++;
	      }
	    }
	  }
	}
      }
    }


    // Underdeep has no gates, only the possible shafts above.
    _game->regions.numLevels += 1;
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
  std::string str_display( ARegionArray* regarr, bool verb=false )
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
    if( verb ) {
     disp << str_gates( regarr ) << std::endl;
     disp << str_shaft( regarr ) << std::endl;
    }
    else {
      disp << nb_gate( regarr ) << " gates and ";
      disp << nb_shaft( regarr ) << " shaft" << std::endl;
    }

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
    for (int j = 0; j < _game->regions.numLevels; j++) {
      ARegionArray *pArr = _game->regions.pRegionArrays[j];
      for( int i=0; i < pArr->x * pArr->y /2; i++) {
	ARegion* reg = pArr->regions[i];
	if( reg->gate > 0 ) {
	  nb_gates ++;
	}
      }
    }
    return nb_gates;
  }
  int nb_gate( ARegionArray* pArr )
  {
    int nb_gates = 0;
    for( int i=0; i < pArr->x * pArr->y /2; i++) {
      ARegion* reg = pArr->regions[i];
      if( reg->gate > 0 ) {
	nb_gates ++;
      }
    }
    return nb_gates;
  }
  /** Ajouter une porte dans ARegion */
  void add_gate( ARegion* reg )
  {
    if( reg->gate == 0 ) {
      int nb_current = nb_gate();
      reg->gate = nb_current+1;
    }
  }
  void del_gate( ARegion *reg )
  {
    if( reg->gate > 0 ) {
      int num_gate = reg->gate;
      // Change all bigger
      for (int i = 0; i < _game->regions.numLevels; i++) {
	ARegionArray *pArr = _game->regions.pRegionArrays[i];
	for( int i=0; i < pArr->x * pArr->y /2; i++) {
	  ARegion* tmpreg = pArr->regions[i];
	  if( tmpreg->gate > num_gate ) {
	    tmpreg->gate = tmpreg->gate -1;
	  }
	}
      }
      reg->gate = 0;
    }
  }
  void change_gate( ARegion *reg, int new_num )
  {
    if( reg->gate > 0 and reg->gate != new_num and new_num <= nb_gate()) {
      int num_current = reg->gate;
      // swap number
      for (int i = 0; i < _game->regions.numLevels; i++) {
	ARegionArray *pArr = _game->regions.pRegionArrays[i];
	for( int i=0; i < pArr->x * pArr->y /2; i++) {
	  ARegion* tmpreg = pArr->regions[i];
	  if( tmpreg->gate == new_num ) {
	    tmpreg->gate = num_current;
	    break;
	  }
	}
      }
      reg->gate = new_num;
    }
  }
  // // And assign new gates
  //    // list all the levels
  //   int idx_new = 0;
  //   for (int i = 0; i < _game->regions.numLevels; i++) {
  //     ARegionArray *pArr = _game->regions.pRegionArrays[i];
  //     for( int i=0; i < pArr->x * pArr->y /2; i++) {
  // 	ARegion* reg = pArr->regions[i];
  // 	if( reg->gate == -1 ) {
  // 	  reg->gate = idx_gate[idx_new];
  // 	  idx_new ++;
  // 	}
  //     }
  //   }
  // *************************************************** MapAccess::rand_int
  /**
   * Generate a random int with a first seed taken from std::steady_clock
   * and then using std::random.
   */
  int init_random()
  {
    std::srand(std::time(0));
  }
  int get_random(int max_val)
  {
    return std::rand() % max_val;
  }
  // *********************************************** MapAccess::recompute_hex
  void recompute_hex( ARegion* reg )
  {
      if (reg->town) delete reg->town;
            reg->town = NULL;

            reg->products.DeleteAll();
            reg->SetupProds();

            reg->markets.DeleteAll();

            reg->SetupEditRegion();
            reg->UpdateEditRegion();
  }
  void add_town( ARegion* reg, AString * name)
  {
      reg->AddTown(name);
  }
  // ************************************************* MapAccess::get_faction
  Faction* get_faction( int fnum )
  {
      Faction* fac = GetFaction( &(_game->factions), fnum );
      return fac;
  }
  Unit* get_unit( int unum )
  {
      Unit* unit = _game->GetUnit( unum );
      return unit;
  }
  // ************************************************* MapAccess::create_unit
  Unit* create_unit(ARegion* reg, Object* obj )
  {
      Faction *fac = get_faction(1);
      Unit *newunit = _game->GetNewUnit(fac);
	  newunit->SetMen(I_LEADERS, 1);
	  newunit->reveal = REVEAL_FACTION;
	  newunit->MoveUnit( obj );

	  return newunit;
  }
  // ****************************************** MapAccess::renew_products
  void renew_products( ARegion* reg )
  {
    forlist((&reg->products)) {
        Production * p = ((Production *) elem);
        if (p->itemtype!=I_SILVER) {
            reg->products.Remove(p);
            delete p;
        }
    }
    reg->SetupProds();
  }
  // ******************************************** MapAccess::create_object
  void create_object( ARegion* reg, int type )
  {
	Object *o = new Object(reg);
	o->num =reg->buildingseq++;
	o->name = new AString(AString(ObjectDefs[type].name) +
			" [" + o->num + "]");
	o->type = type;
	o->incomplete = 0;
	o->inner = -1;
	reg->objects.Add(o);
	}
   bool del_object( ARegion* reg, Object *obj )
   {
       if( obj->type != 0 ) {
            Object *dest = reg->GetDummy();
            forlist(&obj->units) {
                Unit *u = (Unit *) elem;
                u->destroy = 0;
                u->MoveUnit(dest);
            }
            reg->objects.Remove(obj);
            delete obj;
            return true;
        }
        return false;
    }
};
#endif // MAP_ACCESS_H
