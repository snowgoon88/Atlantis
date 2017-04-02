/* -*- coding: utf-8 -*- */

/**
 * Reading, Storing and Writing gamedata.h/.cpp
 */
#include <all_data.h>
#include <utils.h>
#include <fstream>
#include <stdlib.h>

// ***************************************************************************
// ************************************************* AllData::parse_gamedata()
void AllData::parse_gamedata( const std::string& path )
{
  _input_path = path;
  std::ifstream h_in( path+"/gamedata.h" );
  //std::ifstream cpp_in( "mygamedata.cpp" );

  std::string line;
  int idx_item = 0;
  while( std::getline( h_in, line )) {
    //std::cout << "[" << line[0] << "] " << line << std::endl;

	// add enum **************************************************************
	if( line[0] == '\t' && line[1] == 'I' && line[2] == '_') {
	  // get the name of the enum
      auto tokens = split( line, ',' );
      std::string tok = tokens[0];
      //std::cout << "enum " << tok << ":" << idx_item << std::endl;
      _all_enumitems[tok] = idx_item;

	  // add monster *********************************************************
      if( ItemDefs[idx_item].type & IT_MONSTER ) {
		//std::cout << "  + found monster " << line.substr( 1, line.size()-2 ) << ":" << ItemDefs[idx_monster].abr << std::endl;
		add_monster( line.substr( 1, line.size()-2 ), idx_item );
      }
	  // add item ************************************************************
	  if( ((ItemDefs[idx_item].type & IT_MONSTER) == false ) &&
		  ((ItemDefs[idx_item].type & IT_MAN) == false ) &&
		  ((ItemDefs[idx_item].type & IT_MONEY) == false ) ) {
		//std::cout << "  + found monster " << line.substr( 1, line.size()-2 ) << ":" << ItemDefs[idx_monster].abr << std::endl;
		add_item( line.substr( 1, line.size()-2 ), idx_item );
	  }
	idx_item ++;
	}
  }

  _max_item_id = NITEMS;
  _max_mtype_id = NUMMONSTERS;
  _max_wtype_id = NUMWEAPONS;
  _max_atype_id = NUMARMORS;
  _max_mounttype_id = NUMMOUNTS;
  _max_battle_id = NUMBATTLEITEMS;
}
// ***************************************************************************
// ****************************************************** AllData::add_monster
void AllData::add_monster( const std::string& str_enum, int id_item )
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
  _all_monsters[id_item] = monster;
}
// ************************************************* AllData::make_new_monster
AMonster* AllData::make_new_monster( const std::string& item_str )
{
  // Check unique enum_str
  if( exist_enum( item_str )) {
	std::cout << "ERROR: make_new_monster, enum " << item_str << " already exists" << std::endl;
	return nullptr;
  }

  AMonster monster;
  monster._fg_edited = true;
  monster._item_enum = item_str;
  monster._item = new ItemType
    {"new monster", "new monsters", "TODO",
     ItemType::CANTGIVE,
     NULL,0,0,0, {{-1,0},{-1,0},{-1,0},{-1,0}},
     NULL,0,0, {{-1,0},{-1,0},{-1,0},{-1,0}},
     10, IT_MONSTER, 10,1,
     50,50,0,0,2,
     -1,0,
     -1,0,0,
     0, NULL, 0,
     "", { "", "", "", "" }, 0, 0};
  monster._item_id = _max_item_id++;
  monster._mtype = new MonType
    {1,{0,0,0,0,0,0},
     1,1,0,
     0,0,0,
     NULL,0,
     10, IT_NORMAL,10,1,"Un Truc Qui Griffe et Grogne", "TODO"};
  monster._mtype_id = _max_mtype_id++;

  _all_monsters[monster._item_id] = monster;
  _all_enumitems[item_str] = monster._item_id;

  //monster.write_debug();

  return &(_all_monsters[monster._item_id]);
}
// ***************************************************** AllData::find_monster
AMonster* AllData::find_monster( int id_mtype )
{
  for( auto& monster : _all_monsters ) {
    if( monster.second._mtype_id == id_mtype ) {
      return &(monster.second);
    }
  }
  return nullptr;
}
AMonster* AllData::find_monster( const std::string& item_enum )
{
    for( auto& monster : _all_monsters ) {
     if( monster.second._item_enum.compare(item_enum) == 0 ) {
       return &(monster.second);
     }
   }
   return nullptr;
}
// ***************************************************************************
// ********************************************************* AllData::add_item
void AllData::add_item( const std::string& str_enum, int id_item )
{
  ItemType* item_item = &(ItemDefs[id_item]);

  AItem item;
  item._fg_edited = false;
  item._item_enum = str_enum;
  item._item_id = id_item;
  item._item = item_item;

  // Look for weapon if needed
  if( item._item->type & IT_WEAPON ) {
	for( unsigned int i = 0; i < NUMWEAPONS; ++i) {
	  if( WeaponDefs[i].abbr == NULL ) continue;
	  if( item._item->abr == WeaponDefs[i].abbr ) {
		item._wtype_id = i;
		item._wtype = &(WeaponDefs[i]);
		break;
	  }
	}
  }
  // Look for armor if needed
  if( item._item->type & IT_ARMOR ) {
	for( unsigned int i = 0; i < NUMARMORS; ++i) {
	  if( ArmorDefs[i].abbr == NULL ) continue;
	  if( item._item->abr == ArmorDefs[i].abbr ) {
		item._atype_id = i;
		item._atype = &(ArmorDefs[i]);
		break;
	  }
	}
  }
  // Look for mount if needed
  if( item._item->type & IT_MOUNT ) {
	for( unsigned int i = 0; i < NUMMOUNTS; ++i) {
	  if( MountDefs[i].abbr == NULL ) continue;
	  if( item._item->abr == MountDefs[i].abbr ) {
		item._mtype_id = i;
		item._mtype = &(MountDefs[i]);
		break;
	  }
	}
  }
  // Look for battle if needed
  if( item._item->type & IT_BATTLE ) {
	for( unsigned int i = 0; i < NUMBATTLEITEMS; ++i) {
	  if( BattleItemDefs[i].abbr == NULL ) continue;
	  if( item._item->abr == BattleItemDefs[i].abbr ) {
		item._btype_id = i;
		item._btype = &(BattleItemDefs[i]);
		break;
	  }
	}
  }

  // Add
  _all_items[id_item] = item;
}
// **************************************************** AllData::make_new_item
AItem* AllData::make_new_item( const std::string& item_str )
{
  // Check unique enum_str
  if( exist_enum( item_str )) {
	std::cout << "ERROR: make_new_item, enum " << item_str << " already exists" << std::endl;
	return nullptr;
  }

  AItem item;
  item._fg_edited = true;
  item._item_enum = item_str;
  item._item = new ItemType
    {"new item", "new items", "TODO",
	 0,
     NULL,0,0,0, {{-1,0},{-1,0},{-1,0},{-1,0}},
     NULL,0,0, {{-1,0},{-1,0},{-1,0},{-1,0}},
     1, IT_NORMAL, 10,0,
     0,0,0,0,0,
     -1,0,
     -1,0,0,
     0, NULL, 0,
     "", { "", "", "", "" }, 0, 0};
  item._item_id = _max_item_id++;

  _all_items[item._item_id] = item;
  _all_enumitems[item_str] = item._item_id;

  //item.write_debug();

  return &(_all_items[item._item_id]);
}
// ******************************************************** AllData::find_item
AItem* AllData::find_item( const std::string& item_enum )
{
    for( auto& item : _all_items ) {
     if( item.second._item_enum.compare(item_enum) == 0 ) {
       return &(item.second);
     }
   }
   return nullptr;
}
// ****************************************************** AllData::find_weapon
AItem* AllData::find_weapon( int id_wtype )
{
  for( auto& item : _all_items ) {
    if( item.second._wtype_id == id_wtype ) {
      return &(item.second);
    }
  }
  return nullptr;
}
AItem* AllData::find_armor( int id_atype )
{
  for( auto& item : _all_items ) {
    if( item.second._atype_id == id_atype ) {
      return &(item.second);
    }
  }
  return nullptr;
}
AItem* AllData::find_mount( int id_mtype )
{
  for( auto& item : _all_items ) {
    if( item.second._mtype_id == id_mtype ) {
      return &(item.second);
    }
  }
  return nullptr;
}
AItem* AllData::find_battle( int id_btype )
{
  for( auto& item : _all_items ) {
    if( item.second._btype_id == id_btype ) {
      return &(item.second);
    }
  }
  return nullptr;
}
// ***************************************************************************
// ************************************************* AllData::write_gamedata()
void AllData::write_gamedata( bool fg_debug )
{
    std::ifstream h_in( _input_path+"/gamedata.h" );
    std::ifstream cpp_in( _input_path+"/gamedata.cpp" );

    std::ofstream h_out( "gamedata.h.new" );
    std::ofstream cpp_out( "gamedata.cpp.new" );

    bool must_copy = true;
    int idx_monster = 0;
	int idx_our_items = 0;
    int idx_item = 0;
    std::string line;

    // Parse cpp_in to first item struct
    if( fg_debug) cpp_out << "// DEBUG parse to first struct" << std::endl;
    std::string item_line = parse_to_next_item( cpp_in, cpp_out,
						  "{", must_copy );
    cpp_out << item_line << std::endl;

  // Deal with Items
  while( std::getline( h_in, line )) {
    // copy to new
    h_out << line << std::endl;
    if( line[0] == '\t' && line[1] == 'I' && line[2] == '_' ) {
      // found an Item in .h
      std::cout << "ITEM " << idx_item << "/" << NITEMS << std::endl;
      // cpp_out << "// DEBUG parse to next item" << std::endl;
	  // std::string item_line = parse_to_next_item( cpp_in, cpp_out,
	  //						  "{", must_copy );
      // Is it a monster ?
      if( ItemDefs[idx_item].type & IT_MONSTER ) {
        if( _all_monsters[idx_item]._fg_edited ) {
		  // // Write new version if edited
		  std::cout << "write_gamedata: write new version of monster " << ItemDefs[idx_item].abr << std::endl;
		  if( fg_debug) cpp_out << "// DEBUG new version of monster " << std::endl;
		  _all_monsters[idx_item].write_item( cpp_out );
		  copy_item( cpp_in, cpp_out, "", false );
		  must_copy = false;
        }
        else {
		  // Else copy this monster
		  std::cout << "write_gamedata: copy current version of " << ItemDefs[idx_item].abr << std::endl;
		  std::cout << "      with : " << item_line << std::endl;
		  if( fg_debug) cpp_out << "// DEBUG copy monster" << std::endl;
		  // parse_to_next will do the job
		  copy_item( cpp_in, cpp_out, item_line, true );
		  must_copy = true;
        }
        idx_monster++;
      }
	  // Is it a real item (not man)?
	  else if( ((ItemDefs[idx_item].type & IT_MONSTER) == false ) &&
			   ((ItemDefs[idx_item].type & IT_MAN) == false ) &&
			   ((ItemDefs[idx_item].type & IT_MONEY) == false ) ) {
		if( _all_items[idx_item]._fg_edited ) {
		  // Write new version as edited
		  std::cout << "write_gamedata: write new version of item " << ItemDefs[idx_item].abr << std::endl;
		  if( fg_debug) cpp_out << "// DEBUG new version of item " << std::endl;
		  _all_items[idx_item].write_item( cpp_out );
		  copy_item( cpp_in, cpp_out, "", false );
		  must_copy = false;
		}
		else {
		  // Else copy this monster
		  std::cout << "write_gamedata: copy current version of " << ItemDefs[idx_item].abr << std::endl;
		  std::cout << "      with : " << item_line << std::endl;
		  if( fg_debug) cpp_out << "// DEBUG copy item" << std::endl;
		  // parse_to_next will do the job
		  copy_item( cpp_in, cpp_out, item_line, true );
		  must_copy = true;
        }
        idx_our_items++;
	  }
	  else {
        // copy
        //cpp_out << item_line << std::endl;
        if( fg_debug) cpp_out << "// DEBUG pas un monstre ou item" << std::endl;
        // parse to next will do the job
        copy_item( cpp_in, cpp_out, item_line, true );
        must_copy = true;
	  }
	  // if( item_line.compare( "" ) != 0 ) {
	  // 	// Check if edited :o)
	  // }

	  idx_item++;
	  // if last item...
	  if( idx_item >= NITEMS ) {
		// MUST write monsters and items in the right order...
		for (; idx_item < _max_item_id; ++idx_item) {
		  // Write element id=idx_item
		  for( auto& monster: _all_monsters) {
			if( monster.first == idx_item ) {
			  std::cout << "Write new monster " << monster.second._item_id << std::endl;
			  h_out << "\t" << monster.second._item_enum << "," << std::endl;
			  if( fg_debug) cpp_out << "// DEBUG new monster" << std::endl;
			  monster.second.write_item( cpp_out );
			  break;
			}
		  }
		  for( auto& item: _all_items) {
			if( item.first == idx_item ) {
			  std::cout << "Write new item " << item.second._item_id << std::endl;
			  h_out << "\t" << item.second._item_enum << "," << std::endl;
			  if( fg_debug) cpp_out << "// DEBUG new item" << std::endl;
			  item.second.write_item( cpp_out );
			  break;
			}
		  }
		}
		// get out of this while-loop
		break;
	  }
	}
  }

  // Copy to end of h_in
  std::string last_line = parse_to_next_item( h_in, h_out, "#endif", true);
  h_out << last_line << std::endl;

  // Copy to MonType in cpp
  if( fg_debug) cpp_out << "// DEBUG copy to MonType" << std::endl;
  std::string monster_line = parse_to_next_item( cpp_in, cpp_out,
					       "MonType md[]", true);
  cpp_out << monster_line << std::endl;
  // copy Monster None
  monster_line = parse_to_next_item( cpp_in, cpp_out,
				     "{", true);
  cpp_out << monster_line << std::endl;
  int idx_ittype = 1;
  must_copy = true;
  while( idx_ittype < NUMMONSTERS ) {
    // read to beginning
    monster_line = parse_to_next_item( cpp_in, cpp_out,
				       "{", must_copy );
    AMonster* a_monster = find_monster( idx_ittype );
    if( a_monster ) {
      if( a_monster->_fg_edited ) {
	std::cout << "write_gamedata: check if MonType is modified" << std::endl;
	if( fg_debug) cpp_out << "// DEBUG MonType is modified" << std::endl;
	a_monster->write_type( cpp_out );
	must_copy = false;
      }
      else {
	// copy
	if( fg_debug) cpp_out << "// DEBUG MonType is copied" << std::endl;
	cpp_out << monster_line << std::endl;
	must_copy = true;
      }
    }
    else {
      std::cerr << "write_gamedata : Error MonType id=" << idx_ittype << " not found" << std::endl;
      exit(1);
    }
    idx_ittype++;
  }
  // last monster
  if( fg_debug) cpp_out << "// DEBUG last MonType" << std::endl;
  monster_line = parse_to_next_item( cpp_in, cpp_out,
				     "};", must_copy );
  // New monsters
  std::cout << "write_gamedata: copy new MonType " << std::endl;
  for( auto& monster : _all_monsters ) {
    if( monster.second._mtype_id >= NUMMONSTERS ) {
      std::cout << "Write new monster " << monster.second._mtype_id << std::endl;
      if( fg_debug) cpp_out << "// DEBUG new MonType" << std::endl;
      monster.second.write_type( cpp_out );
    }
  }
  cpp_out << monster_line << std::endl;

  // ---------------------------------------------------------- WEAPON
  // copy to WeaponType
  if( fg_debug) cpp_out << "// DEBUG copy to WeaponType" << std::endl;
  std::string weapon_line = parse_to_next_item( cpp_in, cpp_out,
												"WeaponType wepd[]", true);
  cpp_out << weapon_line << std::endl;
  // copy Weapon None
  weapon_line = parse_to_next_item( cpp_in, cpp_out,
									"{", true);
  cpp_out << weapon_line << std::endl;
  idx_ittype = 1;
  must_copy = true;
  while( idx_ittype < NUMWEAPONS ) {
    // read to beginning
    weapon_line = parse_to_next_item( cpp_in, cpp_out,
									  "{", must_copy );
	AItem* a_item = find_weapon( idx_ittype );
    if( a_item ) {
      if( a_item->_fg_edited ) {
		std::cout << "write_gamedata: check if WeaponType is modified" << std::endl;
		if( fg_debug) cpp_out << "// DEBUG WeaponType is modified" << std::endl;
		a_item->write_type_weapon( cpp_out );
		must_copy = false;
      }
      else {
		// copy
		if( fg_debug) cpp_out << "// DEBUG WeaponType is copied" << std::endl;
		cpp_out << weapon_line << std::endl;
		must_copy = true;
      }
    }
	else {
	  // copy
	  if( fg_debug) cpp_out << "// DEBUG WeaponType is copied" << std::endl;
	  cpp_out << weapon_line << std::endl;
	  must_copy = true;
	}

    idx_ittype++;
  }
  // last weapon
  if( fg_debug) cpp_out << "// DEBUG last WeaponType" << std::endl;
  weapon_line = parse_to_next_item( cpp_in, cpp_out,
									"};", must_copy );
  // New Weapons
  std::cout << "write_gamedata: copy new WeaponType " << std::endl;
  for( auto& item : _all_items ) {
	// write if a NEW Weapon AND is really a weapon
	if( (item.second._item->type & IT_WEAPON) &&
		(item.second._wtype_id >= NUMWEAPONS) ) {
      std::cout << "Write new weapon " << item.second._wtype_id << std::endl;
      if( fg_debug) cpp_out << "// DEBUG new WeaponType" << std::endl;
      item.second.write_type_weapon( cpp_out );
    }
  }
  cpp_out << weapon_line << std::endl;

  // ----------------------------------------------------------- ARMOR
  // copy to ArmorType
  if( fg_debug) cpp_out << "// DEBUG copy to ArmorType" << std::endl;
  std::string armor_line = parse_to_next_item( cpp_in, cpp_out,
											   "ArmorType armd[]", true);
  cpp_out << armor_line << std::endl;
  // copy Armor None
  armor_line = parse_to_next_item( cpp_in, cpp_out,
								   "{", true);
  cpp_out << armor_line << std::endl;
  idx_ittype = 1;
  must_copy = true;
  while( idx_ittype < NUMARMORS ) {
    // read to beginning
    armor_line = parse_to_next_item( cpp_in, cpp_out,
									 "{", must_copy );
	AItem* a_item = find_armor( idx_ittype );
    if( a_item ) {
      if( a_item->_fg_edited ) {
		std::cout << "write_gamedata: check if ArmorType is modified" << std::endl;
		if( fg_debug) cpp_out << "// DEBUG ArmorType is modified" << std::endl;
		a_item->write_type_armor( cpp_out );
		must_copy = false;
      }
      else {
		// copy
		if( fg_debug) cpp_out << "// DEBUG ArmorType is copied" << std::endl;
		cpp_out << armor_line << std::endl;
		must_copy = true;
      }
    }
	else {
	  // copy
	  if( fg_debug) cpp_out << "// DEBUG ArmorType is copied" << std::endl;
	  cpp_out << armor_line << std::endl;
	  must_copy = true;
	}

    idx_ittype++;
  }
  // last armor
  if( fg_debug) cpp_out << "// DEBUG last ArmorType" << std::endl;
  armor_line = parse_to_next_item( cpp_in, cpp_out,
								   "};", must_copy );
  // New Armors
  std::cout << "write_gamedata: copy new ArmorType " << std::endl;
  for( auto& item : _all_items ) {
    if( (item.second._item->type & IT_ARMOR) &&
		(item.second._atype_id >= NUMARMORS) ) {
      std::cout << "Write new armor " << item.second._atype_id << std::endl;
      if( fg_debug) cpp_out << "// DEBUG new ArmorType" << std::endl;
      item.second.write_type_armor( cpp_out );
    }
  }
  cpp_out << armor_line << std::endl;

  // ------------------------------------------------------------ MOUNT
  // copy to MountType
  if( fg_debug) cpp_out << "// DEBUG copy to MountType" << std::endl;
  std::string mount_line = parse_to_next_item( cpp_in, cpp_out,
											   "MountType mountd[]", true);
  cpp_out << mount_line << std::endl;
  // copy Mount None
  mount_line = parse_to_next_item( cpp_in, cpp_out,
								   "{", true);
  cpp_out << mount_line << std::endl;
  idx_ittype = 1;
  must_copy = true;
  while( idx_ittype < NUMMOUNTS ) {
    // read to beginning
    mount_line = parse_to_next_item( cpp_in, cpp_out,
									 "{", must_copy );
	AItem* a_item = find_mount( idx_ittype );
    if( a_item ) {
      if( a_item->_fg_edited ) {
		std::cout << "write_gamedata: check if MountType is modified" << std::endl;
		if( fg_debug) cpp_out << "// DEBUG MountType is modified" << std::endl;
		a_item->write_type_mount( cpp_out );
		must_copy = false;
      }
	  else {
	  // copy
	  if( fg_debug) cpp_out << "// DEBUG MountType is copied" << std::endl;
	  cpp_out << mount_line << std::endl;
	  must_copy = true;
	  }
	}
	else {
	  // copy
	  if( fg_debug) cpp_out << "// DEBUG MountType is copied" << std::endl;
	  cpp_out << mount_line << std::endl;
	  must_copy = true;
	}

    idx_ittype++;
  }
  // last ount
  if( fg_debug) cpp_out << "// DEBUG last MountType" << std::endl;
  mount_line = parse_to_next_item( cpp_in, cpp_out,
								   "};", must_copy );
  // New Mounts
  std::cout << "write_gamedata: copy new MountType " << std::endl;
  for( auto& item : _all_items ) {
    if( (item.second._item->type & IT_MOUNT) &&
		(item.second._mtype_id >= NUMMOUNTS) ) {
      std::cout << "Write new mount " << item.second._mtype_id << std::endl;
      if( fg_debug) cpp_out << "// DEBUG new MountType" << std::endl;
      item.second.write_type_mount( cpp_out );
    }
  }
  cpp_out << mount_line << std::endl;

  // ---------------------------------------------------------- BATTLE
  // copy to BattleItemType
  if( fg_debug) cpp_out << "// DEBUG copy to BattleItemType" << std::endl;
  std::string battle_line = parse_to_next_item( cpp_in, cpp_out,
											   "BattleItemType bitd[]", true);
  cpp_out << battle_line << std::endl;
  // copy BATTLE None
  battle_line = parse_to_next_item( cpp_in, cpp_out,
								   "{", true);
  cpp_out << battle_line << std::endl;
  idx_ittype = 1;
  must_copy = true;
  while( idx_ittype < NUMBATTLEITEMS ) {
    // read to beginning
    battle_line = parse_to_next_item( cpp_in, cpp_out,
									 "{", must_copy );
	AItem* a_item = find_battle( idx_ittype );
    if( a_item ) {
      if( a_item->_fg_edited ) {
		std::cout << "write_gamedata: check if BattleType is modified" << std::endl;
		if( fg_debug) cpp_out << "// DEBUG BattleType is modified" << std::endl;
		a_item->write_type_battle( cpp_out );
		must_copy = false;
      }
	  else {
		// copy
		if( fg_debug) cpp_out << "// DEBUG BattleType is copied" << std::endl;
		cpp_out << battle_line << std::endl;
		must_copy = true;
	  }
	}
	else {
	  // copy
	  if( fg_debug) cpp_out << "// DEBUG BattleType is copied" << std::endl;
	  cpp_out << battle_line << std::endl;
	  must_copy = true;
	}

    idx_ittype++;
  }
  // last BattleItem
  if( fg_debug) cpp_out << "// DEBUG last BattleItemType" << std::endl;
  battle_line = parse_to_next_item( cpp_in, cpp_out,
								   "};", must_copy );
  // New BattleItems
  std::cout << "write_gamedata: copy new BattleItemType " << std::endl;
  for( auto& item : _all_items ) {
    if( (item.second._item->type & IT_BATTLE) &&
		(item.second._btype_id >= NUMBATTLEITEMS) ) {
      std::cout << "Write new battle " << item.second._btype_id << std::endl;
      if( fg_debug) cpp_out << "// DEBUG new BattleItemType" << std::endl;
      item.second.write_type_battle( cpp_out );
    }
  }
  cpp_out << battle_line << std::endl;

  // Copy to end of cpp_in
  std::cout << "write_gamedata: copier fin de cpp_in dans cpp_out" << std::endl;
  if( fg_debug) cpp_out << "// DEBUG to end of cpp_in" << std::endl;
  last_line = parse_to_next_item( cpp_in, cpp_out, "int NUMATTRIBMODS", true);
  cpp_out << last_line << std::endl;

  h_in.close();
  cpp_in.close();
  h_out.close();
  cpp_out.close();
}
// ***************************************************************************
// *********************************************** AllData::parse_to_next_item
/**
 * Parse 'in' looking for the next item.
 * @param: fg_copy, are each line of 'in' copied to 'out'
 * @param: startstr first non ' ' or non '\t' is this string
 */
std::string AllData::parse_to_next_item( std::istream& in, std::ostream& out,
										 const std::string& startstr,
										 bool fg_copy )
{
  std::string line;
  while (std::getline(in, line)) {
    // std::cout << "READ: " << line << std::endl;
    // detect item = First non-blanc must be '{'
    auto it = line.begin();
    while( *it == ' ' or *it == '\t') {
      // std::cout << "/" << *it;
      it++;
    }
    //std::cout << std::endl;
    // Compare strippped line to startstr
    std::string stripped_str(it, it+startstr.length());
    if( stripped_str.compare(startstr) == 0 ) {
      std::cout << "__START " << std::string( it, line.end() ) << std::endl;
	return line;
    }
    // copy line juste read to 'out'
    if( fg_copy ) {
      out << line << std::endl;
    }
  }
  return "";
}
// ******************************************************** AllData::copy_item
/**
 * Copy every line until a line ends with "(not})}" or "(not)"t hat does not begins with '{'
 */
void AllData::copy_item( std::istream& in, std::ostream& out,
				const std::string& first_line, bool fg_copy )
{
  //out << first_line << std::endl;
  std::string line;
  while (std::getline(in, line)) {
        if( fg_copy ) {
            out << line << std::endl;
//            if( line.back() != ',')
//                out << ',';
//            out << std::endl;
        }
    // Ends with "'(not})}," ??
    auto found = line.rfind( "}" );
    if( found != std::string::npos ) {
        // check before is not "}"
        if( line[found-1] != '}') {
            return;
        }
    }
//    auto it = line.end();
//    // std::cout << "READ: " << line << std::endl;
//    // First non-blanc must be '{'
//    auto it = line.begin();
//    while( *it == ' ' or *it == '\t') {
//      // std::cout << "/" << *it;
//      it++;
//    }
//    //std::cout << std::endl;
//    if( *it == '{' ) {
//      std::cout << "__END  " << std::string( it, line.end() ) << std::endl;
//      return line;
//    }
//    out << line << std::endl;
  }
  return;
}
// ***************************************************************************
// AMonster* MonsterData::find_monster( const std::string& item_enum )
// {
//     for( auto& monster : _map_item ) {
//     if( monster.second._item_enum.compare(item_enum) == 0 ) {
//       return &(monster.second);
//     }
//   }
//   return nullptr;
// }
// ***************************************************************************
// ******************************************************* AllData::exist_enum
bool AllData::exist_enum( const std::string& strenum )
{
  for( auto& elem: _all_enumitems) {
    if( strenum.compare( elem.first ) == 0) {
	  return true;
	}
  }
  return false;
}

