#ifndef PLAYER_H
#define PLAYER_H

#include <QPainter>
#include <QJsonObject>
#include <QVariant>
#include <QList>
#include "Action.h"
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

    int id;
    static int playerCount;

public:
    enum Team {ally, enemy};
    Q_ENUM(Team)

    Player();
    Player(Team team);
    Player(Team _team, int _x, int _y, double _angle);
    ~Player();

    void setPosition();
    void setAngle();
    void setRole(PlayerRole role); // set role
    PlayerRole checkRole(); // check role based on position

    QRectF getKickableAreaRect();
    QRectF getIntentionsKickableAreaRect();
    QRectF getPlayerWidgetRect();
    Player::Team getTeam() const;
    int getAngle();
    int getY();
    int getX();
    static QVector<PlayerRole> getRoles();
    static int getPlayerCount();

    void readJSON(const QJsonObject &json);
    void writeJSON(QJsonObject &json) const;

    QList<Action> makePrefferedActionList();

private:
    Team team;
};

#endif // PLAYER_H
