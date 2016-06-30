/* -*- coding: utf-8 -*- */

/** 
 * - Open a Game ("game.in")
 * - display the levels (ARegionArray)
 */
#include <game.h>
#include <map_access.h>
#include <stdlib.h>     /* exit, EXIT_FAILURE */



// ****************************************************************** addlevel
void addlevel()
{
  Game game;
  initIO();

  game.ModifyTablesPerRuleset();

  if ( !game.OpenGame() ) {
    Awrite( "Couldn't open the game file!" );
    exit(1);
  }

  MapAccess mapAccess = MapAccess( &game );
  ARegionList* regions = mapAccess.regions();

  int max_underground = 1;
  int max_underdeep = 0;
  int max_abyss = 0;
  
  // list all the levels
  // and count levels
  for (int i = 0; i < regions->numLevels; i++) {
    ARegionArray *pArr = regions->pRegionArrays[i];
    std::cout << mapAccess.str_display( pArr ) << std::endl;
    std::cout << mapAccess.list_regions( pArr ) << std::endl;
    if( pArr->levelType == ARegionArray::LEVEL_UNDERWORLD ) {
      max_underground = i;
    }
    if( pArr->levelType == ARegionArray::LEVEL_UNDERDEEP ) {
      max_underdeep = i - max_underground;
    }
    if( pArr->levelType == ARegionArray::LEVEL_NEXUS and i > 0 ) {
      max_abyss = i - (max_underground+max_underdeep);
    }
  }
  std::cout << (max_underground-1) << " Underground level(s) : ";
  if( max_underground-1 > 0 ) 
    std::cout << "from 2 to " << max_underground;
  std::cout << std::endl;
  std::cout << max_underdeep << " Underdeep level(s) : ";
  if( max_underdeep > 0) 
    std::cout << "from " << max_underground+1 << " to " << max_underground+max_underdeep;
  std::cout << std::endl;
  std::cout << (max_abyss) << " Abyss level(s) : ";
  if( max_abyss > 0 ) {
    std::cout << "from " << max_underground+max_underdeep+1;
    std::cout << " to " << max_underground+max_underdeep+max_abyss;
  }
  std::cout << std::endl;
  
  // // list all the regions
  // forlist( regions ) {
  //   ARegion *reg = (ARegion *) elem;
  //   std::cout << mapAccess.str_display( *reg ) << std::endl;
  // }

  // std::cout << "__ Add ABYSS at 3 " << std::endl;
  // mapAccess.AddAbyssLevel( 3 );

  std::cout << "__ Add Underdeep at 4 " << std::endl;
  int level = 3;
  ARegionArray* pArr = regions->pRegionArrays[1]; // surface level
  // size scale
  int sx, sy;
  if( level == max_underground + 1 ) { // first underdeep
    sx = 2; sy = 4;
  }
  else { // other underdeep
    sx = 4; sy = 4;
  }
  std::cout << "  Size will be " << pArr->x / sx << " x " << pArr->y / sy << std::endl;
  mapAccess.AddUnderDeepLevel( level, pArr->x/sx, pArr->y/sy,
			       max_underground );
  
  // regions = mapAccess.regions();
  // list all the levels
  for (int i = 0; i < regions->numLevels; i++) {
    ARegionArray *pArr = regions->pRegionArrays[i];
    std::cout << mapAccess.str_display( pArr ) << std::endl;
  }
  // // list all the regions
  // forlist( regions ) {
  //   ARegion *reg = (ARegion *) elem;
  //   std::cout << mapAccess.str_display( *reg ) << std::endl;
  // }
  
}
// ********************************************************************** main
int main(int argc, char *argv[])
{
  Game game;
  initIO();

  game.ModifyTablesPerRuleset();

  MapAccess mapAccess = MapAccess( &game );

  mapAccess.createUnderDeep();
  
  return 0;
}
