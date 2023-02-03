#ifndef PLAYER_H
#define PLAYER_H

#include "unit.h"
#include "iplayer.h"

class Player : public IPlayer
{
public:
    Player();
    Player(QVector<QImage *> playerColor);

    ~Player(){
    };

    void AddUnit(Unit *unit) override;
    void AddBuilding(DefenseBuilding *building) override;
    QImage *GetPlayerColor(int type) override;
    void RestoreUnitsStamina() override;
    QVector<WorldEngineBase* > GetUnits() override;
    QVector<Unit* > GetPlayerUnits() override;
    QVector<DefenseBuilding* > GetPlayerBuildings() override;
    void IncreaseMoney() override;
    void DecreaseMoney(int money)  override;
    void IncreaseMoneyIncome(int increaseMoney) override;
    void DecreaseMoneyIncome(int decreaseMoney) override;
    int GetMoney() override;
    int GetMoneyIncome() override;

private:
    int money = 0;
    int moneyIncome = 0;

    //если будет наука надо добавить два поля по аналогии с деньгами

    // int sciencePoints;
    // int sciencePointsIncome;

    QVector<WorldEngineBase *> selectedPlayerUnits;
    QVector<Unit *> playerUnits;
    QVector<DefenseBuilding *> playerBuildings;
    QVector<QImage *>playerColor ; // сделать либо выбор текстуры, либо просто на рандом
                                                //(Каждая текстура это просто разный цвет)
};

#endif // PLAYER_H
