#include "player.h"

Player::Player()
{
    playerTexture = QImage(":/cl_ppl.jpg");
}

void Player::AddUnit(Unit *unit)
{
    playerUnits.append(unit);
    selectedPlayerUnits.append(playerUnits.last());
}

void Player::AddBuilding(Building *building)
{
    playerBuilding.append(building);
}

QImage &Player::GetPlayerTexture()
{
    return playerTexture;
}

QVector<WorldEngineBase *> Player::GetUnits(){
    return selectedPlayerUnits;
}

void Player::RestoreUnitsStamina()
{
    foreach (auto unit, playerUnits) {
        unit->RestoreStamina();
    }
}

