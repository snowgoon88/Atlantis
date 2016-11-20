/* -*- coding: utf-8 -*- */

#ifndef MONSTERDATA_HPP
#define MONSTERDATA_HPP

/**
 * All data about a monster for Atlantis
 */

#include <string>
#include <gamedata.h>
#include <items.h>
#include <iostream>                   // std::cout, std::istream
#include <map>
#include <utils.h>

// ***************************************************************************
// ****************************************************************** AMonster
// ***************************************************************************
class AMonster
{
public:

  AMonster();

  /** is edited */
  bool _fg_edited;

  /** ref to Item */
  std::string _item_enum;
  int _item_id;
  ItemType* _item;

  /** ref to MonType */
  std::string _mtype_enum;
  int _mtype_id;
  MonType* _mtype;

  /** Write functions */
  void write_item( std::ostream& out );
  void write_type( std::ostream& out );
  void write_debug( std::ostream& out = std::cout );
};
// *********************************************************** end of AMonster

// ***************************************************************************
// *************************************************************** MonsterData
// ***************************************************************************
class MonsterData
{
public:
  std::map<int,AMonster> _map_item;

  /** Find AMonster */
  AMonster* find_monster( int id_mtype );
  AMonster* find_monster( const std::string& item_enum );

  /** Parse gamedata for I_MONSTER and MonDefs */
  void parse_gamedata( const std::string& path );
  void write_gamedata( bool fg_debug = false );

  /** Add AMonster defined by its I_ITEMS and id_item */
  void add( const std::string& str_enum, int id_item );
  /** Add new AMonster with default values*/
  AMonster* make_new( const std::string& item_str = "I_BIDULE" );

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
};
// ******************************************************** end of MonsterData



#endif // MONSTERDATA_HPP
