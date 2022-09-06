#ifndef IPLAYER_H
#define IPLAYER_H

#include "unit.h"
#include "building.h"

class IPlayer
{
public:
    virtual ~IPlayer(){};

    virtual void AddUnit(Unit *unit) = 0;
    virtual void AddBuilding(Building *building) = 0;
    virtual QImage &GetPlayerTexture() = 0;
    virtual void RestoreUnitsStamina() = 0;
    virtual QVector<WorldEngineBase *> GetUnits() = 0;

};

#endif // IPLAYER_H
