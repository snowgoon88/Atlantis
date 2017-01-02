/* -*- coding: utf-8 -*- */

#include <enum_data.h>
#include <utils.h>

#include <fstream>
#include <iostream>

// ****************************************************** EnumData::parse_enum
void EnumData::parse_enumitems( const std::string& path)
{
  std::ifstream in( path+"/gamedata.h" );
  if( not in.is_open() ) {
	std::cerr << "ERROR: file " << path+"/gamedata.h" << " not open" << std::endl;
	exit(2);
  }

  std::string line;
  int idx_item = 0;
  while( std::getline( in, line )) {
    //std::cout << "[" << line[0] << "] " << line << std::endl;
    if( line[0] == '\t' && line[1] == 'I' && line[2] == '_') {
	  // get the name of the enum
      auto tokens = split( line, ',' );
      std::string tok = tokens[0];
      std::cout << "enum " << tok << ":" << idx_item << std::endl;
      _map_itype[tok] = idx_item;
	  idx_item ++;
    }
  }
  in.close();
}
