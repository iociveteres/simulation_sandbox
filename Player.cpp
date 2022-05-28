#include "Player.h"

int Player::getId() const
{
    return id;
}

PlayerRole Player::getPlayerRole() const
{
    return playerRole;
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
    return QRectF(r_PITCH_MARGIN + x - r_KICKABLE_AREA/2,
                  r_PITCH_MARGIN + y - r_KICKABLE_AREA/2,
                 r_KICKABLE_AREA, r_KICKABLE_AREA);
}



QRectF Player::getPlayerWidgetRect()
{
    return QRectF(r_PITCH_MARGIN + x - r_PLAYER_WIDGET_SIZE/2,
                  r_PITCH_MARGIN + y - r_PLAYER_WIDGET_SIZE/2,
                 r_KICKABLE_AREA, r_KICKABLE_AREA);

}

QVector<PlayerRole> Player::getRoles()
{
    return Player::roles;
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

// initialise static
const QVector<PlayerRole> Player::roles = getRolesVec();
int Player::playerCount = 0;
