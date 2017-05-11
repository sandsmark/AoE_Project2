#ifndef _TechTree_
#define _TechTree_

#include "Module.h"
#include "Building.h"
#include "Entity.h"
#include "Unit.h"

enum TechMultiplier {

	//units 
	MELEE_UNIT_ATTACK, RANGED_UNIT_ATTACK, CAVALRY_ATTACK,    MELEE_UNIT_DEFENSE, RANGED_UNIT_DEFENSE, CAVALRY_DEFENSE,
	MELEE_UNIT_SPEED, RANGED_UNIT_SPEED, CAVALRY_SPEED,       MELEE_UNIT_COST, RANGED_UNIT_COST, CAVALRY_COST,

	//
	MAX_MULTIPLIERS
};

enum TechType {

	NO_TECH,

	RANGED_WEAPONS,  
	HORSE_TRAINING,  
	TOWN_MILITIA,    

	ENHANCED_BLACKSMITHING,   
	MOUNTAIN_FORGE,           
	DWARVEN_STEEL,            
	DWARVEN_TACTICS,          

	ENHANCED_TRAINING,        
	ELVEN_TACTICS,            
	ELVEN_STEEL,              
	LIBRARY,                  

	ENHANCED_VILLAGERS,       
	HUMAN_WARFARE,            
	ROHAN_HORSES,             
	DUNEDAIN_OUTPOST,

};

struct Tech {
	
	Tech() {
		unlocks_building = (buildingType)-1;
		unlocks_unit.first = (unitType)-1;
		multipliers.first = -1;

	}

	string name;
	string desc;
	TechType id;

	list<TechType> unlocks_tech;
	pair<unitType, buildingType> unlocks_unit;
	pair<float, TechMultiplier> multipliers;
	buildingType unlocks_building;

	buildingType researched_in;
	bool researched = false;
	Cost cost;
	int research_time;
	Timer research_timer;

};


class TechTree {
public:
	list<pair<unitType, buildingType>> available_units;
	list<TechType> available_techs;
	list<buildingType> available_buildings;
	vector<float> multiplier_list;

	vector<Tech*> all_techs;

public:

	void Start(pugi::xml_node gameData);
	void Update();

	void LoadTechTree(pugi::xml_node gameData);

	void StartResearch(TechType tech_id);
	void Researched(TechType tech_id);

};


#endif