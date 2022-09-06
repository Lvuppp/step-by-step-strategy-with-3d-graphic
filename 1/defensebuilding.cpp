#include "defensebuilding.h"

DefenseBuilding::DefenseBuilding(const int &levelOfBuilding, const int &position) : Building(position)
{
    levelOfDefense = levelOfBuilding + 1;
    cost = levelOfBuilding * 15;
}
