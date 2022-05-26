#ifndef PLAYERAI_H
#define PLAYERAI_H

#include "Player.h"
#include "PlayerWorldModel.h"
#include "Geometry.h"

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

    void setRole(PlayerRole role); // set role
    PlayerRole checkRole(); // check role based on position

    QRectF getIntentionsKickableAreaRect();

    static int getPlayerCount();

    QList<Action> makePrefferedActionsList();
    void determinePrefferedIntention();

    Action checkMarking(Player enemy);
    Action checkDefendGoal(Player enemy);
    Action checkWaitDefensive(Player enemy);

    Action getIntention() const;
    double getRoleMargin() const;

private:
    Team team;
};

#endif // PLAYERAI_H
