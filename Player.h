#ifndef PLAYER_H
#define PLAYER_H

#include <QPainter>
#include <QJsonObject>
#include <QVariant>
#include <QList>
#include "Action.h"
#include "ModelConstants.h"
#include "PlayerRole.h"
#include "BaseEntity.h"


class Player:
        public BaseEntity
{
    Q_GADGET;

protected:
    static const QVector<PlayerRole> roles;
    PlayerRole playerRole;
    PlayerRole::RoleName assignedRole = PlayerRole::RoleName::Unassigned;

    int id;
    static int playerCount;

public:
    enum Team {ally, enemy};
    Q_ENUM(Team)

    Player();
    Player(Team team);
    Player(Team _team, double _x, double _y, double _angle);
    ~Player();

    QRectF getKickableAreaRect();
    QRectF getIntentionsKickableAreaRect();
    QRectF getPlayerWidgetRect();
    PlayerRole getPlayerRole() const;
    Player::Team getTeam() const;
    static int getPlayerCount();

    void readJSON(const QJsonObject &json);
    void writeJSON(QJsonObject &json) const;
    static QVector<PlayerRole> getRoles();
    int getId() const;



protected:
    Team team;
};

#endif // PLAYER_H
