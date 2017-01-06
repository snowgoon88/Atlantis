/* -*- coding: utf-8 -*- */

#ifndef ALLDATA_H
#define ALLDATA_H

/** 
 * Gather all game data from gamedata.h/cpp and
 * build new files from structures.
 */
#include <string>
#include <iostream>

#include <monster_data.h>
#include <item_data.h>
#include <map>


// ***************************************************************************
// ******************************************************************* AllData
// ***************************************************************************
class AllData
{
public:
  /** 
   * Parse gamedata for:
   *   - I_MONSTER and MonDefs,
   *   - I_ITEMS ans (WeaponsDefs or ArmorDefs or MountDefs)
   */
  void parse_gamedata( const std::string& path );
  void write_gamedata( bool fg_debug = false );

  /** Add AMonster defined by its I_ITEMS and id_item */
  void add_monster( const std::string& str_enum, int id_item );
  /** Create and Add new AMonster with default values*/
  AMonster* make_new_monster( const std::string& item_str = "I_BIDULE" );
  /** find a Monster with its id of MonDefs */
  AMonster* find_monster( int id_mtype );

  /** Add AItem defined by its I_ITEMS and id_item */
  void add_item( const std::string& str_enum, int id_item );
  AItem* make_new_item( const std::string& item_str = "I_MACHIN" );
  AItem* find_weapon( int id_wtype );
  AItem* find_armor( int id_atype );
  
  
public:
  std::map<std::string,int> _all_enumitems;
  std::map<int,AMonster> _all_monsters;
  std::map<int,AItem>    _all_items;

private:
  /** name of input path */
  std::string _input_path;
  /** parse gamedata.cpp looking for next item */
  std::string parse_to_next_item( std::istream& in, std::ostream& out,
				  const std::string& startstr,
				  bool fg_copy );
  /** copy a monster from in to out */
  void copy_item( std::istream& in, std::ostream& out,
		  const std::string& first_line, bool fg_copy );
  /** check if an enum already exists in _all_enumitems */
  bool exist_enum( const std::string& strenum );
};
#endif // ALLDATA_H
