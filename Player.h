#ifndef PLAYER_H
#define PLAYER_H

#include <QPainter>
#include <QJsonObject>
#include <QVariant>
#include <QList>
#include "Action.h"
#include "ModelConstants.h"
#include "PlayerRole.h"
//#include "PlayerWorldModel.h"

class Player
{
    Q_GADGET;

private:
    static const QVector<PlayerRole> roles;
    PlayerRole playerRole;
    PlayerRole::RoleName assignedRole = PlayerRole::RoleName::Unassigned;
    Action intention;
    //PlayerWorldModel worldModel;

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
    double getAngle();
    double getY();
    double getX();
    QPointF getCoordinatesPoint();
    static QVector<PlayerRole> getRoles();
    static int getPlayerCount();

    void readJSON(const QJsonObject &json);
    void writeJSON(QJsonObject &json) const;

    QList<Action> makePrefferedActionsList();
    void determinePrefferedIntention();

    double checkMarking(Player enemy);
    double checkDefendGoal(Player enemy);
    double checkWaitDefensive(Player enemy);

    Action getIntention() const;

private:
    Team team;
};

#endif // PLAYER_H
