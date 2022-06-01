#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <QFile>
#include <QTextStream>
#include <QVector>
#include <QJsonArray>
#include <QJsonDocument>
#include <QCborValue>
#include <QCborMap>

#include "Ball.h"

class PlayerAI;
class Player;

class World : public QObject
{
    Q_OBJECT
private:
    QVector<PlayerAI> teamAlly;
    QVector<Player> teamEnemy;

    Ball* ball;

public slots:
    void handlePlayButton();

public:
    enum SaveFormat {
            Json, Binary
        };

    //World(const World& a) : teamAlly(a.teamAlly), teamEnemy(a.teamEnemy), ball(a.ball) { } // user-defined copy ctor

    World();
    void writeJSON(QJsonObject &json) const;
    void readJSON(const QJsonObject &json);

    bool loadWorld(World::SaveFormat saveFormat, QString fileName);
    bool saveWorld(World::SaveFormat saveFormat) const;

    //World getWorld();
    QVector<PlayerAI> getTeamAlly() const;
    QVector<Player> getTeamEnemy() const;
    Ball *getBall() const;

    QVector<Player> makePlayerTeamAllyForWorldView(PlayerAI forPlayer) const;

    void populate();

};

#include "PlayerAI.h"

#endif // WORLD_H
