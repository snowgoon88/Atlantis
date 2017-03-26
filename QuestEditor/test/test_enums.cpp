/* -*- coding: utf-8 -*- */

/** 
 * test various classes
 */
#include <all_data.h>
#include <iostream>
#include <utils.h>

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
// ************************************************************* test_new_item
void test_new_item()
{
  AllData _data;
  _data.parse_gamedata( "../.." );

  // Create new Item
  auto item = _data.make_new_item();
  //assert( item != nullptr );
  if( item == nullptr ) {
	std::cout << "ERROR: test_new_item not created" << std::endl;
	exit(1);
  }
  item->write_debug( std::cout );
  for( auto& elem: _data._all_enumitems ) {
    std::cout << elem.first << " = " << elem.second  << std::endl;
  }

  // Create duplicate
  item = _data.make_new_item();
  if( item != nullptr ) {
	std::cout << "ERROR: test_new_item should not created" << std::endl;
	exit(1);
  }
  else {
	std::cout << "OK, duplicate are detected" << std::endl;
  }

  // Create different
  item = _data.make_new_item( "I_MACHIN2" );
  //assert( item != nullptr );
  if( item == nullptr ) {
	std::cout << "ERROR: test_new_item not created" << std::endl;
	exit(1);
  }
  item->write_debug( std::cout );
  for( auto& elem: _data._all_enumitems ) {
    std::cout << elem.first << " = " << elem.second  << std::endl;
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
// **************************************************************** test_write
void test_write()
{
  AllData _data;
  _data.parse_gamedata( "../.." );

  // Create Monster, then Item, then Monster
  auto m1 = _data.make_new_monster( "I_MONSTRE1" );
  auto i1 = _data.make_new_item( "I_ITEM1" );
  // its a weapon
  i1->switch_weapon();
  auto i2 = _data.make_new_item( "I_ITEM2" );
  // its an armor (not weapon)
  i2->switch_weapon();
  i2->switch_armor();
  auto m2 = _data.make_new_monster( "I_MONSTRE2" );
  auto i3 = _data.make_new_item( "I_ITEM3" );
  // its a mount
  i3->switch_armor();
  i3->switch_weapon();
  i3->switch_armor();
  i3->switch_mount();
  i3->switch_armor();
  i3->switch_armor();
  i3->switch_mount();
  i3->write_debug( std::cout );
  i3->switch_battle( true );
  i3->write_debug( std::cout );
  i3->switch_battle( false );
  i3->write_debug( std::cout );
  i3->switch_battle( true );
  
  _data.write_gamedata( true );
}
// ***************************************************************************
// ********************************************************************** main
int main(int argc, char *argv[])
{
  // test_enumitems();
  // test_items();
  // test_new_item();
  // test_monsters();
  // test_new_monster();
  test_write();
  
  return 0;
}
// ***************************************************************************
