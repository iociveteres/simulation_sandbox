#ifndef PLAYERWORLDMODEL_H
#define PLAYERWORLDMODEL_H

#include <QVector>
#include "World.h"
#include "Player.h"
#include "Ball.h"

class PlayerWorldModel
{
private:
    Player* myself;
    Ball* ball;
    QVector<Player> teamAlly;
    QVector<Player> teamEnemy;


public:
    PlayerWorldModel(const World& world, Player* _myself);

    QVector<Player> getTeamAlly() const;
    QVector<Player> getTeamEnemy() const;
    Ball *getBall() const;

    void introduceNoises();
    void limitVisionDist();
    void limitVisionCone();

};

#endif // PLAYERWORLDMODEL_H
