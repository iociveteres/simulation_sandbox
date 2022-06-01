#include "Player.h"

int Player::getId() const
{
    return id;
}

PlayerRole Player::getPlayerRole() const
{
    return playerRole;
}

void Player::setPlayerRole(const PlayerRole &value)
{
    playerRole = value;
}

Player::Player()
{
    playerCount++;
    id = playerCount;
}

Player::Player(Team _team)
{
    team = _team;
    id = playerCount;
    playerCount++;
}

Player::Player(Team _team, double _x, double _y, double _angle):
    BaseEntity(_x, _y, _angle)
{
    team = _team;
    id = playerCount;
    playerCount++;
}

Player::~Player()
{
}

int Player::getPlayerCount()
{
    return playerCount;
}

// От центра до левого верхнего края

QRectF Player::getKickableAreaRect()
{
    return QRectF(PITCH_MARGIN + x - KICKABLE_AREA/2,
                  PITCH_MARGIN + y - KICKABLE_AREA/2,
                 KICKABLE_AREA, KICKABLE_AREA);
}



QRectF Player::getPlayerWidgetRect()
{
    return QRectF(PITCH_MARGIN + x - PLAYER_WIDGET_SIZE/2,
                  PITCH_MARGIN + y - PLAYER_WIDGET_SIZE/2,
                 KICKABLE_AREA, KICKABLE_AREA);

}

QVector<PlayerRole> Player::getDefaultFormation()
{
    return Player::defaultFormation;
}

void Player::readJSON(const QJsonObject &json)
{
    if (json.contains("team") && json["team"].isDouble())
        team = Team(json["team"].toInt());

    if (json.contains("assignedRole") && json["assignedRole"].isDouble())
        assignedRole = PlayerRole::RoleName(json["assignedRole"].toInt());

    if (json.contains("x") && json["x"].isDouble())
        x = json["x"].toDouble();

    if (json.contains("y") && json["y"].isDouble())
        y = json["y"].toDouble();

    if (json.contains("angle") && json["angle"].isDouble())
        angle = json["angle"].toDouble();
}

void Player::writeJSON(QJsonObject &json) const
{
    json["team"] = static_cast<int>(team);
    json["assignedRole"] = static_cast<int>(assignedRole);
    json["x"] = x;
    json["y"] = y;
    json["angle"] = angle;
}


Player::Team Player::getTeam() const
{
    return team;
}

QPointF Player::getDefaultRolePosition(PlayerRole::RoleName roleName) const
{
    for (PlayerRole r: getDefaultFormation()) {
        if (roleName == r.getRoleName())
            return r.getRolePoint();
    }
    // impossible since there are all possible roles in default formation
    return QPointF(0, 0);
}

QVector<PlayerRole> getRolesVec() {
    QVector<PlayerRole> roles;
    //roles.append(Goalie());
    roles.append(DefenderRight());
    roles.append(DefenderRightCentre());
    roles.append(DefenderLeftCentre());
    roles.append(DefenderLeft());
    roles.append(SemidefenderRight());
    roles.append(SemidefenderRightCentre());
    roles.append(SemidefenderLeftCentre());
    roles.append(SemidefenderLeft());
    roles.append(AttackerRight());
    roles.append(AttackerLeft());

    return roles;
}

void Player::tick()
{
    velocity.x += acceleration.x;
    velocity.y += acceleration.y;

    this->x += velocity.x;
    this->y += velocity.y;

    velocity.x *= PLAYER_DECAY;
    velocity.y *= PLAYER_DECAY;

    acceleration.x = 0;
    acceleration.y = 0;

    double velocityAbs = vectorLength(velocity.x, velocity.y);
    if (velocityAbs > PLAYER_SPEED_MAX) {
        velocity.x /= (velocityAbs / PLAYER_SPEED_MAX);
        velocity.y /= (velocityAbs / PLAYER_SPEED_MAX);
    }

    double accelerationAbs = vectorLength(velocity.x, velocity.y);
    if (accelerationAbs > PLAYER_SPEED_MAX) {
        acceleration.x /= (accelerationAbs / PLAYER_SPEED_MAX);
        acceleration.y /= (accelerationAbs / PLAYER_SPEED_MAX);
    }
}

// initialise static
const QVector<PlayerRole> Player::defaultFormation = getRolesVec();
int Player::playerCount = 0;
