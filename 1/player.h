#ifndef PLAYER_H
#define PLAYER_H

#include "unit.h"

class Player
{
public:
    Player();

    void AddUnit(Unit *unit);

private:
    int moneyPoints;
    int moneyPointsIncome;

    //если будет наука надо добавить два поля по аналогии с деньгами

    // int sciencePoints;
    // int sciencePointsIncome;

    QVector<Unit *> availableUnits;

};

#endif // PLAYER_H
