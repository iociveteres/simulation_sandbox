#ifndef PLAYERAI_H
#define PLAYERAI_H

#include "Player.h"
#include "PlayerWorldModel.h"
#include "Geometry.h"
#include <tuple>

class PlayerAI:
        public Player
{
    Q_GADGET;

private:
    double roleMargin;
    Action intention;
    PlayerWorldModel* worldModel;

public:
    PlayerAI();
    PlayerAI(Team team);
    PlayerAI(Team _team, int _x, int _y, double _angle);
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

    Action checkMarking(Player enemy);
    Action checkDefendGoal();
    Action checkIntercept();
    Action checkWaitDefensive();


    QPointF getPointMarking(Action a);
    QRectF findDefendGoalArea();
    double getGoalCoverageRatingFromPlayerPos(QPointF playerPos);
    std::tuple<QPointF, QPointF> getPointsDefendGoal(Action a);
    QPointF getPointIntercept();
    QPointF getPointWait();
    double calcRoleMarginFromGoal();
    bool checkAllyIsInPlace(PlayerRole::RoleName roleName);
private:
    Team team;
};

#endif // PLAYERAI_H
