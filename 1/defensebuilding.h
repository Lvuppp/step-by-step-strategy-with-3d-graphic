#ifndef DEFENSEBUILDING_H
#define DEFENSEBUILDING_H

#include "building.h"

class DefenseBuilding : public Building
{
public:

    DefenseBuilding(const BuildingType &type,QVector3D pos,const int& position);

    QVector<qsizetype> GetProtectedBlocks(const qsizetype &floorSquare, const QVector<QVector<qsizetype>> mapMatrix);

private:
    int levelOfDefense;
};

#endif // DEFENSEBUILDING_H
