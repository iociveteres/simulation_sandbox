#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <QFile>
#include <QTextStream>
#include <QVector>
#include <QJsonArray>

#include "player.h"
#include "ball.h"

class World
{
private:
    QVector<Player> teamAlly;
    QVector<Player> teamEnemy;

    Ball ball;

public:
    World();
    void writeJSON(QJsonObject &json) const;
    void readJSON(const QJsonObject &json);
};

#endif // WORLD_H
