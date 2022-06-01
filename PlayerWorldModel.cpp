#include "PlayerWorldModel.h"

int PlayerWorldModel::getEnemyCount() const
{
    return enemyCount;
}

PlayerWorldModel::PlayerWorldModel(const World& world, Player* _myself)
{
    myself = _myself;
    allyCount = world.getTeamAlly().length();
    teamAlly = world.makePlayerTeamAllyForWorldView(_myself->getId());
    enemyCount = world.getTeamEnemy().length();
    teamEnemy = world.getTeamEnemy();
    ball = world.getBall();
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

Player PlayerWorldModel::getEnemyClosestToTheBall()
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
        return minPlayer;
    else
        throw std::runtime_error("Empty enemy team vector");
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

Player PlayerWorldModel::getEnemyControllingBall()
{
    Player p = getEnemyClosestToTheBall();

    if (distance(p.getCoordinatesPoint(), ball->getCoordinatesPoint())
            <= KICKABLE_AREA + 1)
        return p;
    else
        throw std::runtime_error("No enemy controlling ball");
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

Player PlayerWorldModel::getAllyById(int id)
{
    for (Player a: teamAlly) {
        if (id == a.getId()) {
            return a;
        }
    }
    throw std::runtime_error("No ally with this id");
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
    throw std::runtime_error("No player with this role in formation");
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

void PlayerWorldModel::checkFormationIsInBox(QPointF shift) {
    if (myself->getDefaultRolePosition(PlayerRole::DefenderRight).y()+shift.y()+2*PITCH_WIDTH/16
            > PITCH_WIDTH)
        shift.setY(PITCH_WIDTH/8);
    if (myself->getDefaultRolePosition(PlayerRole::DefenderLeft).y()+shift.y()-2*PITCH_WIDTH/16
            < PITCH_WIDTH)
        shift.setY(-PITCH_WIDTH/8);
    if (myself->getDefaultRolePosition(PlayerRole::DefenderLeft).x()+shift.x()-2*PITCH_LENGTH/16
            < PITCH_WIDTH)
        shift.setY(-PITCH_LENGTH/10);
}

//
void PlayerWorldModel::determineFormation()
{
    QPointF fieldCenter = QPointF(PITCH_LENGTH/2, PITCH_WIDTH/2);
    if (this->ball != nullptr) {
        QPointF shift = fieldCenter - ball->getCoordinatesPoint();
        checkFormationIsInBox(shift);

        for (PlayerRole a: formation) {
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

void PlayerWorldModel::introduceNoises()
{

}

void PlayerWorldModel::limitVisionDist()
{

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
    ball = world.getBall();
    formation = _myself->getDefaultFormation();
    determineFormation();
}

