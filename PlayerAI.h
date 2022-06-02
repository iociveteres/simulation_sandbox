#ifndef PLAYERAI_H
#define PLAYERAI_H

#include "Player.h"
#include "PlayerWorldModel.h"
#include "Geometry.h"
#include "Globals.h"
#include <tuple>
#include <memory>

class PlayerAI:
        public Player
{
    Q_GADGET;

private:
    double roleMargin;
    Action intention;
    PlayerWorldModel* worldModel;
    World* worldLink;

public:
    PlayerAI();
    PlayerAI(Team team, World *_world);
    PlayerAI(Team _team, int _x, int _y, double _angle, World *_world);
    ~PlayerAI();

    void cycle();

    void setRole(PlayerRole role); // set role
    PlayerRole checkRole(); // check role based on position

    QRectF getIntentionsKickableAreaRect();

    static int getPlayerCount();
    Action getIntention() const;
    double getRoleMargin() const;

    QList<Action> makePrefferedActionsListOthers();
    QList<Action> makePrefferedActionsListMe();
    QList<Action> makePrefferedActionsListNeighbours();
    Action determinePrefferedIntention();

    Action checkMarking(Player enemy, Player player);
    Action checkDefendGoal(Player player);
    Action checkIntercept(Player player);
    Action checkWaitDefensive(Player player);

    QPointF getPointMarking(Action a, Player player);

    QRectF findDefendGoalArea();
    double getGoalCoverageRatingFromPlayerPos(QPointF playerPos);

    std::tuple<QPointF, QPointF> getPointsDefendGoal(Action a, Player player);

    std::tuple<bool, QPointF> getPointIntercept(Player player);

    QPointF getPointWait(Player player);

    double calcRoleMarginFromGoal();
    bool checkAllyIsInPlace(PlayerRole::RoleName roleName);
private:
    Team team;
};

#endif // PLAYERAI_H
