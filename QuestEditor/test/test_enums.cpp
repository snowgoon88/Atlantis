/* -*- coding: utf-8 -*- */

/** 
 * test various classes
 */
#include <all_data.h>
#include <iostream>

// ************************************************************ test_enumitems
void test_enumitems()
{
  AllData _data;
  _data.parse_gamedata( "../.." );

  for( auto& elem: _data._all_enumitems ) {
    std::cout << elem.first << " = " << elem.second  << std::endl;
  }
}
// **************************************************************** test_items
void test_items()
{
  AllData _data;
  _data.parse_gamedata( "../.." );

  std::cout << "__TEST_ITEMS" << std::endl;
  for( auto& elem: _data._all_items ) {
    std::cout << elem.first << " = " << elem.second._item_enum  << std::endl;
	elem.second.write_debug( std::cout );
	std::cout << std::endl;
  }
}
// ************************************************************* test_monsters
void test_monsters()
{
  AllData _data;
  _data.parse_gamedata( "../.." );

  std::cout << "__TEST_MONSTERS" << std::endl;
  for( auto& elem: _data._all_monsters ) {
    std::cout << elem.first << " = " << elem.second._item_enum  << std::endl;
	elem.second.write_debug( std::cout );
	std::cout << std::endl;
  }
}
// ********************************************************** test_new_monster
void test_new_monster()
{
  AllData _data;
  _data.parse_gamedata( "../.." );

  // Create new Monster
  auto monster = _data.make_new_monster();
  //assert( monster != nullptr );
  if( monster == nullptr ) {
	std::cout << "ERROR: test_new_monster not created" << std::endl;
	exit(1);
  }
  monster->write_debug( std::cout );
  for( auto& elem: _data._all_enumitems ) {
    std::cout << elem.first << " = " << elem.second  << std::endl;
  }

  // Create duplicate
  monster = _data.make_new_monster();
  if( monster != nullptr ) {
	std::cout << "ERROR: test_new_monster should not created" << std::endl;
	exit(1);
  }
  else {
	std::cout << "OK, duplicate are detected" << std::endl;
  }

  // Create different
  monster = _data.make_new_monster( "I_BIDULE2" );
  //assert( monster != nullptr );
  if( monster == nullptr ) {
	std::cout << "ERROR: test_new_monster not created" << std::endl;
	exit(1);
  }
  monster->write_debug( std::cout );
  for( auto& elem: _data._all_enumitems ) {
    std::cout << elem.first << " = " << elem.second  << std::endl;
  }
}
// ***************************************************************************
// ********************************************************************** main
int main(int argc, char *argv[])
{
  // test_enumitems();
  // test_items();
  // test_monsters();
  test_new_monster();
  
  return 0;
}
// ***************************************************************************
