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
  //static AMonster* find_item( int id_item );

  /** Add elements */
  void add( const std::string& str_enum, int id_item )
  {
    ItemType* monster_item = &(ItemDefs[id_item]);
    
    AMonster monster;
    monster._fg_edited = false;
    monster._item_enum = str_enum;
    monster._item_id = id_item;
    monster._item = monster_item;
    // look for monster in MonDefs
    if( monster_item->abr != NULL ) {
      std::string tag = ( monster_item->type & IT_ILLUSION ? "i" : "");
      tag += monster_item->abr;

      for (int i = 0; i < NUMMONSTERS; i++) {
	if (MonDefs[i].abbr == NULL) continue;
	if (tag == MonDefs[i].abbr) {
	  monster._mtype_id = i;
	  monster._mtype = &(MonDefs[i]);
	  break;
	}
      }
    }

    // Add
    _map_item[id_item] = monster;
  }
};
// ******************************************************** end of MonsterData



#endif // MONSTERDATA_HPP
