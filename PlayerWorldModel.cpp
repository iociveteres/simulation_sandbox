#include "PlayerWorldModel.h"

PlayerWorldModel::PlayerWorldModel(const World& world, Player* _myself)
{
    myself = _myself;
    teamAlly = world.makePlayerTeamAllyForWorldView();
    teamEnemy = world.getTeamEnemy();
    ball = world.getBall();
}

QVector<Player> PlayerWorldModel::getTeamAlly() const
{
    return teamAlly;
}

QVector<Player> PlayerWorldModel::getTeamEnemy() const
{
    return teamEnemy;
}

Ball *PlayerWorldModel::getBall() const
{
    return ball;
}

void PlayerWorldModel::introduceNoises()
{

}

void PlayerWorldModel::limitVisionDist()
{

}

void PlayerWorldModel::limitVisionCone()
{

}

void PlayerWorldModel::update() {

}

