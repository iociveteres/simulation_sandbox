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

PlayerRole::RoleName Player::getAssignedRole() const
{
    return assignedRole;
}

Player::Player()
{
    int a;
}

Player::Player(bool *a)
{
    id = playerCount;
    playerCount++;
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
    //playerCount++;
}

Player::Player(Team _team, double _x, double _y, double _angle, int _id):
    BaseEntity(_x, _y, _angle)
{
    playerRole = PlayerRole();
    team = _team;
    id = _id;

    //playerCount++;
}

Player::Player(Team _team, double _x, double _y, double _angle,
               int _id, PlayerRole _role, Velocity _v, Acceleration _a):
    BaseEntity(_x, _y, _angle)
{
    playerRole = _role;
    team = _team;
    id = _id;
    velocity = _v;
    acceleration = _a;

    //playerCount++;
}

Player::Player(const Player &p): BaseEntity(p)
{
    x = p.x;
    y = p.y;
    angle = p.angle;
    assignedRole = p.assignedRole;
    playerRole = p.playerRole;
    team = p.team;
    id = p.id;
    velocity = p.velocity;
    acceleration = p.acceleration;
}

Player::~Player()
{
}

PlayerRole Player::choosePlayerRole()
{
    switch (assignedRole) {
    case PlayerRole::DefenderLeft:
        return DefenderLeft();
        break;
    case PlayerRole::DefenderLeftCentre:
        return DefenderLeftCentre();
        break;
    case PlayerRole::DefenderRightCentre:
        return DefenderRightCentre();
        break;
    case PlayerRole::DefenderRight:
        return DefenderRight();
        break;
    case PlayerRole::SemidefenderLeft:
        return SemidefenderLeft();
        break;
    case PlayerRole::SemidefenderLeftCentre:
        return SemidefenderLeftCentre();
        break;
    case PlayerRole::SemidefenderRightCentre:
        return SemidefenderRightCentre();
        break;
    case PlayerRole::SemidefenderRight:
        return SemidefenderRight();
        break;
    case PlayerRole::AttackerLeft:
        return AttackerLeft();
        break;
    case PlayerRole::AttackerRight:
        return AttackerRight();
        break;
    case PlayerRole::Goalie:
        return::Goalie();
        break;
    case PlayerRole::Unassigned:
        return PlayerRole();
        break;
    }
    return PlayerRole();
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

QRectF Player::getKickableAreaRect(QPointF pos)
{
    return QRectF(PITCH_MARGIN + pos.x() - KICKABLE_AREA/2,
                  PITCH_MARGIN + pos.y() - KICKABLE_AREA/2,
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

    if (json.contains("velocity") && json["velocity"].isObject())
         velocity.readJSON(json["velocity"].toObject());

    if (json.contains("acceleration") && json["acceleration"].isObject())
         acceleration.readJSON(json["acceleration"].toObject());
}

void Player::writeJSON(QJsonObject &json) const
{
    json["team"] = static_cast<int>(team);
    json["assignedRole"] = static_cast<int>(assignedRole);
    json["x"] = x;
    json["y"] = y;
    json["angle"] = angle;
    QJsonObject velocityObject;
    velocity.writeJSON(velocityObject);
    json["velocity"] = velocityObject;
    QJsonObject accelerationObject;
    acceleration.writeJSON(accelerationObject);
    json["acceleration"] = accelerationObject;
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

    double accelerationAbs = vectorLength(acceleration.x, acceleration.y);
    if (accelerationAbs > PLAYER_SPEED_MAX) {
        acceleration.x /= (accelerationAbs / PLAYER_SPEED_MAX);
        acceleration.y /= (accelerationAbs / PLAYER_SPEED_MAX);
    }
}

// initialise static
const QVector<PlayerRole> Player::defaultFormation = getRolesVec();
int Player::playerCount = 0;
std::map<int, PlayerRole::RoleName> Player::player_roleMap;
