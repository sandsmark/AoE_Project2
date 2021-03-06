#ifndef __BUILDING_H__
#define __BUILDING_H__

#include "Entity.h"
#include "Animation.h"
#include "Unit.h"
#include "Timer.h"
#include <list>
#include <deque>

#define TECHBAR_WIDTH 50

enum buildingType {
    INVALID_BUILDING = -1,
    TOWN_CENTER,
    HOUSE,
    BARRACKS,
    ARCHERY_RANGE,
    STABLES,
    SIEGE_WORKSHOP,
    MARKET,
    BLACKSMITH,
    MILL,
    OUTPOST,
    MONASTERY,
    CASTLE,

    SAURON_TOWER,
    ORC_HOUSE,
    ORC_BARRACKS,
    ORC_ARCHERY_RANGE,
    BEAST_PIT,
    URUK_HAI_PIT,
    ORC_MARKET,
    ORC_BLACKSMITH,

};

class Unit;
class Order;

class Building : public Entity
{
public:
    Building();
    Building(int posX, int posY, Building *building);
    ~Building();

    bool Update(float dt) override;
    bool Draw() override;
    void Destroy() override;
    void GetBuildingBoundaries();

    bool Load(pugi::xml_node &) override;
    bool Save(pugi::xml_node &) const override;
    void drawTechnology(int, int);
    void drawUnitsInQueue(int MaxTime, int Time, bool isHero);
    iPoint techpos;

private:
    Timer mill_food;

public:
    //STATS:
    buildingType type = ORC_BARRACKS;
    int buildingPiercingDamage = 0;
    Cost cost;
    bool canAttack = false;
    int selectionWidth = 0;

    //Utilities
    Timer attack_timer;
    Timer creation_timer;
    uint aux_timer = 0;
    uint imageWidth = 0;
    uint imageHeight = 0;
    Collider *range = nullptr;
    deque<unitType> units_in_queue;
};

#endif // !__BUILDING_H__
