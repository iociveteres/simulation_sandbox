#include "PlayerAI.h"

double MarkingDist = 4;
QPointF allyGoalTopPoint(r_PITCH_MARGIN, r_TOTAL_FIELD_WIDTH/2 - r_GOAL_WIDTH/2);
QPointF allyGoalBotPoint(r_PITCH_MARGIN, r_TOTAL_FIELD_WIDTH/2 - r_GOAL_WIDTH/2 + r_GOAL_WIDTH);
const QPointF leftPitchRimTopPoint(r_PITCH_MARGIN, r_PITCH_MARGIN);
const QPointF leftPitchRimBotPoint(r_PITCH_MARGIN, r_PITCH_MARGIN + r_PITCH_WIDTH);
const QRectF defendGoalArea(r_PITCH_MARGIN + r_GOAL_AREA_LENGTH + 0.5,
                              r_PITCH_MARGIN + r_PITCH_WIDTH/3,
                              4,
                              r_PITCH_WIDTH/3);
const QLineF defendGoalLine(r_PITCH_MARGIN + r_GOAL_AREA_LENGTH + 2.5,
                              r_PITCH_MARGIN + r_PITCH_WIDTH/3,
                              r_PITCH_MARGIN + r_GOAL_AREA_LENGTH + 2.5,
                              r_PITCH_MARGIN + 2*r_PITCH_WIDTH/3);

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
QList<QList<Action>> PlayerAI::makePrefferedActionsList()
{
    QList<QList<Action>> actionsOfPlayers;
    for (Player p: worldModel->getTeamAlly()) {
        QList<Action> actionsOfPlayer;

        for (Player e: worldModel->getTeamEnemy()) {
            Action a = checkMarking(e);
            a.setPrefferedPoint(getPointMarking(a));
            actionsOfPlayer.append(a);
        }
        {
            Action a = checkWaitDefensive();
            a.setPrefferedPoint(getPointWait(a));
            actionsOfPlayer.append(a);
        }
        {
            Action a = checkDefendGoal();
            Action b = a;
            std::tuple<QPointF, QPointF> t = getPointsDefendGoal(a);
            a.setPrefferedPoint(std::get<0>(t));
            b.setPrefferedPoint(std::get<1>(t));
            actionsOfPlayer.append(a);
            actionsOfPlayer.append(b);
        }
        //actionsOfPlayer.append();

        actionsOfPlayers.append(actionsOfPlayer);
    }

    return actionsOfPlayers;
}
// base cycle
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

    return Action(Action::Mark, this->getId(), enemy.getId(), desirebility);
}

Action PlayerAI::checkDefendGoal()
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


    return Action(Action::DefendGoal, this->getId(), Action::NoEnemyCode, desirebility);
}

Action PlayerAI::checkWaitDefensive()
{
    double desirebility = 25;
    return Action(Action::Wait, this->getId(), Action::NoEnemyCode, desirebility);
}


QPointF PlayerAI::getPointMarking(Action a) {
//    QPointF executorPos = worldModel->
//            getAllyById(a.getExecutorId()).
//            getCoordinatesPoint();
    QPointF ballPos = worldModel->
            getBall()->
            getCoordinatesPoint();
    QPointF againstPos = worldModel->
            getEnemyById(a.getAgainstId()).
            getCoordinatesPoint();
    return findPointInDistFromEndOfSegment(againstPos, ballPos, MarkingDist);
}

// search for best point in each part
std::tuple<QPointF, QPointF> PlayerAI::getPointsDefendGoal(Action a) {
    double defendLineX = defendGoalLine.x1();
    QPointF curPoint;
    QPointF maxPoint;
    double curValue = -1;
    double maxValue = -1;

    // If accidentially there is 0 coverage, which can occur
    // if ball is to right or left on the field
    // we should pick point leftest or rightest points to the
    // upper part
    for (double y = defendGoalLine.y1();
         y <= defendGoalLine.y1() + 3*(defendGoalLine.y2() - defendGoalLine.y1())/7;
         y += (defendGoalLine.y2() - defendGoalLine.y1())/7) {
        curPoint = QPointF(defendLineX, y);
        curValue = getGoalCoverageRatingFromPlayerPos(curPoint);
        if (curValue > maxValue) {
            maxPoint = curPoint;
            maxValue = curValue;
        }
    }
    QPointF upperPoint;
    if (maxValue != 0)
        upperPoint = maxPoint;
    else if (worldModel->getBall()->getCoordinatesPoint().y() <= defendGoalLine.y1())
        upperPoint = QPointF(defendLineX, defendGoalLine.y1());
    else
        upperPoint = QPointF(defendLineX, defendGoalLine.y1() + 3*(defendGoalLine.y2() - defendGoalLine.y1())/7);

    maxValue = -1;
    curValue = -1;
    // lower part
    for (double y = defendGoalLine.y1() + 4*(defendGoalLine.y2() - defendGoalLine.y1())/7;
         y <= defendGoalLine.y2();
         y += (defendGoalLine.y2() - defendGoalLine.y1())/7) {
        curPoint = QPointF(defendLineX, y);
        curValue = getGoalCoverageRatingFromPlayerPos(curPoint);
        if (curValue > maxValue) {
            maxPoint = curPoint;
            maxValue = curValue;
        }
    }
    QPointF lowerPoint;
    if (maxValue != 0)
        lowerPoint = maxPoint;
    else if (worldModel->getBall()->getCoordinatesPoint().y() <= defendGoalLine.y1() + 4*(defendGoalLine.y2() - defendGoalLine.y1())/7)
        lowerPoint = QPointF(defendLineX, defendGoalLine.y1() + 4*(defendGoalLine.y2() - defendGoalLine.y1())/7);
    else
        lowerPoint = QPointF(defendLineX, defendGoalLine.y2());
    // At this point we have two points, one in each half,
    // which brings optimal coverage

    return std::make_tuple(upperPoint, lowerPoint);
}

double PlayerAI::getGoalCoverageRatingFromPlayerPos(QPointF playerPos) {
    // get tangent points from ball position to circle with center in
    // supposed player position and with radius of kickable area
    std::tuple<QPointF, QPointF> pointsTuple = getTangentPointsFromLineAndCircle(
                worldModel->getBall()->getCoordinatesPoint(),
                playerPos,
                r_KICKABLE_AREA);
    // get intersections of left pitch rim with lines formed by
    // ball position and points got from previous action
    QPointF intersectingLeftRim1 = intersectTwoLines(
                leftPitchRimTopPoint,
                leftPitchRimBotPoint,
                std::get<0>(pointsTuple),
                worldModel->getBall()->getCoordinatesPoint());
    QPointF intersectingLeftRim2 = intersectTwoLines(
                leftPitchRimTopPoint,
                leftPitchRimBotPoint,
                std::get<1>(pointsTuple),
                worldModel->getBall()->getCoordinatesPoint());
    // determine which one of points got from previous action has
    // lesser y coordinate and calculate fit rating
    double fitRating;
    if (intersectingLeftRim1.y() < intersectingLeftRim2.y())
        fitRating = howGoodTwoLinesFit(
                    allyGoalTopPoint,
                    allyGoalBotPoint,
                    intersectingLeftRim1,
                    intersectingLeftRim2);
    else
        fitRating = howGoodTwoLinesFit(
                    allyGoalTopPoint,
                    allyGoalBotPoint,
                    intersectingLeftRim2,
                    intersectingLeftRim1);
    return fitRating;
}

QPointF PlayerAI::getPointWait(Action a) {
    QPointF rolePoint = getPlayerRole().getRolePoint();
    return rolePoint;
}

double PlayerAI::calcRoleMarginFromGoal() {
    double margin = worldModel->getBall()->getCoordinatesPoint().x() - r_PITCH_LENGTH/2;
    if (margin < 0)
        margin = 0;
    return margin;
}

//std::tuple<QLineF, QLineF> getTangentLines {
//}
