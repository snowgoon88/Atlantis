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
