#ifndef DEFENSEBUILDING_H
#define DEFENSEBUILDING_H

#include "building.h"

class DefenseBuilding : public Building
{
public:
    DefenseBuilding(const int& levelOfBuilding, const int& positionOnBlock);

private:
    int levelOfDefense;
};

#endif // DEFENSEBUILDING_H
