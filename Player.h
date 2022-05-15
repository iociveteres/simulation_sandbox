#ifndef PLAYER_H
#define PLAYER_H

#include <QPainter>
#include <QJsonObject>
#include <QVariant>
#include "ModelConstants.h"
#include "PlayerRole.h"

class Player
{
    Q_GADGET;

private:
    static const QVector<PlayerRole> roles;
    PlayerRole playerRole;
    PlayerRole::RoleName assignedRole = PlayerRole::RoleName::Unassigned;

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
    void setRole(PlayerRole role); // set role
    PlayerRole checkRole(); // check role based on position

    QRectF getKickableAreaRect();
    QRectF getPlayerWidgetRect();
    Player::Team getTeam() const;
    int getAngle();
    int getY();
    int getX();
    static QVector<PlayerRole> getRoles();

    void readJSON(const QJsonObject &json);
    void writeJSON(QJsonObject &json) const;
private:
    Team team;
};

#endif // PLAYER_H
