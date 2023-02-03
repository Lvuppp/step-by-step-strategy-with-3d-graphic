#ifndef IPLAYER_H
#define IPLAYER_H

#include "unit.h"
#include "defensebuilding.h"

class IPlayer
{
public:
    virtual ~IPlayer(){};

    virtual void AddUnit(Unit *unit) = 0;
    virtual void AddBuilding(DefenseBuilding *building) = 0;
    virtual QImage *GetPlayerColor(int type) = 0;
    virtual void RestoreUnitsStamina() = 0;
    virtual QVector<WorldEngineBase *> GetUnits() = 0;
    virtual QVector<Unit *> GetPlayerUnits() = 0;
    virtual QVector<DefenseBuilding *> GetPlayerBuildings() = 0;
    virtual void IncreaseMoney() = 0;
    virtual void DecreaseMoney(int money) = 0;
    virtual void IncreaseMoneyIncome(int increaseMoney) = 0;
    virtual void DecreaseMoneyIncome(int decreaseMoney) = 0;
    virtual int GetMoney() = 0;
    virtual int GetMoneyIncome() = 0;
};

#endif // IPLAYER_H
