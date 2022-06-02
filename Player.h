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
#include "Geometry.h"

class Player:
        public BaseEntity
{
    Q_GADGET;

protected:
    static const QVector<PlayerRole> defaultFormation;
    PlayerRole playerRole;
    PlayerRole::RoleName assignedRole = PlayerRole::RoleName::Unassigned;

    int id;
    static int playerCount;

public:
    enum Team {ally, enemy};
    Q_ENUM(Team)

    Player();
    Player(bool* a);
    Player(Team team);
    Player(Team _team, double _x, double _y, double _angle);
    Player(Team _team, double _x, double _y, double _angle, int _id);
    Player(const Player &p);
    ~Player();
    PlayerRole choosePlayerRole();

    QRectF getKickableAreaRect();
    QRectF getKickableAreaRect(QPointF pos);
    QRectF getPlayerWidgetRect();
    Player::Team getTeam() const;
    static int getPlayerCount();

    PlayerRole getPlayerRole() const;
    void setPlayerRole(const PlayerRole &value);

    void readJSON(const QJsonObject &json);
    void writeJSON(QJsonObject &json) const;
    static QVector<PlayerRole> getDefaultFormation();
    int getId() const;

    QPointF getDefaultRolePosition(PlayerRole::RoleName roleName) const;

    void tick();
    static std::map<int, PlayerRole::RoleName> player_roleMap;
    PlayerRole::RoleName getAssignedRole() const;

protected:
    Team team;
};

#endif // PLAYER_H
