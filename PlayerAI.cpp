#include "PlayerAI.h"

double PlayerAI::getRoleMargin() const
{
    return roleMargin;
}

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

PlayerAI::~PlayerAI()
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

    QList<QList<Action>> actionsOfPlayers;
    for (Player p: worldModel->getTeamAlly()) {
        QList<Action> actionsOfPlayer;

        for (Player e: worldModel->getTeamEnemy()) {
            checkMarking(e);
            checkDefendGoal(e);
            checkWaitDefensive(e);
        }

        actionsOfPlayers.append(actionsOfPlayer);
    }

    return QList<Action>();
}
// let every ally determine what action should he execute,
// based on desirability of action
// return list, containing list of Actions for every player;
void PlayerAI::determinePrefferedIntention()
{
    makePrefferedActionsList();
}

Action PlayerAI::checkMarking(Player enemy)
{
    const double a = 1;
    const double b = 1;
    const double c = 1;
    double distBtwEnemyAndBall =
            distance(enemy.getCoordinatesPoint(),
                     worldModel->getBall()->getCoordinatesPoint());
    double distBtwEnemyAndMe =
            distance(enemy.getCoordinatesPoint(),
                     this->getCoordinatesPoint());
    double distBtwEnemyAndDefensiveLine = enemy.getX() - getRoleMargin();

    double desirebility =
            a * distBtwEnemyAndBall +
            b * distBtwEnemyAndMe +
            c * distBtwEnemyAndDefensiveLine;

    return Action();
}

Action PlayerAI::checkDefendGoal(Player enemy)
{
    const double a = 1;
    const double b = 1;
    const double c = 1;

    double distBtwGoalAndBall =
            distance(QPointF(r_PITCH_MARGIN, r_TOTAL_FIELD_WIDTH/2),
                     worldModel->getBall()->getCoordinatesPoint());

    double ballIsControlledByEnemy;
    try {
        worldModel->getEnemyControllingBall();
        ballIsControlledByEnemy = 20;
    }  catch (std::runtime_error const &e) {
        //std::string s = e.what();
        ballIsControlledByEnemy = 0;
    }

    const QRectF defendGoalArea = QRectF(r_PITCH_MARGIN + r_GOAL_AREA_LENGTH + 0.5,
                                         r_PITCH_MARGIN + r_PITCH_WIDTH/3,
                                         4,
                                         r_PITCH_WIDTH/3);

    double lessThan2AlliesDefendGoal;
    int howManyDefend = worldModel->getHowManyPlayersAreInArea(defendGoalArea);
    if (howManyDefend < 2) {
        lessThan2AlliesDefendGoal = 20;
    } else {
        lessThan2AlliesDefendGoal = 0;
    }

    double desirebility =
            a * 1/distBtwGoalAndBall +
            b * ballIsControlledByEnemy +
            c * lessThan2AlliesDefendGoal;


    return Action();
}

Action PlayerAI::checkWaitDefensive(Player enemy)
{
    double desirebility = 25;
    return Action();
}


