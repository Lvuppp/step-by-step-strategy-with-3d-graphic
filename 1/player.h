#ifndef PLAYER_H
#define PLAYER_H

#include "unit.h"
#include "iplayer.h"

class Player : public IPlayer
{
public:
    Player();

    ~Player(){
    };

    void AddUnit(Unit *unit) override;
    void AddBuilding(Building *building) override;
    QImage &GetPlayerTexture() override;
    void RestoreUnitsStamina() override;
    QVector<WorldEngineBase *> GetUnits() override;

private:
    int moneyPoints;
    int moneyPointsIncome;

    //если будет наука надо добавить два поля по аналогии с деньгами

    // int sciencePoints;
    // int sciencePointsIncome;

    QVector<WorldEngineBase *> selectedPlayerUnits;
    QVector<Unit *> playerUnits;
    QVector<Building *> playerBuilding;
    QImage playerTexture; // сделать либо выбор текстуры, либо просто на рандом
                                                //(Каждая текстура это просто разный цвет)
};

#endif // PLAYER_H
