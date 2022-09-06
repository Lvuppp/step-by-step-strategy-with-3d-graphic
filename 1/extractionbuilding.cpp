#include "extractionbuilding.h"

ExtractionBuilding::ExtractionBuilding(const int &levelOfBuilding, const int &position) : Building(position)
{
    income = levelOfBuilding * 7;
    cost = levelOfBuilding * 12;
}
