#include "PlayerAI.h"

const double MarkingDist = 4;
const QPointF allyGoalTopPoint(PITCH_MARGIN,
                         TOTAL_FIELD_WIDTH/2 - GOAL_WIDTH/2);
const QPointF allyGoalBotPoint(PITCH_MARGIN,
                         TOTAL_FIELD_WIDTH/2 - GOAL_WIDTH/2 + GOAL_WIDTH);
const QPointF leftPitchRimTopPoint(PITCH_MARGIN,
                                   PITCH_MARGIN);
const QPointF leftPitchRimBotPoint(PITCH_MARGIN,
                                   PITCH_MARGIN + PITCH_WIDTH);

double PlayerAI::getRoleMargin() const
{
    return roleMargin;
}

PlayerWorldModel *PlayerAI::getWorldModel() const
{
    return worldModel;
}

PlayerAI::PlayerAI():
    Player()
{
    worldModel = new PlayerWorldModel();
}

PlayerAI::PlayerAI(Team _team, World* _world):
    Player(_team)
{
    worldLink = _world;
    worldModel = new PlayerWorldModel();
}

PlayerAI::PlayerAI(Team _team, int _x, int _y,
                   double _angle, World* _world):
    Player(_team, _x, _y, _angle)
{
    worldLink = _world;
    worldModel = new PlayerWorldModel();
}

PlayerAI::~PlayerAI()
{

}

void PlayerAI::cycle()
{
    worldModel->update(*worldLink, this);
    intention = determinePrefferedIntention();
    qDebug() << "Player " << this->id
             << " finished cycle succesfully\n";
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
QMap<int, QList<Action>> PlayerAI::makePrefferedActionsListOthers()
{
    QMap<int, QList<Action>> actionsOfPlayers;
    for (Player& p: worldModel->getTeamAlly()) {
        QList<Action> actionsOfPlayer;

        for (Player& e: worldModel->getTeamEnemy()) {
            Action a = checkMarking(e, p);
            a.setPrefferedPoint(getPointMarking(a, p));
            actionsOfPlayer.append(a);
        }
        {
            Action a = checkWaitDefensive(p);
            a.setPrefferedPoint(getPointWait(p));
            actionsOfPlayer.append(a);
        }
        {
            Action a = checkIntercept(p);
            actionsOfPlayer.append(a);
        }
        {
            Action a = checkDefendGoal(p);
            Action b = a;
            b.setDefendGoalPart(1);
            std::tuple<QPointF, QPointF> t = getPointsDefendGoal(a, p);
            a.setPrefferedPoint(std::get<0>(t));
            b.setPrefferedPoint(std::get<1>(t));
            actionsOfPlayer.append(a);
            actionsOfPlayer.append(b);
        }
        actionsOfPlayers[p.getId()] = actionsOfPlayer;
    }
    // set cost
    for (QList<Action>& list: actionsOfPlayers)
        for (Action& a: list) {
            std::tuple<bool, Player> t = worldModel->
                    getAllyById(a.getExecutorId());
            Player p = std::get<1>(t);
            double d = distance(a.getPrefferedPoint(),
                                p.getCoordinatesPoint());
            a.setCost(d);
        }

    return actionsOfPlayers;
}

QList<Action> PlayerAI::makePrefferedActionsListMe() {
    QList<Action> actionsOfPlayer;

    for (Player e: worldModel->getTeamEnemy()) {
        Action a = checkMarking(e, *this);
        a.setPrefferedPoint(getPointMarking(a, *this));
        actionsOfPlayer.append(a);
    }
    {
        Action a = checkWaitDefensive(*this);
        a.setPrefferedPoint(getPointWait(*this));
        actionsOfPlayer.append(a);
    }
    {
        Action a = checkIntercept(*this);
        actionsOfPlayer.append(a);
    }
    {
        Action a = checkDefendGoal(*this);
        Action b = a;
        std::tuple<QPointF, QPointF> t = getPointsDefendGoal(a, *this);
        a.setPrefferedPoint(std::get<0>(t));
        b.setPrefferedPoint(std::get<1>(t));
        actionsOfPlayer.append(a);
        actionsOfPlayer.append(b);
    }
    // set cost
    for (Action& a: actionsOfPlayer) {
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
        if (checkAllyIsInPlace(roleName))
            continue;
        this->setPlayerRole(
                    this->worldModel->
                    getPlayerRoleByRoleName(roleName));
        // проверять имеет смысл только те действия,
        // которые зависят от роли игрока
        for (Player e: worldModel->getTeamEnemy()) {
            Action a = checkMarking(e, *this);
            a.setPrefferedPoint(getPointMarking(a, *this));
            actionsOfPlayer.append(a);
        }
    }
    // decrease desirebility of actions for other roles and set cost
    for (Action& a: actionsOfPlayer) {
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

bool isEqual(Action a, Action b) {
    if (a.getActionType() != b.getActionType())
        return false;
    if (a.getAgainstId() != b.getAgainstId())
        return false;
    if (a.getCost() != b.getCost())
        return false;
    if (a.getDesirebility() != b.getDesirebility())
        return false;
    if (a.getExecutorId() != b.getExecutorId())
        return false;
    if (a.getPrefferedPoint() != b.getPrefferedPoint())
        return false;
    return true;
}

// base cycle
// let every ally determine what action should he execute,
// based on desirability of action
// return list, containing list of Actions for every player;
Action PlayerAI::determinePrefferedIntention()
{
    QMap<int, QList<Action>> actionsOfOthers =
            makePrefferedActionsListOthers();
    QList<Action> myActions = makePrefferedActionsListMe();
    myActions.append(makePrefferedActionsListNeighbours());

    std::sort(myActions.begin(), myActions.end(),
              Action::desirebilityDescending());
    for (QList<Action>& list: actionsOfOthers)
        std::sort(list.begin(), list.end(),
                  Action::desirebilityDescending());

    QMap<int, QList<Action>> allActions = actionsOfOthers;
    allActions[this->getId()] = myActions;
    QList<Action> prefferedActions;
    bool choosedDif = true;
    do {
        choosedDif = true;
        prefferedActions.clear();
        for (QList<Action> a: allActions) {
            prefferedActions.append(a.front());
        }
        for (Action& a: prefferedActions) {
            for (Action& b: prefferedActions) {
                if (&a == &b)
                    break;
                if (a == b) {
                    int id;
                    if (a.getCost() < b.getCost())
                        id = b.getExecutorId();
                    else
                        id = a.getExecutorId();
                    allActions[id].pop_front();
                    choosedDif = false;
                }
            }
        }
    } while (!choosedDif);

    Action prefferedAction;
    for (Action& a: prefferedActions) {
        if (a.getExecutorId() == this->getId()) {
            //return a;
            prefferedAction = a;
            break;
        }
    }
//    // Номера игроков, действия для которых уже определены
//    QList<int> skipOthersIds;
//    for (Action my: myActions) {
//        bool cheapest = true;
//        for (Action others: actionsOfOthers) {
//            // если ранее не было предположено действие
//            // для этого игрока
//            if (skipOthersIds.indexOf(others.getExecutorId()) == -1) {
//                // если они выполняют разные действия
//                if (my == others)
//                    // и если другой игрок выполняет
//                    // действие дешевле
//                    if (others.getCost() < my.getCost()) {
//                        cheapest = false;
//                        skipOthersIds.append(others.getExecutorId());
//                        break;
//                    }
//            }
//            else
//                break;
//        }
//        if (cheapest == true) {
//            prefferedAction = my;
//            break;
//        }
//    }
    return prefferedAction;
}

Action PlayerAI::checkMarking(Player enemy, Player player)
{
    double distBtwEnemyAndBall =
            distance(enemy.getCoordinatesPoint(),
                     worldModel->getBall()->getCoordinatesPoint());
    distBtwEnemyAndBall = limit(distBtwEnemyAndBall, 1, 100);
    distBtwEnemyAndBall = log(distBtwEnemyAndBall);

    double distBtwEnemyAndMe =
            distance(enemy.getCoordinatesPoint(),
                     player.getCoordinatesPoint());
    distBtwEnemyAndMe = limit(distBtwEnemyAndMe, 1, 100);
    distBtwEnemyAndMe = log(distBtwEnemyAndMe);

    double distBtwEnemyAndDefensiveLine =
            enemy.getX() - getRoleMargin();
    distBtwEnemyAndDefensiveLine =
            limit(distBtwEnemyAndDefensiveLine, 1, 40);
    distBtwEnemyAndDefensiveLine =
            log(distBtwEnemyAndDefensiveLine);

    double enemyInMyZone = 0;
    int id = player.getId();
    QRectF roleRect = this->worldModel->
            getPlayerRoleByAllyId(id).getRoleRect();
    if (roleRect.contains(enemy.getCoordinatesPoint()))
        enemyInMyZone = 40;

    const double a = 0;
    const double b = 1;
    const double c = 1;
    const double d = 1;

    double desirebility =
            a * 5/distBtwEnemyAndBall +
            b * 5/distBtwEnemyAndMe +
            c * 20/distBtwEnemyAndDefensiveLine +
            d * enemyInMyZone;

    desirebility = limit(desirebility, 0, 100);
    double cost = 0;

    return Action(Action::Mark, player.getId(),
                  enemy.getId(), desirebility, cost);
}

Action PlayerAI::checkDefendGoal(Player player)
{
    double distBtwGoalAndBall =
            distance(QPointF(PITCH_MARGIN, TOTAL_FIELD_WIDTH/2),
                     worldModel->getBall()->getCoordinatesPoint());
    bool danger = (distBtwGoalAndBall < 60)
            || (worldModel->getBall()->getCoordinatesPoint().x() < 52);
    if (danger) {
        distBtwGoalAndBall = limit(distBtwGoalAndBall, 1, 100);
        distBtwGoalAndBall = log(distBtwGoalAndBall);
    } else
        distBtwGoalAndBall = 10000;
    double ballIsControlledByEnemy;
    std::tuple<bool, Player> t = worldModel->
            getEnemyControllingBall();
    if (std::get<0>(t))
        ballIsControlledByEnemy = 5;
    else
        ballIsControlledByEnemy = 0;

    double lessThan2AlliesDefendGoal;
    int howManyDefend = worldModel->
            getHowManyPlayersAreInArea(findDefendGoalArea());
    if (howManyDefend < 2) {
        lessThan2AlliesDefendGoal = 10;
    } else {
        lessThan2AlliesDefendGoal = 0;
    }

    const double a = 1;
    const double b = 1;
    const double c = 1;

    double desirebility =
            a * 100/distBtwGoalAndBall +
            b * ballIsControlledByEnemy +
            c * lessThan2AlliesDefendGoal;
    desirebility = limit(desirebility, 0, 100);
    double cost = 0;

    return Action(Action::DefendGoal, player.getId(),
                  Action::NoEnemyCode, desirebility, cost);
}

Action PlayerAI::checkIntercept(Player player)
{
    std::tuple<bool, QPointF> t = getPointIntercept(player);
    bool canIreachBall = std::get<0>(t);
    QPointF pointToReachBall = std::get<1>(t);

    double iCanReachBall = 0;
    if (canIreachBall)
        iCanReachBall = 100;

    double iDontKnowMyEnemies = (worldModel->getTeamEnemy().length()
                                 - worldModel->getEnemyCount())*6;

    const double a = 1;
    const double b = 1;

    double desirebility =
            a * iCanReachBall +
            b * iDontKnowMyEnemies;
    desirebility = limit(desirebility, 0, 100);
    double cost = 0;

    return Action(Action::Intercept, player.getId(),
                  Action::NoEnemyCode, desirebility,
                  pointToReachBall, cost);
}

Action PlayerAI::checkWaitDefensive(Player player)
{
    double desirebility = 30;
    double cost = 0;
    return Action(Action::Wait, player.getId(),
                  Action::NoEnemyCode, desirebility, cost);
}


QPointF PlayerAI::getPointMarking(Action a, Player player) {
//    QPointF executorPos = worldModel->
//            getAllyById(a.getExecutorId()).
//            getCoordinatesPoint();
    QPointF ballPos = worldModel->
            getBall()->
            getCoordinatesPoint();
    Player e = worldModel->
            getEnemyById(a.getAgainstId());
    int i = ceil(distance(player.getCoordinatesPoint(),
                          e.getCoordinatesPoint())
                 /PLAYER_SPEED_MAX);
    QPointF againstPos = e.getCoordinatesPoint()
            + e.getPVelocity()*i;
    return findPointInDistFromEndOfSegment(againstPos,
                                           ballPos, MarkingDist);
}

QRectF PlayerAI::findDefendGoalArea()
{
    if (worldModel->getBall() != nullptr) {
        double ballPos_x =
                worldModel->getBall()->getCoordinatesPoint().x();
        double defendGoalAreaMargin = PENALTY_AREA_LENGTH - 4;
        if (ballPos_x < (PENALTY_AREA_LENGTH - 4))
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
std::tuple<QPointF, QPointF>
PlayerAI::getPointsDefendGoal(Action a, Player player) {
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
         y <= defendGoalLine.y1()
         + 3*(defendGoalLine.y2() - defendGoalLine.y1())/7;
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
    else if (worldModel->getBall()->getCoordinatesPoint().y()
             <= defendGoalLine.y1())
        upperPoint = QPointF(defendLineX,
                             defendGoalLine.y1());
    else
        upperPoint = QPointF(defendLineX,
                             defendGoalLine.y1()
                             + 3*(defendGoalLine.y2()
                                  - defendGoalLine.y1())/7);

    maxValue = -1;
    curValue = -1;
    // lower part
    for (double y = defendGoalLine.y1()
         + 4*(defendGoalLine.y2() - defendGoalLine.y1())/7;
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
    else if (worldModel->getBall()->getCoordinatesPoint().y()
             <= defendGoalLine.y1()
             + 4*(defendGoalLine.y2() - defendGoalLine.y1())/7)
        lowerPoint = QPointF(defendLineX, defendGoalLine.y1()
                             + 4*(defendGoalLine.y2()
                                  - defendGoalLine.y1())/7);
    else
        lowerPoint = QPointF(defendLineX, defendGoalLine.y2());
    // At this point we have two points, one in each half,
    // which brings optimal coverage

    return std::make_tuple(upperPoint, lowerPoint);
}

double PlayerAI::getGoalCoverageRatingFromPlayerPos(QPointF playerPos) {
    // get tangent points from ball position to circle with center in
    // supposed player position and with radius of kickable area
    std::tuple<QPointF, QPointF> pointsTuple =
            getTangentPointsFromLineAndCircle(
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

std::tuple<bool, QPointF> PlayerAI::getPointIntercept(Player player)
{
    int i = 0;
    bool canIReachBallEarlier = true;
    std::unique_ptr<Ball> predictedBall(
                new Ball(*worldModel->getBall()));
    do {
        i++;
        predictedBall->tick();
        for (Player e: worldModel->getTeamEnemy()) {
            if (distance(predictedBall->getCoordinatesPoint(),
                         e.getCoordinatesPoint())
                    < PLAYER_SPEED_MAX * i) {
                canIReachBallEarlier = false;
                break;
            }
        }
        if (distance(predictedBall->getCoordinatesPoint(),
                     player.getCoordinatesPoint())
                < PLAYER_SPEED_MAX * i) {
            canIReachBallEarlier = true;
            return std::make_tuple(
                        canIReachBallEarlier,
                        predictedBall->getCoordinatesPoint());
        }
    } while (canIReachBallEarlier);

    return std::make_tuple(false, QPointF());
}

QPointF PlayerAI::getPointWait(Player player) {
    int id = player.getId();
    QPointF rolePoint =
            this->worldModel->getPlayerRoleByAllyId(id).getRolePoint();
    return rolePoint;
}

double PlayerAI::calcRoleMarginFromGoal() {
    double margin =
            worldModel->getBall()->getCoordinatesPoint().x() - PITCH_LENGTH/2;
    if (margin < 0)
        margin = 0;
    return margin;
}
// Посмотреть позицию союзника и сравнить с той,
// которая должна быть по формации посмотреть
bool PlayerAI::checkAllyIsInPlace(PlayerRole::RoleName roleName)
{
    QPointF ally =
            worldModel->getAllyByRoleName(roleName).getCoordinatesPoint();
    QPointF hisRolePoint = worldModel->
            getAllyByRoleName(roleName).getPlayerRole().getRolePoint();
    if (distance(ally, hisRolePoint) <= 4)
        return true;
    else
        return false;
}
