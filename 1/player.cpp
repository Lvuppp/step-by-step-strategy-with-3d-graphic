#include "player.h"

Player::Player()
{

}

void Player::AddUnit(Unit *unit)
{
   availableUnits.append(unit);
}

