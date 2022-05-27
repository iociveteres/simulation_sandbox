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

Player PlayerWorldModel::getEnemyClosestToTheBall()
{
    double minVal = 10000;
    Player minPlayer;
    for (Player e: getTeamEnemy()) {
        double curVal = distance(e.getCoordinatesPoint(), ball->getCoordinatesPoint());
        if (curVal < minVal) {
            minVal = curVal;
            minPlayer = e;
        }
    }

    if (minVal != 10000)
        return minPlayer;
    else
        throw std::runtime_error("Empty enemy team vector");

}

Player PlayerWorldModel::getEnemyControllingBall()
{
    Player p = getEnemyClosestToTheBall();

    if (distance(p.getCoordinatesPoint(), ball->getCoordinatesPoint())
            <= r_KICKABLE_AREA + 1)
        return p;
    else
        throw std::runtime_error("No enemy controlling ball");
}

Player PlayerWorldModel::getEnemyById(int id)
{
    for (Player e: teamEnemy) {
        if (id == e.getId()) {
            return e;
        }
    }
    return Player();
}

Player PlayerWorldModel::getAllyById(int id)
{
    for (Player a: teamAlly) {
        if (id == a.getId()) {
            return a;
        }
    }
    return Player();
}

int PlayerWorldModel::getHowManyPlayersAreInArea(QRectF area)
{
    int count = 0;


    for (Player a: getTeamAlly()) {
        if (area.contains(a.getCoordinatesPoint()))
            count++;
    }

    return count;
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

