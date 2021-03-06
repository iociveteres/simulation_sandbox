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
// конструкторы
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

// цикл игрока из обновления модели мира и
// определения желаемого действия
void PlayerAI::cycle()
{
    worldModel->update(*worldLink, this);
    intention = determinePrefferedIntention();
    qDebug() << "Player " << this->id
             << " finished cycle succesfully\n";
}
// получение желаемого действия
Action PlayerAI::getIntention() const
{
    return intention;
}
// получение модели мира игрока
PlayerWorldModel *PlayerAI::getWorldModel() const
{
    return worldModel;
}
// получение текущего отступа от левого края поля
// для роли игрока
double PlayerAI::getRoleMargin() const
{
    return roleMargin;
}

// установка роли
void PlayerAI::setRole(PlayerRole role)
{
    playerRole = role;
}

// получить прямоугольник в координатах окна отрисовки,
// обозначающий зону, в которой игрок может ударить мяч
QRectF PlayerAI::getIntentionsKickableAreaRect()
{
    double _x = this->intention.getPrefferedPoint().x();
    double _y = this->intention.getPrefferedPoint().y();
    return QRectF(PITCH_MARGIN + _x - KICKABLE_AREA/2,
                  PITCH_MARGIN + _y - KICKABLE_AREA/2,
                 KICKABLE_AREA, KICKABLE_AREA);
}

// создание списков действий, доступных для союзников
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
            std::tuple<QPointF, QPointF> t = getPointsDefendGoal(p);
            a.setPrefferedPoint(std::get<0>(t));
            b.setPrefferedPoint(std::get<1>(t));
            actionsOfPlayer.append(a);
            actionsOfPlayer.append(b);
        }
        actionsOfPlayers[p.getId()] = actionsOfPlayer;
    }
    // вычисление стоимости
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

// создание списка действий игрока
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
        std::tuple<QPointF, QPointF> t = getPointsDefendGoal(*this);
        a.setPrefferedPoint(std::get<0>(t));
        b.setPrefferedPoint(std::get<1>(t));
        actionsOfPlayer.append(a);
        actionsOfPlayer.append(b);
    }
    // вычисление стоимости действий
    for (Action& a: actionsOfPlayer) {
        a.setCost(distance(a.getPrefferedPoint(),
                           this->
                           getCoordinatesPoint()));
    }

    return actionsOfPlayer;
}

// создание списка действий игркоа с точки зрения игроков других ролей,
// на данный момент отсутствующих на своей позиции
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
    // вычисление стоимости действий
    for (Action& a: actionsOfPlayer) {
        a.setDesirebility(a.getDesirebility() - 7);
        a.setCost(distance(a.getPrefferedPoint(),
                           this->
                           getCoordinatesPoint()));
    }
    this->setPlayerRole(normalPlayerRole);

    return actionsOfPlayer;
}

// сортировка по желаемости по возрастанию
struct Action::desirebilityDescending
{
    inline bool operator() (const Action& a, const Action& b)
    {
        return (a.desirebility > b.desirebility);
    }
};

// сортировка по стоимости по возрастанию
struct Action::costAscending
{
    inline bool operator() (const Action& a, const Action& b)
    {
        return (a.cost < b.cost);
    }
};

// Определение действий, возможных для других игроков,
// а также выбор действия игрока с учётом этого
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
    // объединить все действия игроков
    QMap<int, QList<Action>> allActions = actionsOfOthers;
    allActions[this->getId()] = myActions;

    QList<Action> prefferedActions;
    bool choosedDif = true;
    // пока все игроки не выбрали разное действие
    do {
        // предположить, что игрокам удастся выбрать разные действия
        choosedDif = true;
        prefferedActions.clear();
        for (QList<Action> a: allActions) {
            prefferedActions.append(a.front());
        }
        // попарно сравнить действия
        for (Action& a: prefferedActions) {
            for (Action& b: prefferedActions) {
                if (&a == &b)
                    break;
                // если действия одинаковы убрать более дорогое
                // из списка доступных действий для игрока,
                // совершающего его
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
    // среди предпочитаемых действий игроков найти текущего игрока
    Action prefferedAction;
    for (Action& a: prefferedActions) {
        if (a.getExecutorId() == this->getId()) {
            prefferedAction = a;
            break;
        }
    }

    return prefferedAction;
}
// вычисление эвристики и создание действия опеки противника
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
// вычисление эвристики и создание действия защиты ворот
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
// вычисление эвристики и создание действия перехвата мяча
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
// вычисление эвристики и создание действия ожидания
Action PlayerAI::checkWaitDefensive(Player player)
{
    double desirebility = 30;
    double cost = 0;
    return Action(Action::Wait, player.getId(),
                  Action::NoEnemyCode, desirebility, cost);
}
// определение точки для опеки противника
QPointF PlayerAI::getPointMarking(Action a, Player player) {
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

// поиск оптимальных точек для защиты ворот
std::tuple<QPointF, QPointF>
PlayerAI::getPointsDefendGoal(Player player) {
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

    // Если все вычисленные точки обеспечивают только нулевое закрытие ворот
    // что возможно, если мяч находится слишком слева или справа
    // необходимо взять самую левую или правую точку
    // в каждой зоне соответственно
    // левая часть
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
    // правая часть
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

    return std::make_tuple(upperPoint, lowerPoint);
}
// вычисление уровня закрытия ворот для отдельной точки
double PlayerAI::getGoalCoverageRatingFromPlayerPos(QPointF playerPos) {
    // поиск касательных от позиции мяча к зоне удара мяча игроком
    // в рассматриваемой точке
    std::tuple<QPointF, QPointF> pointsTuple =
            getTangentPointsFromLineAndCircle(
                worldModel->getBall()->getCoordinatesPoint(),
                playerPos,
                KICKABLE_AREA);
    // поиск пересечения полученных касательных с краем поля,
    // на котором находятся ворота
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
    // определение, какая из точек пересечения находится выше
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
// определение зоны для защиты ворот относительно положения мяча
QRectF PlayerAI::findDefendGoalArea()
{
    if (worldModel->getBall() != nullptr) {
        double ballPos_x =
                worldModel->getBall()->getCoordinatesPoint().x();
        double defendGoalAreaMargin = PENALTY_AREA_LENGTH - 2;
        if (ballPos_x < (PENALTY_AREA_LENGTH - 2))
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

// определение возможности и поиск оптимальной точки для перехвата мяча
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
// получене точки для ожидания
QPointF PlayerAI::getPointWait(Player player) {
    int id = player.getId();
    QPointF rolePoint =
            this->worldModel->getPlayerRoleByAllyId(id).getRolePoint();
    return rolePoint;
}

// сравнение позиции союзника с той, которая должна быть по построению
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
