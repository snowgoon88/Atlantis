/* -*- coding: utf-8 -*- */

/** 
 * test various classes
 */
#include <enum_data.h>
#include <item_data.h>
#include <iostream>

// ************************************************************ test_enumitems
void test_enumitems()
{
  EnumData _enums;
  _enums.parse_enumitems( "../.." );

  for( auto& elem: _enums._map_items ) {
    std::cout << elem.first << " = " << elem.second  << std::endl;
  }
}
// **************************************************************** test_items
void test_items()
{
  ItemData _item_data;
  _item_data.parse_gamedata( "../.." );

  std::cout << "__TEST_ITEMS" << std::endl;
  for( auto& elem: _item_data._map_item ) {
    std::cout << elem.first << " = " << elem.second._item_enum  << std::endl;
	elem.second.write_debug( std::cout );
	std::cout << std::endl;
  }
}
// ***************************************************************************
// ********************************************************************** main
int main(int argc, char *argv[])
{
  //test_enumitems();
  test_items();
  
  return 0;
}
// ***************************************************************************
