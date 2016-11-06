/* -*- coding: utf-8 -*- */

/**
 * Read 'file_report' and
 * "underword" -> "2 <underworld>"
 * save old to 'file_report'.bak
 * and new to 'file_report'
 * 
 * g++ -std=c++11 -o tool_report tool_parsereport.cpp
 */
#include <iostream>                   // std::cout, std::istream
#include <sstream>                   // std::istringstream
#include <fstream>                   // std::files
#include <cstdio>

// ***************************************************************************
// ******************************************************************* GLOBALS

// ***************************************************************************
// **************************************************************** parse_item
// void parse_items()
// {
//   std::ifstream cpp_file( "mygamedata.cpp" );

  
//   std::string line;
//   while (std::getline(cpp_file, line)) {
//     // std::cout << "READ: " << line << std::endl;
//     // First non-blanc must be '{'
//     auto it = line.begin();
//     while( *it == ' ' or *it == '\t') {
//       // std::cout << "/" << *it;
//       it++;
//     }
//     //std::cout << std::endl;
//     if( *it == '{' ) {
//       it++;
//       if( *it == '"' ) {
// 	std::cout << "__FOUND " << std::string( it, line.end() ) << std::endl;
//       }
//     }
//   }
// }
// std::string find_next_item( std::istream& in )
// {
//   std::string line;
//   while (std::getline(in, line)) {
//     // std::cout << "READ: " << line << std::endl;
//     // First non-blanc must be '{'
//     auto it = line.begin();
//     while( *it == ' ' or *it == '\t') {
//       // std::cout << "/" << *it;
//       it++;
//     }
//     //std::cout << std::endl;
//     if( *it == '{' ) {
//       it++;
//       if( *it == '"' ) {
// 	std::cout << "__START " << std::string( it, line.end() ) << std::endl;
// 	return line;
//       }
//     }
//   }
//   return "";
// }
// std::string find_next_enditem( std::istream& in )
// {
//   std::string line;
//   while (std::getline(in, line)) {
//     // std::cout << "READ: " << line << std::endl;
//     // First non-blanc must be '"'
//     auto it = line.begin();
//     while( *it == ' ' or *it == '\t') {
//       // std::cout << "/" << *it;
//       it++;
//     }
//     //std::cout << std::endl;
//     if( *it == '"' ) {
//       std::cout << "__END  " << std::string( it, line.end() ) << std::endl;
//       return line;
//     }
//   }
//   return "";
// }
// void parse_gamedata()
// {
//   std::ifstream h_in( "mygamedata.h" );
//   std::ifstream cpp_in( "mygamedata.cpp" );

//   std::string line;
//   int idx_monster = 0;
//   while( std::getline( h_in, line )) {
//     //std::cout << "[" << line[0] << "] " << line << std::endl;
//     if( line[0] == '\t' && line[1] == 'I' && line[2] == '_' ) {
//       std::cout << "__ITEM: " << line.substr( 1 ) << std::endl;
//       find_next_item( cpp_in );
//       find_next_enditem( cpp_in );
      
//       if( ItemDefs[idx_monster].type & IT_MONSTER ) {
// 	std::cout << "  + found monster " << line.substr( 1, line.size()-2 ) << ":" << ItemDefs[idx_monster].abr << std::endl;
// 	//write_monster_item( std::cout, ItemDefs[idx_monster] );

// 	_monster_data.add( line.substr( 1, line.size()-2 ), idx_monster );
// 	// //_map_abbr[line.substr( 1, line.size()-2 )] = idx_monster;
// 	// MonType* mtype = FindMonster( ItemDefs[idx_monster].abr,
// 	// 			      ItemDefs[idx_monster].type & IT_ILLUSION );
// 	// if( mtype != NULL ) {
// 	//   write_monster_type( std::cout, *mtype );
// 	// }
// 	// else {
// 	//   std::cout << "  NO Monster Type" << std::endl;
// 	// }
//       }
//       idx_monster ++;
//     }
//   }
//   _max_item_id = NITEMS;
//   _max_mtype_id = NUMMONSTERS;
// }
// ***************************************************************************
// ************************************************************** parse_report
void parse_report( char *filename )
{
  std::string file_name( filename );
  std::ifstream file_in( file_name );
  std::ofstream file_bak( "new_"+file_name );

  std::string line;
  while( std::getline( file_in, line )) {
	size_t index = 0;
	while (true) {
	  /* Locate the substring to replace. */
	  index = line.find(",underworld)", index);
	  if (index == std::string::npos) break;

	  /* Make the replacement. */
	  line.replace(index, 12, ",2 <underworld>)");

	  /* Advance index forward so the next iteration doesn't pick it up as well. */
	  index += 14;
	}

	file_bak << line << std::endl;
  }

  file_bak.close();
  file_in.close();

  std::rename( file_name.c_str(), std::string(file_name+".bak").c_str());
  std::rename( std::string("new_"+file_name).c_str(),
			   file_name.c_str() );
}
// ***************************************************************************
// ********************************************************************** main
int main(int argc, char *argv[])
{
  if( argc != 2 ) {
	std::cout << "Usage: tool_report <nom_fichier_report>" << std::endl;
	exit(1);
  }

  parse_report( argv[1] );
  
  return 0;
}

