#ifndef PLAYER_H
#define PLAYER_H

#include <QPainter>
#include <QJsonObject>
#include <QVariant>
#include "ModelConstants.h"


class Player
{
    Q_GADGET;

private:
    bool isGoalie = false;

    double x;
    double y;
    double angle;

public:
    enum Team {ally, enemy};
    Q_ENUM(Team)

    Player();
    Player(Team team);
    Player(Team _team, int _x, int _y, double _angle);

    void setPosition();
    void setAngle();

    QRectF getKickableAreaRect();
    QRectF getPlayerWidgetRect();
    Player::Team getTeam() const;
    int getAngle();
    int getY();
    int getX();

    void readJSON(const QJsonObject &json);
    void writeJSON(QJsonObject &json) const;
private:
    Team team;
};

#endif // PLAYER_H
