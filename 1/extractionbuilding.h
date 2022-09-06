#ifndef EXTRACTIONBUILDING_H
#define EXTRACTIONBUILDING_H

#include "building.h"

class ExtractionBuilding : public Building
{
public:
    ExtractionBuilding(const int &levelOfBuilding, const int &positionOnBlock);

private:
    int income;

};

#endif // EXTRACTIONBUILDING_H
