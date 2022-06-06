#include "PlayerWorldModel.h"

int PlayerWorldModel::getEnemyCount() const
{
    return enemyCount;
}

QVector<PlayerRole> PlayerWorldModel::getFormation() const
{
    return formation;
}

PlayerWorldModel::PlayerWorldModel()
{
}

PlayerWorldModel::PlayerWorldModel(const World& world, Player* _myself)
{
    myself = _myself;
    allyCount = world.getTeamAlly().length();
    teamAlly = world.makePlayerTeamAllyForWorldView(_myself->getId());
    enemyCount = world.getTeamEnemy().length();
    teamEnemy = world.getTeamEnemy();
    ball = new Ball(*world.getBall());
    formation = _myself->getDefaultFormation();
    determineFormation();
}

QVector<Player> PlayerWorldModel::getTeamAlly() const
{
    return teamAlly;
}

QVector<Player> PlayerWorldModel::getTeamEnemy() const
{
    return teamEnemy;
}

Ball *PlayerWorldModel::getBall() const
{
    return ball;
}

std::tuple<bool, Player> PlayerWorldModel::getEnemyClosestToTheBall()
{
    double minVal = 10000;
    Player minPlayer;
    for (Player e: getTeamEnemy()) {
        double curVal = distance(e.getCoordinatesPoint(), ball->getCoordinatesPoint());
        if (curVal < minVal) {
            minVal = curVal;
            minPlayer = e;
        }
    }

    if (minVal != 10000)
        return std::make_tuple(true, minPlayer);
    else
        return std::make_tuple(false, minPlayer);
}

Player PlayerWorldModel::getAllyClosestToThePoint(QPointF p)
{
    double minVal = 10000;
    Player minPlayer;
    for (Player a: getTeamAlly()) {
        double curVal = distance(a.getCoordinatesPoint(), p);
        if (curVal < minVal) {
            minVal = curVal;
            minPlayer = a;
        }
    }

    if (minVal != 10000)
        return minPlayer;
    else
        throw std::runtime_error("Empty ally team vector");

}

std::tuple<bool, Player> PlayerWorldModel::getEnemyControllingBall()
{
    std::tuple<bool, Player> t = getEnemyClosestToTheBall();
    Player p = std::get<1>(t);

    if (distance(p.getCoordinatesPoint(), ball->getCoordinatesPoint())
            <= KICKABLE_AREA + 1)
        return std::make_tuple(true, p);
    else
        return std::make_tuple(false, p);
}

Player PlayerWorldModel::getEnemyById(int id)
{
    for (Player e: teamEnemy) {
        if (id == e.getId()) {
            return e;
        }
    }
    return Player();
}

std::tuple<bool, Player> PlayerWorldModel::getAllyById(int id)
{
    for (Player a: teamAlly) {
        if (id == a.getId()) {
            return std::make_tuple(true, a);
        }
    }
    return std::make_tuple(false, Player());
    //throw std::runtime_error("No ally with this id");
}

Player PlayerWorldModel::getAllyByRoleName(PlayerRole::RoleName roleName)
{
    for (Player a: teamAlly) {
        if (roleName == a.getPlayerRole().getRoleName()) {
            return a;
        }
    }
    throw std::runtime_error("No ally with this role");
}

PlayerRole PlayerWorldModel::getPlayerRoleByRoleName(PlayerRole::RoleName roleName)
{
    for (PlayerRole r: formation) {
        if (roleName == r.getRoleName())
            return r;
    }
    return PlayerRole();
    //throw std::runtime_error("No player with this role in formation");
}

PlayerRole PlayerWorldModel::getPlayerRoleByAllyId(int id)
{
    return getPlayerRoleByRoleName(this->myself->player_roleMap[id]);
}

int PlayerWorldModel::getHowManyPlayersAreInArea(QRectF area)
{
    int count = 0;


    for (Player a: getTeamAlly()) {
        if (area.contains(a.getCoordinatesPoint()))
            count++;
    }

    return count;
}

void PlayerWorldModel::checkFormationIsInBox(QPointF &shift) {
    double a = myself->getDefaultRolePosition(PlayerRole::DefenderRight).y()+shift.y()+2*PITCH_WIDTH/16;
    double b = myself->getDefaultRolePosition(PlayerRole::DefenderLeft).y()+shift.y()-2*PITCH_WIDTH/16;
    double c = myself->getDefaultRolePosition(PlayerRole::DefenderLeft).x()+shift.x()-2*PITCH_LENGTH/16;
    if (myself->getDefaultRolePosition(PlayerRole::DefenderRight).y()+shift.y()-2*PITCH_WIDTH/16
            > PITCH_WIDTH)
        shift.setY(PITCH_WIDTH/8);
    if (myself->getDefaultRolePosition(PlayerRole::DefenderLeft).y()+shift.y()+2*PITCH_WIDTH/16
            < 0)
        shift.setY(-PITCH_WIDTH/8);
    if (myself->getDefaultRolePosition(PlayerRole::DefenderLeft).x()+shift.x()-2*PITCH_LENGTH/16
            < 0)
        shift.setX(-PITCH_LENGTH/10);
}

//
void PlayerWorldModel::determineFormation()
{
    QPointF fieldCenter = QPointF(PITCH_LENGTH/2, PITCH_WIDTH/2);
    if (this->ball != nullptr) {
        QPointF shift = ball->getCoordinatesPoint() - fieldCenter;
        checkFormationIsInBox(shift);

        for (PlayerRole& a: formation) {
            a.setFormationMargin_x(shift.x());
            a.setFormationMargin_y(shift.y());
        }
    } else if (teamAlly.isEmpty() == false) {
        // Проверить видно ли игроков соседней роли
        // Найти относительное порожение относительно них как?
        // Взять объекты PlayerRole по умолчанию, получить точки и вычесть одну из другой
        QList<PlayerRole::RoleName> neighbours =
                myself->getPlayerRole().getNeighbourRoles();
        QPointF avg = QPointF(0, 0);
        int avgCount = 0;
        QPointF defMyRolePos = myself->
                getDefaultRolePosition(myself->getPlayerRole().getRoleName());

        for (PlayerRole::RoleName roleName: neighbours) {
            try {
                QPointF curAllyPoint = getAllyByRoleName(roleName).getCoordinatesPoint();
                QPointF defAllyRolePoint = myself->getDefaultRolePosition(roleName);
                QPointF myRolePosRelativelyToAlly =
                        curAllyPoint - defAllyRolePoint + defMyRolePos;
                avg += myRolePosRelativelyToAlly;
                avgCount++;
            } catch (std::runtime_error const &e) {}
            avg /= avgCount;
        }
        checkFormationIsInBox(avg);
        for (PlayerRole a: formation) {
            a.setFormationMargin_x(avg.x());
            a.setFormationMargin_y(avg.y());
        }
    } else {
       formation = Player::getDefaultFormation();
    }
}

static bool firstTime = true;
double rand(double fMin, double fMax)
{
    if (bDefaultRandomSeed)
        srand (1);
    else
        if (firstTime) {
            srand (time(NULL));
            firstTime = false;
        }
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

void PlayerWorldModel::introduceNoises()
{
    for (Player& p: teamAlly) {
        QPointF pos = p.getCoordinatesPoint();
        double strength = log(distance(pos, myself->getCoordinatesPoint())/125)*1.3;
        double x = pos.x() + rand(-0.4, 0.4) * strength;
        double y = pos.y() + rand(-0.4, 0.4) * strength;
        p.setX(x);
        p.setY(y);
    }
    for (Player& p: teamEnemy) {
        QPointF pos = p.getCoordinatesPoint();
        double strength = log(distance(pos, myself->getCoordinatesPoint())/125);
        double x = pos.x() + rand(-0.4, 0.4) * strength;
        double y = pos.y() + rand(-0.4, 0.4) * strength;
        p.setX(x);
        p.setY(y);
    }
    if (ball) {
        QPointF pos = ball->getCoordinatesPoint();
        double strength = log(distance(pos, myself->getCoordinatesPoint())/125);
        double x = pos.x() + rand(-0.4, 0.4) * strength;
        double y = pos.y() + rand(-0.4, 0.4) * strength;
        ball->setX(x);
        ball->setY(y);
    }
}

void PlayerWorldModel::limitVisionDist()
{
    for (int i = 0; i < teamAlly.length(); i++) {
        Player p = teamAlly[i];
        double d = distance(p.getCoordinatesPoint(), myself->getCoordinatesPoint());
        if (d > 70) {
            teamAlly.remove(i);
            i--;
        }
    }
    for (int i = 0; i < teamEnemy.length(); i++) {
        Player p = teamEnemy[i];
        double d = distance(p.getCoordinatesPoint(), myself->getCoordinatesPoint());
        if (d > 70) {
            teamEnemy.remove(i);
            i--;
        }
    }
    if (distance(ball->getCoordinatesPoint(), myself->getCoordinatesPoint()) > 70)
        ball = nullptr;
}

void PlayerWorldModel::limitVisionCone()
{

}

void PlayerWorldModel::update(const World& world, Player* _myself) {
    pastStates.append(this);
    if (pastStates.length() > 30)
        pastStates.pop_back();

    myself = _myself;
    allyCount = world.getTeamAlly().length();
    teamAlly = world.makePlayerTeamAllyForWorldView(_myself->getId());
    enemyCount = world.getTeamEnemy().length();
    teamEnemy = world.getTeamEnemy();
    ball = new Ball(*world.getBall());

    if (bIntroduceNoises)
        introduceNoises();
    if (bLimitVision)
        limitVisionDist();

    formation = _myself->getDefaultFormation();
    determineFormation();
}

