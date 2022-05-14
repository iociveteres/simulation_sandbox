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

#include "Player.h"
#include "Ball.h"

class World
{
private:
    QVector<Player> teamAlly;
    QVector<Player> teamEnemy;

    Ball* ball;

public:
    enum SaveFormat {
            Json, Binary
        };

    World();
    void writeJSON(QJsonObject &json) const;
    void readJSON(const QJsonObject &json);
    void populate();
    bool loadWorld(World::SaveFormat saveFormat);
    bool saveWorld(World::SaveFormat saveFormat) const;

    World getWorld();
    QVector<Player> getTeamAlly() const;
    QVector<Player> getTeamEnemy() const;
    Ball *getBall() const;
};

#endif // WORLD_H
