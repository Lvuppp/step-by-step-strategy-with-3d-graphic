#include "player.h"

Player::Player()
{

}

Player::Player(QVector<QImage *>playerColor) :playerColor(playerColor)
{

}

void Player::AddUnit(Unit *unit)
{
    playerUnits.append(unit);
    selectedPlayerUnits.append(playerUnits.last());
}

void Player::AddBuilding(DefenseBuilding *building)
{
    playerBuildings.append(building);
}

QImage *Player::GetPlayerColor(int type)
{
    return playerColor[type];
}

QVector<WorldEngineBase *> Player::GetUnits(){
    return selectedPlayerUnits;
}

QVector<Unit *> Player::GetPlayerUnits()
{
    return playerUnits;
}

QVector<DefenseBuilding *> Player::GetPlayerBuildings()
{
    return playerBuildings;
}

void Player::IncreaseMoney()
{
    money += moneyIncome;
}

void Player::DecreaseMoney(int decreaseMoney)
{
     money -= decreaseMoney;
}

void Player::IncreaseMoneyIncome(int increaseMoney)
{
    moneyIncome += increaseMoney;
}

void Player::DecreaseMoneyIncome(int decreaseMoney)
{
    moneyIncome -= decreaseMoney;
}

int Player::GetMoney()
{
    return money;
}

int Player::GetMoneyIncome()
{
    return moneyIncome;
}

void Player::RestoreUnitsStamina()
{
    foreach (auto unit, playerUnits) {
        unit->RestoreStamina();
    }
}

