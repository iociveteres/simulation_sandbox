#ifndef PLAYERWORLDMODEL_H
#define PLAYERWORLDMODEL_H

#include <stdexcept>
#include <QVector>
#include "Player.h"
#include "Ball.h"


class World;

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

    Player getEnemyClosestToTheBall();
    Player getEnemyControllingBall();
    int getHowManyPlayersAreInArea(QRectF area);

    void introduceNoises();
    void limitVisionDist();
    void limitVisionCone();

    void update();


};

#include "World.h"

#endif // PLAYERWORLDMODEL_H
