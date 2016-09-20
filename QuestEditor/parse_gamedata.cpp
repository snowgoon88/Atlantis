/* -*- coding: utf-8 -*- */

/**
 * Read gamedata.hpp to identify Items.
 */
#include <iostream>                   // std::cout, std::istream
#include <sstream>                   // std::istringstream
#include <fstream>                   // std::files

#include <gamedata.h>
#include <items.h>
#include <map>
#include <monster_data.h>
#include <utils.h>

// ***************************************************************************
// ******************************************************************* GLOBALS
MonsterData _monster_data;

// ***************************************************************************
// **************************************************************** parse_item
void parse_items()
{
  std::ifstream cpp_file( "mygamedata.cpp" );

  
  std::string line;
  while (std::getline(cpp_file, line)) {
    // std::cout << "READ: " << line << std::endl;
    // First non-blanc must be '{'
    auto it = line.begin();
    while( *it == ' ' or *it == '\t') {
      // std::cout << "/" << *it;
      it++;
    }
    //std::cout << std::endl;
    if( *it == '{' ) {
      it++;
      if( *it == '"' ) {
	std::cout << "__FOUND " << std::string( it, line.end() ) << std::endl;
      }
    }
  }
}
std::string find_next_item( std::istream& in )
{
  std::string line;
  while (std::getline(in, line)) {
    // std::cout << "READ: " << line << std::endl;
    // First non-blanc must be '{'
    auto it = line.begin();
    while( *it == ' ' or *it == '\t') {
      // std::cout << "/" << *it;
      it++;
    }
    //std::cout << std::endl;
    if( *it == '{' ) {
      it++;
      if( *it == '"' ) {
	std::cout << "__START " << std::string( it, line.end() ) << std::endl;
	return line;
      }
    }
  }
  return "";
}
std::string find_next_enditem( std::istream& in )
{
  std::string line;
  while (std::getline(in, line)) {
    // std::cout << "READ: " << line << std::endl;
    // First non-blanc must be '"'
    auto it = line.begin();
    while( *it == ' ' or *it == '\t') {
      // std::cout << "/" << *it;
      it++;
    }
    //std::cout << std::endl;
    if( *it == '"' ) {
      std::cout << "__END  " << std::string( it, line.end() ) << std::endl;
      return line;
    }
  }
  return "";
}
void parse_gamedata()
{
  std::ifstream h_in( "mygamedata.h" );
  std::ifstream cpp_in( "mygamedata.cpp" );

  std::string line;
  int idx_monster = 0;
  while( std::getline( h_in, line )) {
    //std::cout << "[" << line[0] << "] " << line << std::endl;
    if( line[0] == '\t' && line[1] == 'I' && line[2] == '_' ) {
      std::cout << "__ITEM: " << line.substr( 1 ) << std::endl;
      find_next_item( cpp_in );
      find_next_enditem( cpp_in );
      
      if( ItemDefs[idx_monster].type & IT_MONSTER ) {
	std::cout << "  + found monster " << line.substr( 1, line.size()-2 ) << ":" << ItemDefs[idx_monster].abr << std::endl;
	//write_monster_item( std::cout, ItemDefs[idx_monster] );

	_monster_data.add( line.substr( 1, line.size()-2 ), idx_monster );
	// //_map_abbr[line.substr( 1, line.size()-2 )] = idx_monster;
	// MonType* mtype = FindMonster( ItemDefs[idx_monster].abr,
	// 			      ItemDefs[idx_monster].type & IT_ILLUSION );
	// if( mtype != NULL ) {
	//   write_monster_type( std::cout, *mtype );
	// }
	// else {
	//   std::cout << "  NO Monster Type" << std::endl;
	// }
      }
      idx_monster ++;
    }
  }
  _max_item_id = NITEMS;
  _max_mtype_id = NUMMONSTERS;
}

// ***************************************************************************
// ********************************************************************** main
int main(int argc, char *argv[])
{
  read_itemtype_enum();
  //parse_gamedata();
  _monster_data.parse_gamedata();

  for( auto& monster : _monster_data._map_item ) {
    monster.second.write_debug();
    // std::cout << "** " << monster.second._item_enum << std::endl;
    // monster.second.write_item( std::cout );
    // monster.second.write_type( std::cout );
  }

  _monster_data.make_new();

  _monster_data.write_gamedata();

  // auto res = split( "Coucou c'est moi c est" );
  // for(auto& str : res) {
  //   std::cout << "FOUND: " << str << std::endl;
  // }

  // for (auto it=_map_itype.begin(); it!=_map_itype.end(); ++it)
  //   std::cout << it->first << " => " << it->second << '\n';
  
  return 0;
}

