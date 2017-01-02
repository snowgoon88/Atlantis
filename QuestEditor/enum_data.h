/* -*- coding: utf-8 -*- */

#ifndef ENUMDATA_HPP
#define ENUMDATA_HPP

/** 
 * Manage a Map <enumString, enumVal> with all the I_XXXXX enums of
 * the Atlantis game of gamedata.h
 */

#include <map>
#include <string>

// ***************************************************************************
// ****************************************************************** EnumData
// ***************************************************************************
class EnumData
{
public:
  // *****************************************************EnumData::parse_enum
  /** Read path/gamedata.h and init map */
  void parse_enumitems( const std::string& path = ".." );

  // ***************************************************** EnumData::attributs
  std::map<std::string,int> _map_items;
}; // class EnumData

#endif // ENUMDATA_HPP
