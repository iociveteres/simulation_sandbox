#ifndef PLAYERWORLDMODEL_H
#define PLAYERWORLDMODEL_H

#include <stdexcept>
#include <QVector>
#include "Player.h"
#include "Ball.h"
#include "Globals.h"

class World;

class PlayerWorldModel
{
private:
    Player* myself;
    Ball* ball;
    int allyCount;
    QVector<Player> teamAlly;
    int enemyCount;
    QVector<Player> teamEnemy;
    QVector<PlayerRole> formation;

    QList<PlayerWorldModel*> pastStates;

public:
    PlayerWorldModel();
    PlayerWorldModel(const World& world, Player* _myself);

    QVector<Player> getTeamAlly() const;
    QVector<Player> getTeamEnemy() const;
    Ball *getBall() const;

    std::tuple<bool, Player> getEnemyClosestToTheBall();
    std::tuple<bool, Player> getEnemyControllingBall();
    Player getAllyClosestToThePoint(QPointF p);

    Player getEnemyById(int id);
    std::tuple<bool, Player> getAllyById(int id);
    Player getAllyByRoleName(PlayerRole::RoleName roleName);
    PlayerRole getPlayerRoleByRoleName(PlayerRole::RoleName  roleName);
    PlayerRole getPlayerRoleByAllyId(int id);

    int getHowManyPlayersAreInArea(QRectF area);

    void determineFormation();

    void introduceNoises();
    void limitVisionDist();
    void limitVisionCone();

    void update(const World &world, Player *_myself);

    void checkFormationIsInBox(QPointF shift);
    int getEnemyCount() const;

    QVector<PlayerRole> getFormation() const;
};

#include "World.h"

#endif // PLAYERWORLDMODEL_H
