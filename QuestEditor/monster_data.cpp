/* -*- coding: utf-8 -*- */

/** 
 * TODO
 */
#include <monster_data.h>
#include <utils.h>

// ******************************************************** AMonster::AMonster
AMonster::AMonster() :
  _fg_edited(false), _item_enum(""), _item_id(-1), _item(nullptr),
  _mtype_id(-1), _mtype(nullptr)
{}
// ****************************************************** AMonster::write_item
void AMonster::write_item( std::ostream& out )
{
  // Already written a flag on this line ?
  bool fg_before = false;

  // name, names, abr
  out << "\t{\"" << _item->name << "\", \"" << _item->names << "\", \"" << _item->abr << "\"," << std::endl;
  // flags
  out << "\t";
  out << "ItemType::NOTRANSPORT";
  fg_before = true;
  if( _item->flags & ItemType::DISABLED ) {
    if( fg_before ) out << " | ";
    out << "ItemType::DISABLED";
  }
  if( _item->flags & ItemType::CANTGIVE ) {
    if( fg_before ) out << " | ";
    out << "ItemType::CANTGIVE";
  }
  out << "," << std::endl;
  fg_before = false;
  // pSkill
  out << "\tNULL,0,0,0, {{-1,0},{-1,0},{-1,0},{-1,0}}," << std::endl;
  // mSkill
  if( _item->mSkill == NULL ) {
    out << "\tNULL,0,0, {{-1,0},{-1,0},{-1,0},{-1,0}}," << std::endl;
  }
  else {
    out << "\t\"" << _item->mSkill << "\"," << _item->mLevel << "," << _item->mOut << ", " << "{{-1,0},{-1,0},{-1,0},{-1,0}}," << std::endl;
  }
  // weight
  out << "\t" << _item->weight << ",";
  // type
  out << " IT_MONSTER";
  fg_before = true;
  if( _item->type & IT_ANIMAL ) {
    if( fg_before ) out << " |";
    out << " IT_ANIMAL";
  }
  if( _item->type & IT_DEMON ) {
    if( fg_before ) out << " |";
    out << " IT_DEMON";
  }
  if( _item->type & IT_ILLUSION ) {
    if( fg_before ) out << " |";
    out << "IT_ILLUSION";
  }
  if( _item->type & IT_UNDEAD ) {
    if( fg_before ) out << " |";
    out << " IT_UNDEAD";
  }
  out << ", ";
  fg_before = false;
  // baseprice, combat
  out << _item->baseprice << "," << _item->combat << "," << std::endl;
  // move
  out << "\t" << _item->walk << "," << _item->ride << "," << _item->fly << "," << _item->swim << "," << _item->speed << "," << std::endl;
  // hitchItem hitchwalk
  out << "\t" << _item->hitchItem << "," << _item->hitchwalk << "," << std::endl;
  // LLS
  out << "\t" << _item->mult_item << "," << _item->mult_val << "," << _item->max_inventory << "," << std::endl;
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
  out << "\t\"\", { \"\", \"\", \"\", \"\" }, 0, 0}," << std::endl;
}
// ****************************************************** AMonster::write_type
void AMonster::write_type( std::ostream& out )
{
  // Already written a flag on this line ?
  bool fg_before = false;
  
  // attack level
  out << "\t{" << _mtype->attackLevel << ",";
  // defenses
  out << "{";
  int idx_def = 0;
  out << _mtype->defense[idx_def++] << ",";
  out << _mtype->defense[idx_def++] << ",";
  out << _mtype->defense[idx_def++] << ",";
  out << _mtype->defense[idx_def++] << ",";
  out << _mtype->defense[idx_def++] << ",";
  out << _mtype->defense[idx_def++] << "}";
  out << "," << std::endl;
  // numAttacks, hits, regen
  out << "\t" << _mtype->numAttacks << "," << _mtype->hits << "," << _mtype->regen << ',' << std::endl;
  // tactics, steal, obs
  out << "\t" << _mtype->tactics << "," << _mtype->stealth << "," << _mtype->obs << "," << std::endl;
  // special, specialLevel
  if( _mtype->special == NULL ) {
    out << "\tNULL,0," << std::endl;
  }
  else {
    out << "\t\"" << _mtype->special << "\", " << _mtype->specialLevel << "," << std::endl;
  }
  // silver
  out << "\t" << _mtype->silver << ",";
  // spoilType
  //out << "TODO,";
  if( _mtype->spoiltype < 0 ) {
     out << "-1";
  }
  else {
    fg_before = false;
    for( auto& itype : _map_itype ) {
      if( _mtype->spoiltype & itype.second ) {
	if( fg_before ) out << "| ";
	out << " " << itype.first;
	fg_before = true;
      }
    }
  }
  out << ",";
  // hostile, number
  out << _mtype->hostile << "," << _mtype->number << ",";
  // name, abbr
  out << "\"" << _mtype->name << "\", \"" << _mtype->abbr << "\"}," << std::endl;
}
// ****************************************************** AMonster::write_debg
void AMonster::write_debug( std::ostream& out )
{
  out << "** " << _item_enum;
  out << "\t(" << _item_id << " + " << _mtype_id << ")" << std::endl;
  out << "   edited=" << (_fg_edited ? "true" : "false") << std::endl;
  write_item( out );
  write_type( out );
}

// ***************************************************************************
// ***************************************************************************
// ********************************************* MonsterData::parse_gamedata()
void MonsterData::parse_gamedata()
{
  std::ifstream h_in( "mygamedata.h" );
  //std::ifstream cpp_in( "mygamedata.cpp" );

  std::string line;
  int idx_monster = 0;
  while( std::getline( h_in, line )) {
    //std::cout << "[" << line[0] << "] " << line << std::endl;
    if( line[0] == '\t' && line[1] == 'I' && line[2] == '_' ) {
      if( ItemDefs[idx_monster].type & IT_MONSTER ) {
	//std::cout << "  + found monster " << line.substr( 1, line.size()-2 ) << ":" << ItemDefs[idx_monster].abr << std::endl;
	add( line.substr( 1, line.size()-2 ), idx_monster );
      }
      idx_monster ++;
    }
  }
  _max_item_id = NITEMS;
  _max_mtype_id = NUMMONSTERS;
}
// ********************************************* MonsterData::write_gamedata()
void MonsterData::write_gamedata()
{
  std::ifstream h_in( "mygamedata.h" );
  std::ifstream cpp_in( "mygamedata.cpp" );

  std::ofstream h_out( "new_gamedata.h" );
  std::ofstream cpp_out( "new_gamedata.cpp" );

  bool must_copy = true;
  int idx_monster = 0;
  int idx_item = 0;
  std::string line;

  // Deal with Items
  while( std::getline( h_in, line )) {
    // copy to new
    h_out << line << std::endl;
    if( line[0] == '\t' && line[1] == 'I' && line[2] == '_' ) {
      // found an Item in .h
      std::cout << "ITEM " << idx_item << "/" << NITEMS << std::endl;
      std::string item_line = parse_to_next_item( cpp_in, cpp_out,
						  "{\"", must_copy );
      // Is it a monster ?
      if( ItemDefs[idx_item].type & IT_MONSTER ) {
	if( _map_item[idx_item]._fg_edited ) {
	  // // Write new version if edited
	  std::cout << "write_gamedata: write new version of monster " << ItemDefs[idx_item].abr << std::endl;
	  _map_item[idx_item].write_item( cpp_out );
	  must_copy = false;
	}
	else {
	  // Else copy this monster
	  std::cout << "write_gamedata: copy current version of " << ItemDefs[idx_item].abr << std::endl;
	  std::cout << "      with : " << item_line << std::endl;
	  copy_item( cpp_in, cpp_out, item_line );
	  must_copy = true;
	}
	idx_monster++;
      }
      else {
	// copy
	//cpp_out << item_line << std::endl;
	copy_item( cpp_in, cpp_out, item_line );
	must_copy = true;
      }
      // if( item_line.compare( "" ) != 0 ) {
      // 	// Check if edited :o)
      // }

      idx_item++;
      // if last item...
      if( idx_item >= NITEMS ) {
	std::cout << "write_gamedata: add new monsters with id_item >= " << NITEMS << std::endl;
	for( auto& monster : _map_item ) {
	  if( monster.second._item_id >= NITEMS ) {
	    std::cout << "Write new monster " << monster.second._item_id << std::endl;
	    h_out << "\t" << monster.second._item_enum << "," << std::endl;
	    monster.second.write_item( cpp_out );
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
	a_monster->write_type( cpp_out );
	must_copy = false;
      }
      else {
	// copy
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
  monster_line = parse_to_next_item( cpp_in, cpp_out,
				     "};", must_copy );
  // New monsters
  std::cout << "write_gamedata: copy new MonType " << std::endl;
  for( auto& monster : _map_item ) {
    if( monster.second._mtype_id >= NUMMONSTERS ) {
      std::cout << "Write new monster " << monster.second._mtype_id << std::endl;
      monster.second.write_type( cpp_out );
    }
  }
  cpp_out << monster_line << std::endl;
  
  // Copy to end of cpp_in
  std::cout << "write_gamedata: copier fin de cpp_in dans cpp_out" << std::endl;
  last_line = parse_to_next_item( cpp_in, cpp_out, "int NUMATTRIBMODS", true);
  cpp_out << last_line << std::endl;

  h_in.close();
  cpp_in.close();
  h_out.close();
  cpp_out.close();
}
/**
 * Parse 'in' looking for the next item.
 * @param: fg_copy, are each line of 'in' copied to 'out'
 * @param: startstr first non ' ' or non '\t' is this string
 */
std::string MonsterData::parse_to_next_item( std::istream& in, std::ostream& out,
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
void MonsterData::copy_item( std::istream& in, std::ostream& out,
				const std::string& first_line )
{
  out << first_line << std::endl;
  std::string line;
  while (std::getline(in, line)) {
    out << line << std::endl;
    // std::cout << "READ: " << line << std::endl;
    // First non-blanc must be '"'
    auto it = line.begin();
    while( *it == ' ' or *it == '\t') {
      // std::cout << "/" << *it;
      it++;
    }
    //std::cout << std::endl;
    if( *it == '"' ) {
      std::cout << "__END  " << std::string( it, line.end() ) << std::endl;
      return;
    }
  }
  return;
}
// ********************************************************** MonsterData::add
void MonsterData::add( const std::string& str_enum, int id_item )
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
// ***************************************************** MonsterData::make_new
void MonsterData::make_new()
{
  std::cout << "TODO: make_new, check that no other new with same _item_enum" << std::endl;
  AMonster monster;
  monster._fg_edited = true;
  monster._item_enum = "I_TODO";
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

  _map_item[monster._item_id] = monster;
  
  monster.write_debug();
}
// ************************************************* MonsterData::find_monster
AMonster* MonsterData::find_monster( int id_mtype )
{
  for( auto& monster : _map_item ) {
    if( monster.second._mtype_id == id_mtype ) {
      return &(monster.second);
    }
  }
  return nullptr;
}
