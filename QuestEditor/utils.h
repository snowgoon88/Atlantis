/* -*- coding: utf-8 -*- */

#ifndef UTILS_HPP
#define UTILS_HPP

/**
 * Global variables and useful functions.
 */
#include <string>
#include <map>
#include <vector>

// ***************************************************************************
// ******************************************************************* Globals
/** 
 * _map_itypes is a map of <IT_XXXX::string, IT_XXXX:value>
 * as defined in [path/items.h]
 */
extern std::map<std::string,int> _map_itype;
extern int _max_item_id;
extern int _max_mtype_id;


// ******************************************************** read_itemtype_enum
void read_itemtype_enum( const std::string& path = "..");

// ********************************************************************* split
std::vector<std::string> split(const std::string& str, char c = ' ');

#endif // UTILS_HPP
