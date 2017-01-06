/* -*- coding: utf-8 -*- */

/**
 * _map_itypes = map<IT_XXX::string,IT_XXX::value>
 * split a string according to c
 */
#include <utils.h>

#include <fstream>
#include <sstream>

// ******************************************************************* Globals
std::map<std::string,int> _map_itype;
int _max_item_id = -1;
int _max_mtype_id = -2;
int _max_wtype_id = -3;
int _max_atype_id = -4;
// ******************************************************** read_itemtype_enum
void read_itemtype_enum( const std::string& path)
{
  std::ifstream in( path+"items.h" );

  std::string line;
  int idx_monster = 0;
  while( std::getline( in, line )) {
    //std::cout << "[" << line[0] << "] " << line << std::endl;
    if( line[0] == '\t' && line[1] == 'I' && line[2] == 'T' && line[3] == '_' ) {
      auto tokens = split( line );
      std::string tok = tokens[0];
      std::stringstream t2(tokens[2]);
      int id; t2 >> id;
      //int id = std::stoi( tokens[2], nullptr, 0 );
      //std::cout << "enum " << tok << ":" << id << " (" << tokens[2] << ")" << std::endl;
      _map_itype[tok] = id;
    }
  }
  in.close();
}
// ***************************************************************** find_byid
std::string find_itype_byid( int id )
{
  for( auto& elem: _map_itype) {
    if( elem.second == id ) {
	  return elem.first;
	}
  }
  return "UNKNOWN";
}
// ********************************************************************* split
std::vector<std::string> split(const std::string& str, char c)
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
