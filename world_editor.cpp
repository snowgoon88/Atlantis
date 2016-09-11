/* -*- coding: utf-8 -*- */

/** 
 * - Init Game
 * - Apply Ruleset
 * - List existing elements
 */
#include <game.h>
#include <map_access.h>
#include <stdlib.h>     /* exit, EXIT_FAILURE */
#include <string.h>
#include <sstream>
#include <fstream>                 // std::ofstream

Game _game;

std::string head_item = "name\tnames\tabr\tCANTGIVE\tDISABLED\tNOMARKET\tORINPUTS\tSKILLOUT\tNOTRANSPORT\tpSkill\tpLevel\tpMonths\tpOut\tpIn0_item\tpIn0_amt\tpIn1_item\tpIn1_amt\tpIn2_item\tpIn2_amt\tpIn3_item\tpIn3_amt\tmSkill\tmLevel\tmOut\tmIn0_item\tmIn0_amt\tmIn1_item\tmIn1_amt\tmIn2_item\tmIn2_amt\tmIn3_item\tmIn3_amt\tweight\tIT_NORMAL\tIT_ADVANCED\tIT_TRADE\tIT_MAN\tIT_MONSTER\tIT_MAGIC\tIT_WEAPON\tIT_ARMOR\tIT_MOUNT\tIT_BATTLE\tIT_SPECIAL\tIT_TOOL\tIT_FOOD\tIT_ILLUSION\tIT_UNDEAD\tIT_DEMON\tIT_LEADER\tIT_MONEY\tIT_ANIMAL\tIT_SHIP\tIT_MAGEONLY\tIT_ALWAYS_SPOIL\tIT_NEVER_SPOIL\tbaseprice\tcombat\twalk\tride\tfly\tswim\tspeed\thitchItem\thitchwalk\tmult_item\tmult_val\tmax_inventory\tLOSE_LINKED\tHAS_SKILL\tESC_LEV_LINEAR\tESC_LEV_SQUARE\tESC_LEV_CUBE\tESC_LEV_QUAD\tLOSS_CHANCE\tESC_NUM_SQUARE\tesc_skill\tesc_val\tgrantSkill\tfromSkill0\tfromSkill1\tfromSkill2\tfromSkill3\tminGrant\tmaxGrant";

std::string head_mon = "attackLevel\tATT_COMBAT\tATT_ENERGY\tATT_SPIRIT\tATT_WEATHER\tATT_RIDING\tATT_RANGED\tnumAttacks\thits\tregen\ttactics\tstealth\tobs\tspecial\tspecialLevel\tsilver\tSPOIL_IT_NORMAL\tSPOIL_IT_ADVANCED\tSPOIL_IT_TRADE\tSPOIL_IT_MAN\tSPOIL_IT_MONSTER\tSPOIL_IT_MAGIC\tSPOIL_IT_WEAPON\tSPOIL_IT_ARMOR\tSPOIL_IT_MOUNT\tSPOIL_IT_BATTLE\tSPOIL_IT_SPECIAL\tSPOIL_IT_TOOL\tSPOIL_IT_FOOD\tSPOIL_IT_ILLUSION\tSPOIL_IT_UNDEAD\tSPOIL_IT_DEMON\tSPOIL_IT_LEADER\tSPOIL_IT_MONEY\tSPOIL_IT_ANIMAL\tSPOIL_IT_SHIP\tSPOIL_IT_MAGEONLY\tSPOIL_IT_ALWAYS_SPOIL\tSPOIL_IT_NEVER_SPOIL\thostile\tnumber\tmon_name\tmon_abbr";

void print_item(int id)
{
  if( id >= 0 ) {
    std::cout << ItemDefs[id].name;
  }
  else {
    std::cout << "-1";
  }
}

/** list all monsters as Items, */
void list_monsters()
{
  std::ofstream ofile( "monsters.data" );
  
  std::cout << head_item << "\t" << head_mon << std::endl;
  ofile << head_item << "\t" << head_mon << std::endl;
  
  for( int i=0; i<NUMMONSTERS; ++i ) {
    std::cout << MonDefs[i].name;
    if( MonDefs[i].abbr != NULL ) {
      std::cout  << " [" << MonDefs[i].abbr << "]" << std::endl;
      // find it in Items
      for(int j=0; j < NITEMS; ++j) {
	if( strcmp( MonDefs[i].abbr, ItemDefs[j].abr) == 0 ) {
	    std::cout << "  ->Item " << j << " : ";
	    print_item(j);
	    std::cout << std::endl;
	    
	    // Prepare string
	    stringstream line;
	    line << ItemDefs[j].name << "\t";
	    line << ItemDefs[j].names << "\t";
	    line << ItemDefs[j].abr << "\t";
	    if( ItemDefs[j].flags & ItemType::CANTGIVE )
	      line << "Y\t";
	    else  line << "N\t";
	    if( ItemDefs[j].flags & ItemType::DISABLED )
	      line << "Y\t";
	    else  line << "N\t";
	    if( ItemDefs[j].flags & ItemType::NOMARKET )
	      line << "Y\t";
	    else  line << "N\t";
	    if( ItemDefs[j].flags & ItemType::ORINPUTS )
	      line << "Y\t";
	    else  line << "N\t";
	    if( ItemDefs[j].flags & ItemType::SKILLOUT )
	      line << "Y\t";
	    else  line << "N\t";
	    if( ItemDefs[j].flags & ItemType::NOTRANSPORT )
	      line << "Y\t";
	    else  line << "N\t";
	    if( ItemDefs[j].pSkill ) {
	      line << ItemDefs[j].pSkill << "\t";
	    }
	    else line << "NULL\t";
	    line << ItemDefs[j].pLevel << "\t";
	    line << ItemDefs[j].pMonths << "\t";
	    line << ItemDefs[j].pOut << "\t";
	    for( int k=0; k<4; ++k ) {
	      line << ItemDefs[j].pInput[k].item << "\t" << ItemDefs[j].pInput[k].amt << "\t";
	    }
	    if( ItemDefs[j].mSkill ) {
	      line << ItemDefs[j].mSkill << "\t";
	    }
	    else line << "NULL\t";
	    line << ItemDefs[j].mLevel << "\t";
	    line << ItemDefs[j].mOut << "\t";
	    for( int k=0; k<4; ++k ) {
	      line << ItemDefs[j].mInput[k].item << "\t" << ItemDefs[j].mInput[k].amt << "\t";
	    }
	    line << ItemDefs[j].weight << "\t";
	    if( ItemDefs[j].type & IT_NORMAL )
	      line << "Y\t";
	    else  line << "N\t";
	    if( ItemDefs[j].type & IT_ADVANCED )
	      line << "Y\t";
	    else  line << "N\t";
	    if( ItemDefs[j].type & IT_TRADE )
	      line << "Y\t";
	    else  line << "N\t";
	    if( ItemDefs[j].type & IT_MAN )
	      line << "Y\t";
	    else  line << "N\t";
	    if( ItemDefs[j].type & IT_MONSTER )
	      line << "Y\t";
	    else  line << "N\t";
	    if( ItemDefs[j].type & IT_MAGIC )
	      line << "Y\t";
	    else  line << "N\t";
	    if( ItemDefs[j].type & IT_WEAPON )
	      line << "Y\t";
	    else  line << "N\t";
	    if( ItemDefs[j].type & IT_ARMOR )
	      line << "Y\t";
	    else  line << "N\t";
	    if( ItemDefs[j].type & IT_MOUNT )
	      line << "Y\t";
	    else  line << "N\t";
	    if( ItemDefs[j].type & IT_BATTLE )
	      line << "Y\t";
	    else  line << "N\t";	    
	    if( ItemDefs[j].type & IT_SPECIAL )
	      line << "Y\t";
	    else  line << "N\t";	    
	    if( ItemDefs[j].type & IT_TOOL )
	      line << "Y\t";
	    else  line << "N\t";	    
	    if( ItemDefs[j].type & IT_FOOD )
	      line << "Y\t";
	    else  line << "N\t";	    
	    if( ItemDefs[j].type & IT_ILLUSION )
	      line << "Y\t";
	    else  line << "N\t";	    
	    if( ItemDefs[j].type & IT_UNDEAD )
	      line << "Y\t";
	    else  line << "N\t";	    
	    if( ItemDefs[j].type & IT_DEMON )
	      line << "Y\t";
	    else  line << "N\t";	    
	    if( ItemDefs[j].type & IT_LEADER )
	      line << "Y\t";
	    else  line << "N\t";	    
	    if( ItemDefs[j].type & IT_MONEY )
	      line << "Y\t";
	    else  line << "N\t";	    
	    if( ItemDefs[j].type & IT_ANIMAL )
	      line << "Y\t";
	    else  line << "N\t";	    
	    if( ItemDefs[j].type & IT_SHIP )
	      line << "Y\t";
	    else  line << "N\t";	    
	    if( ItemDefs[j].type & IT_MAGEONLY )
	      line << "Y\t";
	    else  line << "N\t";
	    if( ItemDefs[j].type & IT_ALWAYS_SPOIL )
	      line << "Y\t";
	    else  line << "N\t";	    
	    if( ItemDefs[j].type & IT_NEVER_SPOIL )
	      line << "Y\t";
	    else  line << "N\t";	    
	    line << ItemDefs[j].baseprice << "\t";
	    line << ItemDefs[j].combat << "\t";
	    line << ItemDefs[j].walk << "\t";
	    line << ItemDefs[j].ride << "\t";
	    line << ItemDefs[j].fly << "\t";
	    line << ItemDefs[j].swim << "\t";
	    line << ItemDefs[j].speed << "\t";
	    line << ItemDefs[j].hitchItem << "\t";
	    line << ItemDefs[j].hitchwalk << "\t";
	    line << ItemDefs[j].mult_item << "\t";
	    line << ItemDefs[j].mult_val << "\t";
	    line << ItemDefs[j].max_inventory << "\t";
	    if( ItemDefs[j].escape & ItemType::LOSE_LINKED )
	      line << "Y\t";
	    else  line << "N\t";	    
	    if( ItemDefs[j].escape & ItemType::HAS_SKILL )
	      line << "Y\t";
	    else  line << "N\t";	    
	    if( ItemDefs[j].escape & ItemType::ESC_LEV_LINEAR )
	      line << "Y\t";
	    else  line << "N\t";	    
	    if( ItemDefs[j].escape & ItemType::ESC_LEV_SQUARE )
	      line << "Y\t";
	    else  line << "N\t";
	    if( ItemDefs[j].escape & ItemType::ESC_LEV_CUBE )
	      line << "Y\t";
	    else  line << "N\t";	    
	    if( ItemDefs[j].escape & ItemType::ESC_LEV_QUAD )
	      line << "Y\t";
	    else  line << "N\t";	    
	    if( ItemDefs[j].escape & ItemType::LOSS_CHANCE )
	      line << "Y\t";
	    else  line << "N\t";
	    if( ItemDefs[j].escape & ItemType::ESC_NUM_SQUARE )
	      line << "Y\t";
	    else  line << "N\t";
	    if( ItemDefs[j].esc_skill ) {
	      line << ItemDefs[j].esc_skill << "\t";
	    }
	    else line << "NULL\t";
	    line << ItemDefs[j].esc_val << "\t";
	    line << ItemDefs[j].grantSkill << "\t";
	    for( int k=0; k<4; ++k ) {
	      line << ItemDefs[j].fromSkills[k] << "\t";
	    }
	    line << ItemDefs[j].minGrant << "\t";
	    line << ItemDefs[j].maxGrant << "\t";
	    
	    // MonType
	    line << MonDefs[i].attackLevel << "\t";
	    for( int k=0; k<NUM_ATTACK_TYPES; ++k ) {
	      line << MonDefs[i].defense[k] << "\t";
	    }
	    line << MonDefs[i].numAttacks << "\t";
	    line << MonDefs[i].hits << "\t";
	    line << MonDefs[i].regen << "\t";
	    line << MonDefs[i].tactics << "\t";
	    line << MonDefs[i].stealth << "\t";
	    line << MonDefs[i].obs << "\t";
	    if( MonDefs[i].special ) {
	      line << MonDefs[i].special << "\t";
	    }
	    else line << "NULL\t";
	    line << MonDefs[i].specialLevel << "\t";
	    line << MonDefs[i].silver << "\t";
	    
	    if( MonDefs[i].spoiltype & IT_NORMAL )
	      line << "Y\t";
	    else  line << "N\t";
	    if( MonDefs[i].spoiltype & IT_ADVANCED )
	      line << "Y\t";
	    else  line << "N\t";
	    if( MonDefs[i].spoiltype & IT_TRADE )
	      line << "Y\t";
	    else  line << "N\t";
	    if( MonDefs[i].spoiltype & IT_MAN )
	      line << "Y\t";
	    else  line << "N\t";
	    if( MonDefs[i].spoiltype & IT_MONSTER )
	      line << "Y\t";
	    else  line << "N\t";
	    if( MonDefs[i].spoiltype & IT_MAGIC )
	      line << "Y\t";
	    else  line << "N\t";
	    if( MonDefs[i].spoiltype & IT_WEAPON )
	      line << "Y\t";
	    else  line << "N\t";
	    if( MonDefs[i].spoiltype & IT_ARMOR )
	      line << "Y\t";
	    else  line << "N\t";
	    if( MonDefs[i].spoiltype & IT_MOUNT )
	      line << "Y\t";
	    else  line << "N\t";
	    if( MonDefs[i].spoiltype & IT_BATTLE )
	      line << "Y\t";
	    else  line << "N\t";	    
	    if( MonDefs[i].spoiltype & IT_SPECIAL )
	      line << "Y\t";
	    else  line << "N\t";	    
	    if( MonDefs[i].spoiltype & IT_TOOL )
	      line << "Y\t";
	    else  line << "N\t";	    
	    if( MonDefs[i].spoiltype & IT_FOOD )
	      line << "Y\t";
	    else  line << "N\t";	    
	    if( MonDefs[i].spoiltype & IT_ILLUSION )
	      line << "Y\t";
	    else  line << "N\t";	    
	    if( MonDefs[i].spoiltype & IT_UNDEAD )
	      line << "Y\t";
	    else  line << "N\t";	    
	    if( MonDefs[i].spoiltype & IT_DEMON )
	      line << "Y\t";
	    else  line << "N\t";	    
	    if( MonDefs[i].spoiltype & IT_LEADER )
	      line << "Y\t";
	    else  line << "N\t";	    
	    if( MonDefs[i].spoiltype & IT_MONEY )
	      line << "Y\t";
	    else  line << "N\t";	    
	    if( MonDefs[i].spoiltype & IT_ANIMAL )
	      line << "Y\t";
	    else  line << "N\t";	    
	    if( MonDefs[i].spoiltype & IT_SHIP )
	      line << "Y\t";
	    else  line << "N\t";	    
	    if( MonDefs[i].spoiltype & IT_MAGEONLY )
	      line << "Y\t";
	    else  line << "N\t";
	    if( MonDefs[i].spoiltype & IT_ALWAYS_SPOIL )
	      line << "Y\t";
	    else  line << "N\t";	    
	    if( MonDefs[i].spoiltype & IT_NEVER_SPOIL )
	      line << "Y\t";
	    else  line << "N\t";
	    line << MonDefs[i].hostile << "\t";
	    line << MonDefs[i].number << "\t";
	    line << MonDefs[i].name << "\t";
	    line << MonDefs[i].abbr;
	    std::cout << line.str() << std::endl;
	    ofile << line.str() << std::endl;
	}
      }
    }
    else {
      std::cout << " [NULL]" << std::endl;
    }
  }

  ofile.close();
}


void list_items()
{
  // Every enables items
  std::cout << "__ITEMS_________________" << std::endl;
  for( int i=0; i<NITEMS; i++) {
    if( !(ItemDefs[i].flags & ItemType::DISABLED)) {
      std::cout << i << "- " << ItemDefs[i].name << ":" << ItemDefs[i].abr << std::endl;
    }
    else {
      std::cout << "****" << i << "- " << ItemDefs[i].name << ":" << ItemDefs[i].abr << std::endl;
    }
  }
}
void list_skills()
{
  // Every enables items
  std::cout << "__SKILLS________________" << std::endl;
  for( int i=0; i<NSKILLS; i++) {
    if( !(SkillDefs[i].flags & ItemType::DISABLED)) {
      std::cout << i << "- " << SkillDefs[i].name << ":" << SkillDefs[i].abbr << std::endl;
    }
    else {
      std::cout << "****" << i << "- " << SkillDefs[i].name << ":" << SkillDefs[i].abbr << std::endl;
    }
  }
}
void list_objects()
{
  // Every enables items
  std::cout << "__OBJECTS_______________" << std::endl;
  for( int i=0; i<NOBJECTS; i++) {
    if( !(ObjectDefs[i].flags & ItemType::DISABLED)) {
      std::cout << i << "- " << ObjectDefs[i].name << std::endl;
    }
    else {
      std::cout << "****" << i << "- " << ObjectDefs[i].name << std::endl;
    }
  }
}
void list_terrain()
{
  // Every enables items
  std::cout << "__TERRAIN_______________" << std::endl;
  for( int i=0; i<R_NUM; i++) {
    if( !(TerrainDefs[i].flags & ItemType::DISABLED)) {
      std::cout << i << "- " << TerrainDefs[i].name << ":" << TerrainDefs[i].type << std::endl;
    }
    else {
      std::cout << "****" << i << TerrainDefs[i].name << ":" << TerrainDefs[i].type << std::endl;
    }
    std::cout << "  small=";print_item( TerrainDefs[i].smallmon );std::cout << std::endl;
    std::cout << "  big  =";print_item( TerrainDefs[i].bigmon );std::cout << std::endl;
    std::cout << "  huma =";print_item( TerrainDefs[i].humanoid );std::cout << std::endl;
    std::cout << "  lair =";
    for( int j=0; j<6; j++ ) {
      int id_lair = TerrainDefs[i].lairs[j];
      if( id_lair >= 0 ) {
	std::cout <<  ObjectDefs[id_lair].name << ", ";
      }
      else {
	std::cout << "-1, ";
      }
    }
      std::cout << std::endl;
  }
}
void list_elements()
{
  //Game game;
  initIO();

    _game.ModifyTablesPerRuleset();

  MapAccess mapAccess = MapAccess( &_game );

  // list_skills();
  // list_objects();
  //list_terrain();
  list_monsters();
}
int main(int argc, char *argv[])
{
  list_elements();
  
  return 0;
}

