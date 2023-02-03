#ifndef EXTRACTIONBUILDING_H
#define EXTRACTIONBUILDING_H

#include "building.h"

class ExtractionBuilding : public Building
{
public:
    ExtractionBuilding(const BuildingType &type,QVector3D pos,const int& position);

private:
    int income;

};

#endif // EXTRACTIONBUILDING_H
