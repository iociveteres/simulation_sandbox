#include "PlayerAI.h"

const double MarkingDist = 4;
QPointF allyGoalTopPoint(PITCH_MARGIN, TOTAL_FIELD_WIDTH/2 - GOAL_WIDTH/2);
QPointF allyGoalBotPoint(PITCH_MARGIN, TOTAL_FIELD_WIDTH/2 - GOAL_WIDTH/2 + GOAL_WIDTH);
const QPointF leftPitchRimTopPoint(PITCH_MARGIN, PITCH_MARGIN);
const QPointF leftPitchRimBotPoint(PITCH_MARGIN, PITCH_MARGIN + PITCH_WIDTH);
const QRectF defendGoalArea(0,
                              PITCH_WIDTH/3,
                              PENALTY_AREA_LENGTH + 0.5,
                              PITCH_WIDTH/3);
QLineF defendGoalLine(GOAL_AREA_LENGTH + 2.5,
                              PITCH_MARGIN + PITCH_WIDTH/3,
                              PITCH_MARGIN + GOAL_AREA_LENGTH + 2.5,
                              PITCH_MARGIN + 2*PITCH_WIDTH/3);

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

void PlayerAI::cycle()
{
    worldModel->update();
    intention = getIntention();
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
    return QRectF(PITCH_MARGIN + _x - KICKABLE_AREA/2,
                  PITCH_MARGIN + _y - KICKABLE_AREA/2,
                 KICKABLE_AREA, KICKABLE_AREA);
}

// calculate level of desirability for possible actions
// for every known ally against every known enemy
// return list, containing list of Actions for every player;
QList<Action> PlayerAI::makePrefferedActionsListOthers()
{
    QList<Action> actionsOfPlayers;
    for (Player p: worldModel->getTeamAlly()) {
        QList<Action> actionsOfPlayer;

        for (Player e: worldModel->getTeamEnemy()) {
            Action a = checkMarking(e);
            a.setPrefferedPoint(getPointMarking(a));
            actionsOfPlayer.append(a);
        }
        {
            Action a = checkWaitDefensive();
            a.setPrefferedPoint(getPointWait());
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
        // set cost
        for (Action a: actionsOfPlayer) {
            a.setCost(distance(a.getPrefferedPoint(),
                               worldModel->
                               getAllyById(a.getExecutorId()).
                               getCoordinatesPoint()));
        }

        actionsOfPlayers.append(actionsOfPlayer);
    }

    return actionsOfPlayers;
}

QList<Action> PlayerAI::makePrefferedActionsListMe() {
    QList<Action> actionsOfPlayer;

    for (Player e: worldModel->getTeamEnemy()) {
        Action a = checkMarking(e);
        a.setPrefferedPoint(getPointMarking(a));
        actionsOfPlayer.append(a);
    }
    {
        Action a = checkWaitDefensive();
        a.setPrefferedPoint(getPointWait());
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
    // set cost
    for (Action a: actionsOfPlayer) {
        a.setCost(distance(a.getPrefferedPoint(),
                           this->
                           getCoordinatesPoint()));
    }

    return actionsOfPlayer;
}

QList<Action> PlayerAI::makePrefferedActionsListNeighbours() {
    QList<Action> actionsOfPlayer;
    PlayerRole normalPlayerRole = this->getPlayerRole();
    for (PlayerRole::RoleName roleName:
         this->getPlayerRole().getNeighbourRoles()) {
        if (checkAllyIsInPlace(roleName)) {

        }
        this->setPlayerRole(
                    this->worldModel->getPlayerRoleByRoleName(roleName));
        // проверять имеет смысл только те действия,
        // которые зависят от роли игрока
        for (Player e: worldModel->getTeamEnemy()) {
            Action a = checkMarking(e);
            a.setPrefferedPoint(getPointMarking(a));
            actionsOfPlayer.append(a);
        }
    }
    // decrease desirebility of actions for other roles and set cost
    for (Action a: actionsOfPlayer) {
        a.setDesirebility(a.getDesirebility() - 7);
        a.setCost(distance(a.getPrefferedPoint(),
                           this->
                           getCoordinatesPoint()));
    }
    this->setPlayerRole(normalPlayerRole);

    return actionsOfPlayer;
}

struct Action::desirebilityDescending
{
    inline bool operator() (const Action& a, const Action& b)
    {
        return (a.desirebility > b.desirebility);
    }
};


struct Action::costAscending
{
    inline bool operator() (const Action& a, const Action& b)
    {
        return (a.cost < b.cost);
    }
};

// base cycle
// let every ally determine what action should he execute,
// based on desirability of action
// return list, containing list of Actions for every player;
Action PlayerAI::determinePrefferedIntention()
{
    QList<Action> actionsOfOthers = makePrefferedActionsListOthers();
    QList<Action> myActions = makePrefferedActionsListMe();
    myActions.append(makePrefferedActionsListNeighbours());

    std::sort(myActions.begin(), myActions.end(), Action::desirebilityDescending());
    std::sort(myActions.begin(), myActions.end(), Action::desirebilityDescending());

    Action prefferedAction;
    for (Action my: myActions) {
        bool cheapest = true;
        for (Action others: actionsOfOthers) {
            if (others.getCost() < my.getCost()) {
                cheapest = false;
                break;
            }
        }
        if (cheapest == true) {
            prefferedAction = my;
            break;
        }
    }
    return prefferedAction;
}



Action PlayerAI::checkMarking(Player enemy)
{
    const double a = 1;
    const double b = 1;
    const double c = 1;
    double distBtwEnemyAndBall =
            distance(enemy.getCoordinatesPoint(),
                     worldModel->getBall()->getCoordinatesPoint());
    distBtwEnemyAndBall = limit(distBtwEnemyAndBall, 0, 30);
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
            distance(QPointF(PITCH_MARGIN, TOTAL_FIELD_WIDTH/2),
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
    int howManyDefend = worldModel->getHowManyPlayersAreInArea(findDefendGoalArea());
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

Action PlayerAI::checkIntercept()
{
    const double a = 1;
    const double b = 1;
    const double c = 1;

    std::tuple<bool, QPointF> t = getPointIntercept();
    bool canIreachBall = std::get<0>(t);
    QPointF pointToReachBall = std::get<1>(t);

    double iCanReachBall = 0;
    if (canIreachBall)
        iCanReachBall = 100;

    double iDontKnowMyEnemies = (worldModel->getTeamEnemy().length() - worldModel->getEnemyCount())*6;
    ;

    double desirebility =
            a * iCanReachBall +
            b * iDontKnowMyEnemies;


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

QRectF PlayerAI::findDefendGoalArea()
{
    if (worldModel->getBall() != nullptr) {
        double ballPos_x = worldModel->getBall()->getCoordinatesPoint().x();
        double defendGoalAreaMargin = 0;
        if (ballPos_x > (PENALTY_AREA_LENGTH - 4))
            defendGoalAreaMargin = ballPos_x - 1;
        else if (ballPos_x < 1)
            defendGoalAreaMargin = 0;

        return QRectF(defendGoalAreaMargin,
                      PITCH_WIDTH/3,
                      4,
                      PITCH_WIDTH/3);
    } else
        return QRectF(PENALTY_AREA_LENGTH - 4,
                    PITCH_WIDTH/3,
                    4,
                    PITCH_WIDTH/3);
}


// search for best point in each part
std::tuple<QPointF, QPointF> PlayerAI::getPointsDefendGoal(Action a) {
    QRectF curDefendGoalArea = findDefendGoalArea();
    QLineF defendGoalLine = QLineF(curDefendGoalArea.left() + 2,
                                   PITCH_WIDTH/3,
                                   curDefendGoalArea.left() + 2,
                                   PITCH_WIDTH/3*2);

    double defendLineX = defendGoalLine.x1();
    QPointF curPoint;
    QPointF maxPoint;
    double curValue = -1;
    double maxValue = -1;

    // If accidentially there is 0 coverage, which can occur
    // if ball is to right or left on the field
    // we should pick point leftest or rightest points accordingly
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
                KICKABLE_AREA);
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

std::tuple<bool, QPointF> PlayerAI::getPointIntercept()
{
    int i = 0;
    bool canIReachBallEarlier = true;
    std::unique_ptr<Ball> predictedBall(new Ball(*worldModel->getBall()));
    do {
        i++;
        predictedBall->tick();
        for (Player e: worldModel->getTeamEnemy()) {
            if (distance(predictedBall->getCoordinatesPoint(), e.getCoordinatesPoint())
                    < PLAYER_SPEED_MAX * i) {
                canIReachBallEarlier = false;
                break;
            }
        }
        if (distance(predictedBall->getCoordinatesPoint(), this->getCoordinatesPoint())
                < PLAYER_SPEED_MAX * i) {
            canIReachBallEarlier = true;
            return std::make_tuple(
                        canIReachBallEarlier, predictedBall->getCoordinatesPoint());
        }
    } while (canIReachBallEarlier);

    return std::make_tuple(false, QPointF());
}

QPointF PlayerAI::getPointWait() {
    QPointF rolePoint = getPlayerRole().getRolePoint();
    return rolePoint;
}

double PlayerAI::calcRoleMarginFromGoal() {
    double margin = worldModel->getBall()->getCoordinatesPoint().x() - PITCH_LENGTH/2;
    if (margin < 0)
        margin = 0;
    return margin;
}
// Посмотреть позицию союзника и сравнить с той,
// которая должна быть по формации посмотреть
bool PlayerAI::checkAllyIsInPlace(PlayerRole::RoleName roleName)
{
    QPointF ally = worldModel->getAllyByRoleName(roleName).getCoordinatesPoint();
    QPointF hisRolePoint = worldModel->
            getAllyByRoleName(roleName).getPlayerRole().getRolePoint();
    if (distance(ally, hisRolePoint) <= 4)
        return true;
    else
        return false;

}

//std::tuple<QLineF, QLineF> getTangentLines {
//}
