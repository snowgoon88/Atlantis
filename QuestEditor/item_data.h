/* -*- coding: utf-8 -*- */

#ifndef ITEMDATA_HPP
#define ITEMDATA_HPP

/**
 * All data about a Item for Atlantis
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
class AItem
{
public:

  AItem();

  /** is edited */
  bool _fg_edited;

  /** ref to Item */
  std::string _item_enum;
  int _item_id;
  ItemType* _item;

  /** ref to Weapon */
  //std::string _weapon_enum;
  int _wtype_id;
  WeaponType* _wtype;

  /** Write functions */
  void write_item( std::ostream& out );
  void write_type_weapon( std::ostream& out );
  void write_debug( std::ostream& out = std::cout );
};
// ************************************************************** end of AItem

// ***************************************************************************
// ****************************************************************** ItemData
// ***************************************************************************
class ItemData
{
public:
  std::map<int,AItem> _map_item;

  /** Find AItem */
  AItem* find_item_weapon( int id_wtype );
  AItem* find_item( const std::string& item_enum );

  /** Parse gamedata for I_ITEM and Weapon/Armor/Mount/BattleDefs */
  void parse_gamedata( const std::string& path );
  //void write_gamedata( bool fg_debug = false );

  /** Add AItem defined by its I_ITEMS and id_item */
  void add( const std::string& str_enum, int id_item );
  /** Add new AItem with default values*/
  //AItem* make_new( const std::string& item_str = "I_MACHIN" );

 private:
  // /** name of input path */
  std::string _input_path;
  // /** parse gamedata.cpp looking for next item */
  // std::string parse_to_next_item( std::istream& in, std::ostream& out,
  // 				  const std::string& startstr,
  // 				  bool fg_copy );
  // /** copy a item from in to out */
  // void copy_item( std::istream& in, std::ostream& out,
  // 		  const std::string& first_line, bool fg_copy );
public:
  static std::string attack_enum[];
  static std::string attack_type[];
};
// ******************************************************** end of MonsterData



#endif // ITEMDATA_HPP
