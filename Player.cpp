#include "Player.h"

Player::Player()
{
}

Player::Player(Team _team)
{
    team = _team;
}

Player::Player(Team _team, int _x, int _y, double _angle)
{
    team = _team;
    x = _x;
    y = _y;
    angle = _angle;
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

// От центра до левого верхнего края

QRectF Player::getKickableAreaRect()
{
    return QRectF(x - r_KICKABLE_AREA/2, y - r_KICKABLE_AREA/2,
                 r_KICKABLE_AREA, r_KICKABLE_AREA);
}

QRectF Player::getPlayerWidgetRect()
{
    return QRectF(x - r_PLAYER_WIDGET_SIZE/2, y - r_PLAYER_WIDGET_SIZE/2,
                 r_KICKABLE_AREA, r_KICKABLE_AREA);

}

void Player::readJSON(const QJsonObject &json)
{
    if (json.contains("team") && json["team"].isDouble())
        team = Team(json["team"].toInt());

    if (json.contains("isGoalie") && json["isGoalie"].isBool())
        isGoalie = json["isGoalie"].toBool();

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
    json["isGoalie"] = isGoalie;
    json["x"] = x;
    json["y"] = y;
    json["angle"] = angle;
}

Player::Team Player::getTeam() const
{
    return team;
}
