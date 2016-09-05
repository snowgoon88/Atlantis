/* -*- coding: utf-8 -*- */

/** 
 * - Init Game
 * - Apply Ruleset
 * - List existing elements
 */
#include <game.h>
#include <map_access.h>
#include <stdlib.h>     /* exit, EXIT_FAILURE */


Game _game;

void print_item(int id)
{
  if( id >= 0 ) {
    std::cout << ItemDefs[id].name;
  }
  else {
    std::cout << "-1";
  }
}

void list_items()
{
  // Every enables items
  std::cout << "__ITEMS_________________" << std::endl;
  for( int i=0; i<NITEMS; i++) {
    if( !(ItemDefs[i].flags & ItemType::DISABLED)) {
      std::cout << i << "- " << ItemDefs[i].name << ":" << ItemDefs[i].abr << std::endl;
    }
    else {
      std::cout << "****" << i << "- " << ItemDefs[i].name << ":" << ItemDefs[i].abr << std::endl;
    }
  }
}
void list_skills()
{
  // Every enables items
  std::cout << "__SKILLS________________" << std::endl;
  for( int i=0; i<NSKILLS; i++) {
    if( !(SkillDefs[i].flags & ItemType::DISABLED)) {
      std::cout << i << "- " << SkillDefs[i].name << ":" << SkillDefs[i].abbr << std::endl;
    }
    else {
      std::cout << "****" << i << "- " << SkillDefs[i].name << ":" << SkillDefs[i].abbr << std::endl;
    }
  }
}
void list_objects()
{
  // Every enables items
  std::cout << "__OBJECTS_______________" << std::endl;
  for( int i=0; i<NOBJECTS; i++) {
    if( !(ObjectDefs[i].flags & ItemType::DISABLED)) {
      std::cout << i << "- " << ObjectDefs[i].name << std::endl;
    }
    else {
      std::cout << "****" << i << "- " << ObjectDefs[i].name << std::endl;
    }
  }
}
void list_terrain()
{
  // Every enables items
  std::cout << "__TERRAIN_______________" << std::endl;
  for( int i=0; i<R_NUM; i++) {
    if( !(TerrainDefs[i].flags & ItemType::DISABLED)) {
      std::cout << i << "- " << TerrainDefs[i].name << ":" << TerrainDefs[i].type << std::endl;
    }
    else {
      std::cout << "****" << i << TerrainDefs[i].name << ":" << TerrainDefs[i].type << std::endl;
    }
    std::cout << "  small=";print_item( TerrainDefs[i].smallmon );std::cout << std::endl;
    std::cout << "  big  =";print_item( TerrainDefs[i].bigmon );std::cout << std::endl;
    std::cout << "  huma =";print_item( TerrainDefs[i].humanoid );std::cout << std::endl;
    std::cout << "  lair =";
    for( int j=0; j<6; j++ ) {
      int id_lair = TerrainDefs[i].lairs[j];
      if( id_lair >= 0 ) {
	std::cout <<  ObjectDefs[id_lair].name << ", ";
      }
      else {
	std::cout << "-1, ";
      }
    }
      std::cout << std::endl;
  }
}
void list_elements()
{
  //Game game;
  initIO();

    _game.ModifyTablesPerRuleset();

  MapAccess mapAccess = MapAccess( &_game );

  // list_skills();
  // list_objects();
  list_terrain();
}
int main(int argc, char *argv[])
{
  list_elements();
  
  return 0;
}

