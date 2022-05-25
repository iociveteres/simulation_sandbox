#ifndef PLAYERAI_H
#define PLAYERAI_H

#include "Player.h"
//#include "PlayerWorldModel.h"

class PlayerAI:
        public Player
{
    Q_GADGET;

private:
    Action intention;
    //PlayerWorldModel* worldModel;

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

    double checkMarking(Player enemy);
    double checkDefendGoal(Player enemy);
    double checkWaitDefensive(Player enemy);

    Action getIntention() const;

private:
    Team team;
};

#endif // PLAYERAI_H
