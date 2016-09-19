/* -*- coding: utf-8 -*- */

/**
 * Read gamedata.hpp to identify Items.
 */
#include <iostream>                   // std::cout, std::istream
#include <sstream>                   // std::istringstream
#include <fstream>                   // std::files

#include <gamedata.h>
#include <items.h>
#include <map>


// ***************************************************************************
// ******************************************************************* GLOBALS
std::map<std::string,int> _map_itype; // IT_XXX enum id items.h

#include <vector>
#include <string>
using namespace std;
// ***************************************************************************
// ********************************************************************* split
std::vector<std::string> split(const std::string& str, char c = ' ')
{
  std::vector<std::string> result;

  // iterator on token.begin
  auto it_start = str.begin();
  do {
    while( (*it_start == c or *it_start == ',' or *it_start == '\t') and it_start != str.end() ) {
      //std::cout << "s:(" << *it_start << ")" << std::endl;
      it_start++;
    }
    
    // iterator on token.end
    auto it_end = it_start;
    while( *it_end != c and *it_end != ',' and *it_end != '\t' and it_end != str.end() ) {
      //std::cout << "e:(" << *it_start << ")" << std::endl;
      it_end++;
    }
    //std::cout << "FOUND:[" << std::string( it_start, it_end ) << "]" << std::endl;
    result.push_back( std::string( it_start, it_end ) );
    it_start = it_end++;
  } while( it_start != str.end() );
  
  return result;
}
// ***************************************************************************
// ******************************************************** read_itemtype_enum
void read_itemtype_enum()
{
  std::ifstream in( "myitems.h" );

  std::string line;
  int idx_monster = 0;
  while( std::getline( in, line )) {
    //std::cout << "[" << line[0] << "] " << line << std::endl;
    if( line[0] == '\t' && line[1] == 'I' && line[2] == 'T' && line[3] == '_' ) {
      auto tokens = split( line );
      std::string tok = tokens[0];
      int id = std::stoi( tokens[2], nullptr, 0 );
      //std::cout << "enum " << tok << ":" << id << " (" << tokens[2] << ")" << std::endl;
      _map_itype[tok] = id;
    }
  }
  in.close();
}
// ***************************************************************************
// ************************************************************* write_monster
void write_monster_item( std::ostream& out, ItemType& idef )
{
  // Already written a flag on this line ?
  bool fg_before = false;

  // name, names, abr
  out << "\t{\"" << idef.name << "\", \"" << idef.names << "\", \"" << idef.abr << "\"," << std::endl;
  // flags
  out << "\t";
  out << "ItemType::NOTRANSPORT";
  fg_before = true;
  if( idef.flags & ItemType::DISABLED ) {
    if( fg_before ) out << " | ";
    out << "ItemType::DISABLED";
  }
  if( idef.flags & ItemType::CANTGIVE ) {
    if( fg_before ) out << " | ";
    out << "ItemType::CANTGIVE";
  }
  out << "," << std::endl;
  fg_before = false;
  // pSkill
  out << "\tNULL,0,0,0, {{-1,0},{-1,0},{-1,0},{-1,0}}," << std::endl;
  // mSkill
  if( idef.mSkill == NULL ) {
    out << "\tNULL,0,0, {{-1,0},{-1,0},{-1,0},{-1,0}}," << std::endl;
  }
  else {
    out << "\t\"" << idef.mSkill << "\"," << idef.mLevel << "," << idef.mOut << ", " << "{{-1,0},{-1,0},{-1,0},{-1,0}}," << std::endl;
  }
  // weight
  out << "\t" << idef.weight << ",";
  // type
  out << " IT_MONSTER";
  fg_before = true;
  if( idef.type & IT_ANIMAL ) {
    if( fg_before ) out << " |";
    out << " IT_ANIMAL";
  }
  if( idef.type & IT_DEMON ) {
    if( fg_before ) out << " |";
    out << " IT_DEMON";
  }
  if( idef.type & IT_ILLUSION ) {
    if( fg_before ) out << " |";
    out << "IT_ILLUSION";
  }
  if( idef.type & IT_UNDEAD ) {
    if( fg_before ) out << " |";
    out << " IT_UNDEAD";
  }
  out << ", ";
  fg_before = false;
  // baseprice, combat
  out << idef.baseprice << "," << idef.combat << "," << std::endl;
  // move
  out << "\t" << idef.walk << "," << idef.ride << "," << idef.fly << "," << idef.swim << "," << idef.speed << "," << std::endl;
  // hitchItem hitchwalk
  out << "\t" << idef.hitchItem << "," << idef.hitchwalk << "," << std::endl;
  // LLS
  out << "\t" << idef.mult_item << "," << idef.mult_val << "," << idef.max_inventory << "," << std::endl;
  // ESCAPE
  out << "\t";
  if( idef.escape == 0 ) {
    out << idef.escape;
  }
  else {
    if( idef.escape & ItemType::LOSE_LINKED ) {
      if( fg_before ) out << " |";
      out << " ItemType::LOSE_LINKED";
      fg_before = true;
    }
    if( idef.escape & ItemType::HAS_SKILL ) {
      if( fg_before ) out << " |";
      out << " ItemType::HAS_SKILL";
      fg_before = true;
    }
    if( idef.escape & ItemType::ESC_LEV_LINEAR ) {
      if( fg_before ) out << " |";
      out << " ItemType::ESC_LEV_LINEAR";
      fg_before = true;
    }
    if( idef.escape & ItemType::ESC_LEV_SQUARE ) {
      if( fg_before ) out << " |";
      out << " ItemType::ESC_LEV_SQUARE";
      fg_before = true;
    }
    if( idef.escape & ItemType::ESC_LEV_CUBE ) {
      if( fg_before ) out << " |";
      out << " ItemType::ESC_LEV_CUBE";
      fg_before = true;
    }
    if( idef.escape & ItemType::ESC_LEV_QUAD ) {
      if( fg_before ) out << " |";
      out << " ItemType::ESC_LEV_QUAD";
      fg_before = true;
    }
    if( idef.escape & ItemType::LOSS_CHANCE ) {
      if( fg_before ) out << " |";
      out << " ItemType::LOSS_CHANCE";
      fg_before = true;
    }
    if( idef.escape & ItemType::ESC_NUM_SQUARE ) {
      if( fg_before ) out << " |";
      out << " ItemType::ESC_NUM_SQUARE";
      fg_before = true;
    }
  }
  out << ", ";
  if( idef.esc_skill != NULL ) {
    out << "\"" << idef.esc_skill << "\", ";
  }
  else {
    out << "NULL, ";
  }
  out << idef.esc_val << "," << std::endl;
  // grant Skill
  out << "\t\"\", { \"\", \"\", \"\", \"\" }, 0, 0}," << std::endl;
}
void write_monster_type( std::ostream& out, MonType& mdef )
{
  // Already written a flag on this line ?
  bool fg_before = false;
  
  // attack level
  out << "\t{" << mdef.attackLevel << ",";
  // defenses
  out << "{";
  int idx_def = 0;
  out << mdef.defense[idx_def++] << ",";
  out << mdef.defense[idx_def++] << ",";
  out << mdef.defense[idx_def++] << ",";
  out << mdef.defense[idx_def++] << ",";
  out << mdef.defense[idx_def++] << ",";
  out << mdef.defense[idx_def++] << "}";
  out << "," << std::endl;
  // numAttacks, hits, regen
  out << "\t" << mdef.numAttacks << "," << mdef.hits << "," << mdef.regen << ',' << std::endl;
  // tactics, steal, obs
  out << "\t" << mdef.tactics << "," << mdef.stealth << "," << mdef.obs << "," << std::endl;
  // special, specialLevel
  if( mdef.special == NULL ) {
    out << "\tNULL,0," << std::endl;
  }
  else {
    out << "\t\"" << mdef.special << "\", " << mdef.specialLevel << "," << std::endl;
  }
  // silver
  out << "\t" << mdef.silver << ",";
  // spoilType
  //out << "TODO,";
  if( mdef.spoiltype < 0 ) {
     out << "-1";
  }
  else {
    fg_before = false;
    for( auto& itype : _map_itype ) {
      if( mdef.spoiltype & itype.second ) {
	if( fg_before ) out << "| ";
	out << " " << itype.first;
	fg_before = true;
      }
    }
  }
  out << ",";
  // hostile, number
  out << mdef.hostile << "," << mdef.number << ",";
  // name, abbr
  out << "\"" << mdef.name << "\", \"" << mdef.abbr << "\"}," << std::endl;
}
// ***************************************************************************
// **************************************************************** parse_item
void parse_items()
{
  std::ifstream cpp_file( "mygamedata.cpp" );

  
  std::string line;
  while (std::getline(cpp_file, line)) {
    // std::cout << "READ: " << line << std::endl;
    // First non-blanc must be '{'
    auto it = line.begin();
    while( *it == ' ' or *it == '\t') {
      // std::cout << "/" << *it;
      it++;
    }
    //std::cout << std::endl;
    if( *it == '{' ) {
      it++;
      if( *it == '"' ) {
	std::cout << "__FOUND " << std::string( it, line.end() ) << std::endl;
      }
    }
  }
}
std::string find_next_item( std::istream& in )
{
  std::string line;
  while (std::getline(in, line)) {
    // std::cout << "READ: " << line << std::endl;
    // First non-blanc must be '{'
    auto it = line.begin();
    while( *it == ' ' or *it == '\t') {
      // std::cout << "/" << *it;
      it++;
    }
    //std::cout << std::endl;
    if( *it == '{' ) {
      it++;
      if( *it == '"' ) {
	std::cout << "__START " << std::string( it, line.end() ) << std::endl;
	return line;
      }
    }
  }
  return "";
}
std::string find_next_enditem( std::istream& in )
{
  std::string line;
  while (std::getline(in, line)) {
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
      return line;
    }
  }
  return "";
}
void parse_gamedata()
{
  std::ifstream h_in( "mygamedata.h" );
  std::ifstream cpp_in( "mygamedata.cpp" );

  std::string line;
  int idx_monster = 0;
  while( std::getline( h_in, line )) {
    //std::cout << "[" << line[0] << "] " << line << std::endl;
    if( line[0] == '\t' && line[1] == 'I' && line[2] == '_' ) {
      std::cout << "__ITEM: " << line.substr( 1 ) << std::endl;
      find_next_item( cpp_in );
      find_next_enditem( cpp_in );
      
      if( ItemDefs[idx_monster].type & IT_MONSTER ) {
	std::cout << "  + found monster " << line.substr( 1, line.size()-2 ) << ":" << ItemDefs[idx_monster].abr << std::endl;
	write_monster_item( std::cout, ItemDefs[idx_monster] );
	//_map_abbr[line.substr( 1, line.size()-2 )] = idx_monster;
	MonType* mtype = FindMonster( ItemDefs[idx_monster].abr,
				      ItemDefs[idx_monster].type & IT_ILLUSION );
	if( mtype != NULL ) {
	  write_monster_type( std::cout, *mtype );
	}
	else {
	  std::cout << "  NO Monster Type" << std::endl;
	}
      }
      idx_monster ++;
    }
  }
}

// ***************************************************************************
// ********************************************************************** main
int main(int argc, char *argv[])
{
  read_itemtype_enum();
  parse_gamedata();

  // auto res = split( "Coucou c'est moi c est" );
  // for(auto& str : res) {
  //   std::cout << "FOUND: " << str << std::endl;
  // }

  // for (auto it=_map_itype.begin(); it!=_map_itype.end(); ++it)
  //   std::cout << it->first << " => " << it->second << '\n';
  
  return 0;
}

