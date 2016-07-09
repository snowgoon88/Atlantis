/* -*- coding: utf-8 -*- */

/** 
 * - Open a Game ("game.in")
 * - display the levels (ARegionArray)
 */
#include <game.h>
#include <map_access.h>
#include <stdlib.h>     /* exit, EXIT_FAILURE */

#include <algorithm>    // std::random_shuffle
#include <vector>       // std::vector

int _xsize_surf = 0;
int _ysize_surf = 0;
int _nb_gates_ini = 0;
int _max_underworld = 0;
int _nb_underdeep = 0;
int _nb_abyss = 0;
bool _modified = false;
bool _saved = false;
Game _game;

// ********************************************************* analyse_situation
int nb_underworld() { return (_max_underworld - 1); }
void analyse_situation( Game* game )
{

  MapAccess mapAccess = MapAccess( game );
  ARegionList* regions = mapAccess.regions();

  // list all the levels
  // and count levels
  for (int i = 0; i < regions->numLevels; i++) {
    ARegionArray *pArr = regions->pRegionArrays[i];
    std::cout << "{"<<i<<"} " << mapAccess.str_display( pArr ) << std::endl;
    //std::cout << mapAccess.list_regions( pArr ) << std::endl;
    if( pArr->levelType == ARegionArray::LEVEL_UNDERWORLD ) {
      _max_underworld = i;
    }
    if( pArr->levelType == ARegionArray::LEVEL_UNDERDEEP ) {
      _nb_underdeep = i - _max_underworld;
    }
    if( pArr->levelType == ARegionArray::LEVEL_NEXUS and i > 0 ) {
      _nb_abyss = i - (_max_underworld+_nb_underdeep);
    }
  }
  _xsize_surf = regions->pRegionArrays[1]->x;
  _ysize_surf = regions->pRegionArrays[1]->y;
  
  std::cout << nb_underworld() << " Underworld level(s) : ";
  if( nb_underworld() > 0 ) 
    std::cout << "from 2 to " << _max_underworld;
  std::cout << std::endl;
  std::cout << _nb_underdeep << " Underdeep level(s) : ";
  if( _nb_underdeep > 0) 
    std::cout << "from " << _max_underworld+1 << " to " << _max_underworld+_nb_underdeep;
  std::cout << std::endl;
  std::cout << (_nb_abyss) << " Abyss level(s) : ";
  if( _nb_abyss > 0 ) {
    std::cout << "from " << _max_underworld+_nb_underdeep+1;
    std::cout << " to " << _max_underworld+_nb_underdeep+_nb_abyss;
  }
  std::cout << std::endl;
}
// ************************************************************** make_choice
/** 
 * Return true when finished.
 */
void make_choice( MapAccess& ma )
{
  bool finished = false;
  std::string choice = "";

  while( finished == false ) {
    // nb gates
    std::cout << ma.regions()->numberofgates - _nb_gates_ini << " gates a finaliser" << std::endl;
    if( _nb_underdeep == 0 ) {
      std::cout << "  W) Intercale underWorld at level " << _max_underworld+1;
      std::cout << " size=" << _xsize_surf/2 << " x " << _ysize_surf/2;
      std::cout << std::endl;
    }
    std::cout << "  D) Intercale underDeep at level " << _max_underworld+_nb_underdeep+1;
    std::cout << std::endl;

    std::cout << "  G) Finalise les Gates" << std::endl;
    std::cout << "  S) Save in game.out" << std::endl;
    std::cout << "  Q) Quit" << std::endl;

    std::cout << "Quel choix : ";
    std::cin >> choice;
    std::cout << "-"<<choice<<"-" << std::endl;

    if( (choice[0] == 'w' || choice[0] == 'W') && _nb_underdeep == 0 ) {
      std::cout << "add_underworld( _max_underworld+1 )";
      std::cout << " [" << _max_underworld+1 << "]" << std::endl;
      ma.AddUnderWorldLevel( _max_underworld+1,
			     _xsize_surf/2, _ysize_surf/2,
			     _max_underworld );
      finished = true;
      _modified = true;
    }
    else if( choice[0] == 'd' || choice[0] == 'D' ) {
      std::cout << "add_underdeep( _max_underworld+_nb_underdeep+1 )";
      std::cout << " [" << _max_underworld+_nb_underdeep+1 << "]" << std::endl;
      finished = true;
      _modified = true;
    }
    else if( (choice[0] == 'g' || choice[0] == 'G') &&
	     ((ma.regions()->numberofgates - _nb_gates_ini) > 0)) {
      std::cout << "reshuffle_gates(" << _nb_gates_ini << ", " << ma.regions()->numberofgates - _nb_gates_ini << ")" << std::endl;
      ma.reshuffle_gates( _nb_gates_ini,
			  ma.regions()->numberofgates - _nb_gates_ini);
      _nb_gates_ini = ma.nb_gate();
      finished = true;
      _modified = true;
    }
    else if( choice[0] == 's' || choice[0] == 'S' ) {
      std::cout << "save_game()" << std::endl;
      if ( !_game.SaveGame() ) {
	Awrite( "Couldn't save the game!" );
      }
      else {
	_saved = true;
      }
      finished = true;
    }
    else if( choice[0] == 'q' || choice[0] == 'Q' ) {

      bool answered = false;
      while( answered == false ) {
	if( (ma.regions()->numberofgates - _nb_gates_ini) > 0 ) {
	  std::cout << "Il y a des Gates à finaliser" << std::endl;
	}
	if( _modified == true && _saved == false ) {
	  std::cout << "Game est modifie mais pas sauve.";
	}
	std::cout << " Voulez vous vraiment quitter (o/n) ? " << std::endl;
	std::cin >> choice;

	if( choice[0] == 'o' || choice[0] == 'O' ) {
	  std::cout << "Au revoir..." << std::endl;
	  exit(0);
	}
	else if( choice[0] == 'n' || choice[0] == 'N' ) {
	  answered = true;
	}
      }
    }
  }
}
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
// ************************************************************** create_level
void create_level()
{
  Game game;
  initIO();

  game.ModifyTablesPerRuleset();

  MapAccess mapAccess = MapAccess( &game );

  //mapAccess.createUnderDeep();
  mapAccess.createUnderWorld();
}
// ********************************************************************** menu
void menu()
{
  //Game game;
  initIO();

  _game.ModifyTablesPerRuleset();

  if ( !_game.OpenGame() ) {
    Awrite( "Couldn't open the game file!" );
    exit(1);
  }
  MapAccess mapAccess = MapAccess( &_game );
  _nb_gates_ini = mapAccess.nb_gate();
  std::cout << "__Nb de Gate initial=" << _nb_gates_ini << std::endl;
  std::cout << "  mapAccess.regions.nbgates="<< mapAccess.regions()->numberofgates << std::endl;

  while(true) {
    analyse_situation( &_game );
    make_choice( mapAccess );
  }
}
// ******************************************************************* shuffle
void shuffle()
{
  int nb_gate = 20;
  std::vector<int> vec;

  for( unsigned int i = 1; i <= nb_gate; ++i) {
    vec.push_back( i );
  }
  // print out content:
  std::cout << "__START vec:";
  for (std::vector<int>::iterator it=vec.begin(); it!=vec.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  // using built-in random generator:
  std::random_shuffle ( vec.begin()+10, vec.end() );

  // print out content:
  std::cout << "__SHUFFLE vec:";
  for (std::vector<int>::iterator it=vec.begin(); it!=vec.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  for( unsigned int i = 0; i < vec.size(); ++i) {
    std::cout << "G_"<<(i+1)<<" -> "<<"G_"<<vec[i] << std::endl;
  }
}

// ********************************************************************** main
int main(int argc, char *argv[])
{
  shuffle();
  menu();
  
  return 0;
}
