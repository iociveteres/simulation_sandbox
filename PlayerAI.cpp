#include "PlayerAI.h"

PlayerAI::PlayerAI():
    Player()
{

}

PlayerAI::PlayerAI(Team _team):
    Player(_team)
{

}

PlayerAI::PlayerAI(Team _team, int _x, int _y, double _angle):
    Player(_team, _x, _y, _angle)
{

}

Action PlayerAI::getIntention() const
{
    return intention;
}

void PlayerAI::setRole(PlayerRole role)
{
    playerRole = role;
}

PlayerRole PlayerAI::checkRole()
{
//    for (PlayerRole r: roles) {

//    }
    return Goalie();
}


QRectF PlayerAI::getIntentionsKickableAreaRect()
{
    double _x = this->intention.getPrefferedPoint().x();
    double _y = this->intention.getPrefferedPoint().y();
    return QRectF(r_PITCH_MARGIN + _x - r_KICKABLE_AREA/2,
                  r_PITCH_MARGIN + _y - r_KICKABLE_AREA/2,
                 r_KICKABLE_AREA, r_KICKABLE_AREA);
}

// calculate level of desirability for possible actions
// for every known ally against every known enemy
// return list, containing list of Actions for every player;
QList<Action> PlayerAI::makePrefferedActionsList()
{
    /*
    QList<QList<Action>> actionsOfPlayers;
    for (Player p: worldModel->getTeamAlly()) {
        QList<Action> actionsOfPlayer;

        for (Player e: worldModel->getTeamEnemy()) {
            //checkMarking();
            //checkDefendGoal();
            //checkWaitDefensive();
        }

        actionsOfPlayers.append(actionsOfPlayer);
    }
*/
    return QList<Action>();
}
// let every ally determine what action should he execute,
// based on desirability of action
// return list, containing list of Actions for every player;
void PlayerAI::determinePrefferedIntention()
{
    makePrefferedActionsList();
}
