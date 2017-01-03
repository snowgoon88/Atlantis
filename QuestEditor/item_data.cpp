/* -*- coding: utf-8 -*- */

/**
 * TODO
 */
#include <item_data.h>
#include <utils.h>
#include <stdlib.h>

// ******************************************************** AMonster::AMonster
AItem::AItem() :
  _fg_edited(false), _item_enum(""), _item_id(-1), _item(nullptr),
  _wtype_id(-1), _wtype(nullptr)
{}
// ****************************************************** AMonster::write_item
void AItem::write_item( std::ostream& out )
{
  // Already written a flag on this line ?
  bool fg_before = false;

  // name, names, abr
  out << "\t{\"" << _item->name << "\", \"" << _item->names << "\", \"" << _item->abr << "\"," << std::endl;
  // flags
  out << "\t";
  fg_before = false;
  if( _item->flags & ItemType::CANTGIVE ) {
	if( fg_before ) out << " | ";
    out << "ItemType::CANTGIVE";
	fg_before = true;
  }
  if( _item->flags & ItemType::DISABLED ) {
    if( fg_before ) out << " | ";
    out << "ItemType::DISABLED";
	fg_before = true;
  }
  if( _item->flags & ItemType::NOMARKET ) {
    if( fg_before ) out << " | ";
    out << "ItemType::NOMARKET";
	fg_before = true;
  }
  if( _item->flags & ItemType::ORINPUTS ) {
    if( fg_before ) out << " | ";
    out << "ItemType::ORINPUTS";
	fg_before = true;
  }
  if( _item->flags & ItemType::SKILLOUT ) {
    if( fg_before ) out << " | ";
    out << "ItemType::SKILLOUT";
	fg_before = true;
  }
  if( _item->flags & ItemType::NOTRANSPORT ) {
    if( fg_before ) out << " | ";
    out << "ItemType::NOTRANSPORT";
	fg_before = true;
  }
  out << "," << std::endl;
  fg_before = false;
  // pSkill
  if( _item->pSkill == NULL ) {
    out << "\tNULL,0,0,0, {{-1,0},{-1,0},{-1,0},{-1,0}}," << std::endl;
  }
  else {
    out << "\t\"" << _item->pSkill << "\"," << _item->pMonths << "," << _item->pLevel << "," << _item->pOut << ", " << "{";
    for( int p=0; p < 4; ++p ) {
        if( p < _item->pOut ) {
            out << "{" << _item->pInput[p].item << "," << _item->pInput[p].amt << "}";
        }
        else {
            out << "{-1,0}";
        }
        if( p < 3) out << ",";
    }
    out << "}," << std::endl;
  }
  // mSkill
  if( _item->mSkill == NULL ) {
    out << "\tNULL,0,0, {{-1,0},{-1,0},{-1,0},{-1,0}}," << std::endl;
  }
  else {
    out << "\t\"" << _item->mSkill << "\"," << _item->mLevel << "," << _item->mOut << ", " << "{";
    for( int m=0; m < 4; ++m ) {
        if( m < _item->mOut ) {
            out << "{" << _item->mInput[m].item << "," << _item->mInput[m].amt << "}";
        }
        else {
            out << "{-1,0}";
        }
        if( m < 3) out << ",";
    }
    out << "}," << std::endl;
  }
  // weight
  out << "\t" << _item->weight << ",";
  // type
  fg_before = false;
  if( _item->type & IT_NORMAL ) {
    if( fg_before ) out << " |";
    out << " IT_NORMAL";
    fg_before = true;
  }
  if( _item->type & IT_ADVANCED ) {
    if( fg_before ) out << " |";
    out << " IT_ADVANCED";
    fg_before = true;
  }
  if( _item->type & IT_TRADE ) {
    if( fg_before ) out << " |";
    out << " IT_TRADE";
    fg_before = true;
  }
  if( _item->type & IT_MAN ) {
    if( fg_before ) out << " |";
    out << " IT_MAN";
    fg_before = true;
  }
  if( _item->type & IT_MONSTER ) {
    if( fg_before ) out << " |";
    out << " IT_MONSTER";
    fg_before = true;
  }
  if( _item->type & IT_MAGIC ) {
    if( fg_before ) out << " |";
    out << " IT_MAGIC";
    fg_before = true;
  }
  if( _item->type & IT_WEAPON ) {
    if( fg_before ) out << " |";
    out << " IT_WEAPON";
    fg_before = true;
  }
  if( _item->type & IT_ARMOR ) {
    if( fg_before ) out << " |";
    out << " IT_ARMOR";
    fg_before = true;
  }
  if( _item->type & IT_MOUNT ) {
    if( fg_before ) out << " |";
    out << " IT_MOUNT";
    fg_before = true;
  }
  if( _item->type & IT_BATTLE ) {
    if( fg_before ) out << " |";
    out << " IT_BATTLE";
    fg_before = true;
  }
  if( _item->type & IT_SPECIAL ) {
    if( fg_before ) out << " |";
    out << " IT_SPECIAL";
    fg_before = true;
  }
  if( _item->type & IT_TOOL ) {
    if( fg_before ) out << " |";
    out << " IT_TOOL";
    fg_before = true;
  }
  if( _item->type & IT_FOOD ) {
    if( fg_before ) out << " |";
    out << " IT_FOOD";
    fg_before = true;
  }
  if( _item->type & IT_ILLUSION ) {
    if( fg_before ) out << " |";
    out << " IT_ILLUSION";
    fg_before = true;
  }
  if( _item->type & IT_UNDEAD ) {
    if( fg_before ) out << " |";
    out << " IT_UNDEAD";
    fg_before = true;
  }
  if( _item->type & IT_DEMON ) {
    if( fg_before ) out << " |";
    out << " IT_DEMON";
    fg_before = true;
  }
  if( _item->type & IT_LEADER ) {
    if( fg_before ) out << " |";
    out << " IT_LEADER";
    fg_before = true;
  }
  if( _item->type & IT_MONEY ) {
    if( fg_before ) out << " |";
    out << " IT_MONEY";
    fg_before = true;
  }
  if( _item->type & IT_ANIMAL ) {
    if( fg_before ) out << " |";
    out << " IT_ANIMAL";
    fg_before = true;
  }
  if( _item->type & IT_SHIP ) {
    if( fg_before ) out << " |";
    out << " IT_SHIP";
    fg_before = true;
  }
  if( _item->type & IT_MAGEONLY ) {
    if( fg_before ) out << " |";
    out << " IT_MAGEONLY";
    fg_before = true;
  }
  if( _item->type & IT_ALWAYS_SPOIL ) {
    if( fg_before ) out << " |";
    out << " IT_ALWAYS_SPOIL";
    fg_before = true;
  }
  if( _item->type & IT_NEVER_SPOIL ) {
    if( fg_before ) out << " |";
    out << " IT_NEVER_SPOIL";
    fg_before = true;
  }
  out << ", ";
  fg_before = false;
  // baseprice, combat
  out << _item->baseprice << "," << _item->combat << "," << std::endl;
  // move
  out << "\t" << _item->walk << "," << _item->ride << "," << _item->fly << "," << _item->swim << "," << _item->speed << "," << std::endl;
  // hitchItem hitchwalk
  if( _item->hitchItem >= 0 ) {
	out << "\t" << find_itype_byid( _item->hitchItem );
  }
  else out << "\t" << _item->hitchItem;
  out << "," << _item->hitchwalk << "," << std::endl;
  // LLS
  if( _item->mult_item >= 0 ) {
	out << "\t" << find_itype_byid( _item->mult_item );
  }
  else out << "\t" << _item->mult_item;
  out << "," << _item->mult_val << "," << _item->max_inventory << "," << std::endl;
  // ESCAPE
  out << "\t";
  if( _item->escape == 0 ) {
    out << _item->escape;
  }
  else {
    if( _item->escape & ItemType::LOSE_LINKED ) {
      if( fg_before ) out << " |";
      out << " ItemType::LOSE_LINKED";
      fg_before = true;
    }
    if( _item->escape & ItemType::HAS_SKILL ) {
      if( fg_before ) out << " |";
      out << " ItemType::HAS_SKILL";
      fg_before = true;
    }
    if( _item->escape & ItemType::ESC_LEV_LINEAR ) {
      if( fg_before ) out << " |";
      out << " ItemType::ESC_LEV_LINEAR";
      fg_before = true;
    }
    if( _item->escape & ItemType::ESC_LEV_SQUARE ) {
      if( fg_before ) out << " |";
      out << " ItemType::ESC_LEV_SQUARE";
      fg_before = true;
    }
    if( _item->escape & ItemType::ESC_LEV_CUBE ) {
      if( fg_before ) out << " |";
      out << " ItemType::ESC_LEV_CUBE";
      fg_before = true;
    }
    if( _item->escape & ItemType::ESC_LEV_QUAD ) {
      if( fg_before ) out << " |";
      out << " ItemType::ESC_LEV_QUAD";
      fg_before = true;
    }
    if( _item->escape & ItemType::LOSS_CHANCE ) {
      if( fg_before ) out << " |";
      out << " ItemType::LOSS_CHANCE";
      fg_before = true;
    }
    if( _item->escape & ItemType::ESC_NUM_SQUARE ) {
      if( fg_before ) out << " |";
      out << " ItemType::ESC_NUM_SQUARE";
      fg_before = true;
    }
  }
  out << ", ";
  if( _item->esc_skill != NULL ) {
    out << "\"" << _item->esc_skill << "\", ";
  }
  else {
    out << "NULL, ";
  }
  out << _item->esc_val << "," << std::endl;

  // grant Skill
  if( _item->grantSkill != NULL ) {
    out << "\t\"" << _item->grantSkill << "\", {";
  }
  else out << "\t\"\", {";
  for( int f = 0; f < 4; ++f ) {
    if( _item->fromSkills[f] != NULL ) out << " \"" << _item->fromSkills[f] << "\"";
    else out << " \"\"";
    if( f < 3 ) out << ",";
  }
  out << " }, " << _item->minGrant << ", " << _item->maxGrant << "}," << std::endl;
}
// ***************************************************** AItem::write_type_weapon
void AItem::write_type_weapon( std::ostream& out )
{
  // Already written a flag on this line ?
  bool fg_before = false;

  // abbr
  out << "\t{\"" << _wtype->abbr << "\"," << std::endl;
  // flags
  out << "\t";
  fg_before = false;
  if( _wtype->flags & WeaponType::NEEDSKILL ) {
	if( fg_before ) out << "| ";
	out << "WeaponType::NEEDSKILL ";
	fg_before = true;
  }
  if( _wtype->flags & WeaponType::ALWAYSREADY ) {
	if( fg_before ) out << "| ";
	out << "WeaponType::ALWAYSREADY ";
	fg_before = true;
  }
  if( _wtype->flags & WeaponType::NODEFENSE ) {
	if( fg_before ) out << "| ";
	out << "WeaponType::NODEFENSE ";
	fg_before = true;
  }
  if( _wtype->flags & WeaponType::NOFOOT ) {
	if( fg_before ) out << "| ";
	out << "WeaponType::NOFOOT ";
	fg_before = true;
  }
  if( _wtype->flags & WeaponType::NOMOUNT ) {
	if( fg_before ) out << "| ";
	out << "WeaponType::NOMOUNT ";
	fg_before = true;
  }
  if( _wtype->flags & WeaponType::SHORT ) {
	if( fg_before ) out << "| ";
	out << "WeaponType::SHORT ";
	fg_before = true;
  }
  if( _wtype->flags & WeaponType::LONG ) {
	if( fg_before ) out << "| ";
	out << "WeaponType::LONG ";
	fg_before = true;
  }
  if( _wtype->flags & WeaponType::RANGED ) {
	if( fg_before ) out << "| ";
	out << "WeaponType::RANGED ";
	fg_before = true;
  }
  if( _wtype->flags & WeaponType::NOATTACKERSKILL ) {
	if( fg_before ) out << "| ";
	out << "WeaponType::NOATTACKERSKILL ";
	fg_before = true;
  }
  if( _wtype->flags & WeaponType::RIDINGBONUS ) {
	if( fg_before ) out << "| ";
	out << "WeaponType::RIDINGBONUS ";
	fg_before = true;
  }
  if( _wtype->flags & WeaponType::RIDINGBONUSDEFENSE ) {
	if( fg_before ) out << "| ";
	out << "WeaponType::RIDINGBONUSDEFENSE ";
	fg_before = true;
  }
  out << "," << std::endl;

  // baseSkill and orSkill
  if( _wtype->baseSkill ) {
	out << "\t\"" << _wtype->baseSkill << "\", ";
  }
  else out << "\tNULL, ";
  if( _wtype->orSkill ) {
	out << "\"" << _wtype->orSkill << "\"," << std::endl;
  }
  else out << "NULL," << std::endl;

  // weapClass
  out << "\t" << ItemData::attack_enum[_wtype->weapClass] << ", " << ItemData::attack_type[_wtype->attackType] << ", ";
  // Num ATTACK
  if( _wtype->numAttacks >= WeaponType::NUM_ATTACKS_SKILL ) {
	out << "WeaponType::NUM_ATTACKS_SKILL";
	if( _wtype->numAttacks > WeaponType::NUM_ATTACKS_SKILL ) {
	  out << "+" << (_wtype->numAttacks-WeaponType::NUM_ATTACKS_SKILL);
	}
  }
  else if ( _wtype->numAttacks >= WeaponType::NUM_ATTACKS_HALF_SKILL ) {
	out << "WeaponType::NUM_ATTACKS_HALF_SKILL";
	if( _wtype->numAttacks > WeaponType::NUM_ATTACKS_HALF_SKILL ) {
	  out << "+" << (_wtype->numAttacks-WeaponType::NUM_ATTACKS_HALF_SKILL);
	}
  }
  else {
	out << _wtype->numAttacks;
  }
  out << "," << std::endl;

  // bonuses
  out << "\t" << _wtype->attackBonus << ", " << _wtype->defenseBonus << ", " << _wtype->mountBonus << "}," << std::endl;
}

// // ****************************************************** AMonster::write_type
// void AMonster::write_type( std::ostream& out )
// {
//   // Already written a flag on this line ?
//   bool fg_before = false;

//   // attack level
//   out << "\t{" << _mtype->attackLevel << ",";
//   // defenses
//   out << "{";
//   int idx_def = 0;
//   out << _mtype->defense[idx_def++] << ",";
//   out << _mtype->defense[idx_def++] << ",";
//   out << _mtype->defense[idx_def++] << ",";
//   out << _mtype->defense[idx_def++] << ",";
//   out << _mtype->defense[idx_def++] << ",";
//   out << _mtype->defense[idx_def++] << "}";
//   out << "," << std::endl;
//   // numAttacks, hits, regen
//   out << "\t" << _mtype->numAttacks << "," << _mtype->hits << "," << _mtype->regen << ',' << std::endl;
//   // tactics, steal, obs
//   out << "\t" << _mtype->tactics << "," << _mtype->stealth << "," << _mtype->obs << "," << std::endl;
//   // special, specialLevel
//   if( _mtype->special == NULL ) {
//     out << "\tNULL,0," << std::endl;
//   }
//   else {
//     out << "\t\"" << _mtype->special << "\", " << _mtype->specialLevel << "," << std::endl;
//   }
//   // silver
//   out << "\t" << _mtype->silver << ",";
//   // spoilType
//   //out << "TODO,";
//   if( _mtype->spoiltype < 0 ) {
//      out << "-1";
//   }
//   else {
//     fg_before = false;
//     for( auto& itype : _map_itype ) {
//       if( _mtype->spoiltype & itype.second ) {
// 	if( fg_before ) out << "| ";
// 	out << " " << itype.first;
// 	fg_before = true;
//       }
//     }
//   }
//   out << ",";
//   // hostile, number
//   out << _mtype->hostile << "," << _mtype->number << ",";
//   // name, abbr
//   out << "\"" << _mtype->name << "\", \"" << _mtype->abbr << "\"}," << std::endl;
// }
// ******************************************************** AItem::write_debug
void AItem::write_debug( std::ostream& out )
{
  out << "** " << _item_enum;
  out << "\t(" << _item_id << " + " << _wtype_id << ")" << std::endl;
  out << "   edited=" << (_fg_edited ? "true" : "false") << std::endl;
  write_item( out );
  if( _wtype ) {
	out << "  --WEAPON--" << std::endl;
	write_type_weapon( out );
  }
}

// ***************************************************************************
// ***************************************************************************
std::string ItemData::attack_enum[] = {"SLASHING","PIERCING","CRUSHING","CLEAVING","ARMORPIERCING","MAGIC_ENERGY","MAGIC_SPIRIT","MAGIC_WEATHER","NUM_WEAPON_CLASSES"};
std::string ItemData::attack_type[] = {"ATTACK_COMBAT", "ATTACK_ENERGY", "ATTACK_SPIRIT", "ATTACK_WEATHER", "ATTACK_RIDING", "ATTACK_RANGED", "NUM_ATTACK_TYPES"}; 
// ************************************************ ItemData::parse_gamedata()
void ItemData::parse_gamedata( const std::string& path )
{
  _input_path = path;
  std::ifstream h_in( path+"/gamedata.h" );
  if( not h_in.is_open() ) {
	std::cerr << "ERROR: file " << path+"/gamedata.h" << " not open" << std::endl;
	exit(2);
  }
  
  std::string line;
  int idx_item = 0;
  while( std::getline( h_in, line )) {
    if( line[0] == '\t' && line[1] == 'I' && line[2] == '_' ) {
	  if( ((ItemDefs[idx_item].type & IT_MONSTER) == false ) &&
		  ((ItemDefs[idx_item].type & IT_MAN) == false ) &&
		  ((ItemDefs[idx_item].type & IT_MONEY) == false ) )
		{
		  //std::cout << "  + found monster " << line.substr( 1, line.size()-2 ) << ":" << ItemDefs[idx_monster].abr << std::endl;
		  add( line.substr( 1, line.size()-2 ), idx_item );
      }
      idx_item ++;
    }
  }
  _max_item_id = NITEMS;
  _max_wtype_id = NUMWEAPONS;
}
// // ********************************************* MonsterData::write_gamedata()
// void MonsterData::write_gamedata( bool fg_debug )
// {
//     std::ifstream h_in( _input_path+"/gamedata.h" );
//     std::ifstream cpp_in( _input_path+"/gamedata.cpp" );

//     std::ofstream h_out( "gamedata.h.new" );
//     std::ofstream cpp_out( "gamedata.cpp.new" );

//     bool must_copy = true;
//     int idx_monster = 0;
//     int idx_item = 0;
//     std::string line;

//     // Parse cpp_in to first struct
//     if( fg_debug) cpp_out << "// DEBUG parse to first struct" << std::endl;
//     std::string item_line = parse_to_next_item( cpp_in, cpp_out,
// 						  "{", must_copy );
//     cpp_out << item_line << std::endl;

//   // Deal with Items
//   while( std::getline( h_in, line )) {
//     // copy to new
//     h_out << line << std::endl;
//     if( line[0] == '\t' && line[1] == 'I' && line[2] == '_' ) {
//       // found an Item in .h
//       std::cout << "ITEM " << idx_item << "/" << NITEMS << std::endl;
//       //cpp_out << "// DEBUG parse to next item" << std::endl;
// //      std::string item_line = parse_to_next_item( cpp_in, cpp_out,
// //						  "{", must_copy );
//       // Is it a monster ?
//       if( ItemDefs[idx_item].type & IT_MONSTER ) {
//         if( _map_item[idx_item]._fg_edited ) {
//             // // Write new version if edited
//             std::cout << "write_gamedata: write new version of monster " << ItemDefs[idx_item].abr << std::endl;
//             if( fg_debug) cpp_out << "// DEBUG new version of monster " << std::endl;
//             _map_item[idx_item].write_item( cpp_out );
//             copy_item( cpp_in, cpp_out, "", false );
//             must_copy = false;
//         }
//         else {
//             // Else copy this monster
//             std::cout << "write_gamedata: copy current version of " << ItemDefs[idx_item].abr << std::endl;
//             std::cout << "      with : " << item_line << std::endl;
//             if( fg_debug) cpp_out << "// DEBUG copy monster" << std::endl;
//             // parse_to_next will do the job
//             copy_item( cpp_in, cpp_out, item_line, true );
//             must_copy = true;
//         }
//         idx_monster++;
//       }
//     else {
//         // copy
//         //cpp_out << item_line << std::endl;
//         if( fg_debug) cpp_out << "// DEBUG pas un monstre" << std::endl;
//         // parse to next will do the job
//         copy_item( cpp_in, cpp_out, item_line, true );
//         must_copy = true;
//     }
//     // if( item_line.compare( "" ) != 0 ) {
//     // 	// Check if edited :o)
//     // }

//     idx_item++;
//     // if last item...
//     if( idx_item >= NITEMS ) {
// 	std::cout << "write_gamedata: add new monsters with id_item >= " << NITEMS << std::endl;
// 	for( auto& monster : _map_item ) {
// 	  if( monster.second._item_id >= NITEMS ) {
// 	    std::cout << "Write new monster " << monster.second._item_id << std::endl;
// 	    h_out << "\t" << monster.second._item_enum << "," << std::endl;
// 	    if( fg_debug) cpp_out << "// DEBUG new monster" << std::endl;
// 	    monster.second.write_item( cpp_out );
// 	  }
// 	}
// 	// get out of this while-loop
// 	break;
//       }
//     }
//   }
//   // Copy to end of h_in
//   std::string last_line = parse_to_next_item( h_in, h_out, "#endif", true);
//   h_out << last_line << std::endl;

//   // Copy to MonType in cpp
//   if( fg_debug) cpp_out << "// DEBUG copy to MonType" << std::endl;
//   std::string monster_line = parse_to_next_item( cpp_in, cpp_out,
// 					       "MonType md[]", true);
//   cpp_out << monster_line << std::endl;
//   // copy Monster None
//   monster_line = parse_to_next_item( cpp_in, cpp_out,
// 				     "{", true);
//   cpp_out << monster_line << std::endl;
//   int idx_ittype = 1;
//   must_copy = true;
//   while( idx_ittype < NUMMONSTERS ) {
//     // read to beginning
//     monster_line = parse_to_next_item( cpp_in, cpp_out,
// 				       "{", must_copy );
//     AMonster* a_monster = find_monster( idx_ittype );
//     if( a_monster ) {
//       if( a_monster->_fg_edited ) {
// 	std::cout << "write_gamedata: check if MonType is modified" << std::endl;
// 	if( fg_debug) cpp_out << "// DEBUG MonType is modified" << std::endl;
// 	a_monster->write_type( cpp_out );
// 	must_copy = false;
//       }
//       else {
// 	// copy
// 	if( fg_debug) cpp_out << "// DEBUG MonType is copied" << std::endl;
// 	cpp_out << monster_line << std::endl;
// 	must_copy = true;
//       }
//     }
//     else {
//       std::cerr << "write_gamedata : Error MonType id=" << idx_ittype << " not found" << std::endl;
//       exit(1);
//     }
//     idx_ittype++;
//   }
//   // last monster
//   if( fg_debug) cpp_out << "// DEBUG last MonType" << std::endl;
//   monster_line = parse_to_next_item( cpp_in, cpp_out,
// 				     "};", must_copy );
//   // New monsters
//   std::cout << "write_gamedata: copy new MonType " << std::endl;
//   for( auto& monster : _map_item ) {
//     if( monster.second._mtype_id >= NUMMONSTERS ) {
//       std::cout << "Write new monster " << monster.second._mtype_id << std::endl;
//       if( fg_debug) cpp_out << "// DEBUG new MonType" << std::endl;
//       monster.second.write_type( cpp_out );
//     }
//   }
//   cpp_out << monster_line << std::endl;

//   // Copy to end of cpp_in
//   std::cout << "write_gamedata: copier fin de cpp_in dans cpp_out" << std::endl;
//   if( fg_debug) cpp_out << "// DEBUG to end of cpp_in" << std::endl;
//   last_line = parse_to_next_item( cpp_in, cpp_out, "int NUMATTRIBMODS", true);
//   cpp_out << last_line << std::endl;

//   h_in.close();
//   cpp_in.close();
//   h_out.close();
//   cpp_out.close();
// }
// /**
//  * Parse 'in' looking for the next item.
//  * @param: fg_copy, are each line of 'in' copied to 'out'
//  * @param: startstr first non ' ' or non '\t' is this string
//  */
// std::string MonsterData::parse_to_next_item( std::istream& in, std::ostream& out,
// 					     const std::string& startstr,
// 					     bool fg_copy )
// {
//   std::string line;
//   while (std::getline(in, line)) {
//     // std::cout << "READ: " << line << std::endl;
//     // detect item = First non-blanc must be '{'
//     auto it = line.begin();
//     while( *it == ' ' or *it == '\t') {
//       // std::cout << "/" << *it;
//       it++;
//     }
//     //std::cout << std::endl;
//     // Compare strippped line to startstr
//     std::string stripped_str(it, it+startstr.length());
//     if( stripped_str.compare(startstr) == 0 ) {
//       std::cout << "__START " << std::string( it, line.end() ) << std::endl;
// 	return line;
//     }
//     // copy line juste read to 'out'
//     if( fg_copy ) {
//       out << line << std::endl;
//     }
//   }
//   return "";
// }
// /**
//  * Copy every line until a line ends with "(not})}" or "(not)"t hat does not begins with '{'
//  */
// void MonsterData::copy_item( std::istream& in, std::ostream& out,
// 				const std::string& first_line, bool fg_copy )
// {
//   //out << first_line << std::endl;
//   std::string line;
//   while (std::getline(in, line)) {
//         if( fg_copy ) {
//             out << line << std::endl;
// //            if( line.back() != ',')
// //                out << ',';
// //            out << std::endl;
//         }
//     // Ends with "'(not})}," ??
//     auto found = line.rfind( "}" );
//     if( found != std::string::npos ) {
//         // check before is not "}"
//         if( line[found-1] != '}') {
//             return;
//         }
//     }
// //    auto it = line.end();
// //    // std::cout << "READ: " << line << std::endl;
// //    // First non-blanc must be '{'
// //    auto it = line.begin();
// //    while( *it == ' ' or *it == '\t') {
// //      // std::cout << "/" << *it;
// //      it++;
// //    }
// //    //std::cout << std::endl;
// //    if( *it == '{' ) {
// //      std::cout << "__END  " << std::string( it, line.end() ) << std::endl;
// //      return line;
// //    }
// //    out << line << std::endl;
//   }
//   return;
// }
// ********************************************************** MonsterData::add
void ItemData::add( const std::string& str_enum, int id_item )
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
  
  // Add
  _map_item[id_item] = item;
}
// // ***************************************************** MonsterData::make_new
// AMonster* MonsterData::make_new( const std::string& item_str )
// {
//   std::cout << "TODO: make_new, check that no other new with same _item_enum" << std::endl;
//   AMonster monster;
//   monster._fg_edited = true;
//   monster._item_enum = item_str;
//   monster._item = new ItemType
//     {"new monster", "new monsters", "TODO",
//      ItemType::CANTGIVE,
//      NULL,0,0,0, {{-1,0},{-1,0},{-1,0},{-1,0}},
//      NULL,0,0, {{-1,0},{-1,0},{-1,0},{-1,0}},
//      10, IT_MONSTER, 10,1,
//      50,50,0,0,2,
//      -1,0,
//      -1,0,0,
//      0, NULL, 0,
//      "", { "", "", "", "" }, 0, 0};
//   monster._item_id = _max_item_id++;
//   monster._mtype = new MonType
//     {1,{0,0,0,0,0,0},
//      1,1,0,
//      0,0,0,
//      NULL,0,
//      10, IT_NORMAL,10,1,"Un Truc Qui Griffe et Grogne", "TODO"};
//   monster._mtype_id = _max_mtype_id++;

//   _map_item[monster._item_id] = monster;

//   monster.write_debug();

//   return &(_map_item[monster._item_id]);
// }
// **************************************************** ItemData::find_monster
AItem* ItemData::find_item_weapon( int id_wtype )
{
  for( auto& elem : _map_item ) {
    if( elem.second._wtype_id == id_wtype ) {
      return &(elem.second);
    }
  }
  return nullptr;
}
AItem* ItemData::find_item( const std::string& item_enum )
{
    for( auto& elem : _map_item ) {
    if( elem.second._item_enum.compare(item_enum) == 0 ) {
      return &(elem.second);
    }
  }
  return nullptr;
}

