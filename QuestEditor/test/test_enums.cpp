/* -*- coding: utf-8 -*- */

/** 
 * test various classes
 */
#include <enum_data.h>
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
// ***************************************************************************
// ********************************************************************** main
int main(int argc, char *argv[])
{
  test_enumitems();
  return 0;
}
// ***************************************************************************
