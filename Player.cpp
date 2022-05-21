#include "Player.h"

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

Player::Player(Team _team, int _x, int _y, double _angle)
{
    team = _team;
    x = _x;
    y = _y;
    angle = _angle;
    id = playerCount;
    playerCount++;
}

}

void Player::setRole(PlayerRole role)
{
    playerRole = role;
}

PlayerRole Player::checkRole()
{
//    for (PlayerRole r: roles) {

//    }
    return Goalie();
}

int Player::getX() {
    return x;
}

int Player::getY() {
    return y;
}

int Player::getAngle() {
    return angle;
}

QVector<PlayerRole> Player::getRoles()
{
    return Player::roles;
}

// От центра до левого верхнего края

QRectF Player::getKickableAreaRect()
{
    return QRectF(r_PITCH_MARGIN+ x - r_KICKABLE_AREA/2,
                  r_PITCH_MARGIN + y - r_KICKABLE_AREA/2,
                 r_KICKABLE_AREA, r_KICKABLE_AREA);
}

QRectF Player::getPlayerWidgetRect()
{
    return QRectF(r_PITCH_MARGIN + x - r_PLAYER_WIDGET_SIZE/2,
                  r_PITCH_MARGIN + y - r_PLAYER_WIDGET_SIZE/2,
                 r_KICKABLE_AREA, r_KICKABLE_AREA);

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
