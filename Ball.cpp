#include "Ball.h"

Ball::Ball()
{

}

Ball::Ball(double _x, double _y)
{
    x = _x;
    y = _y;

}

double Ball::getX() const
{
    return x;
}

double Ball::getY() const
{
    return y;
}



QRectF Ball::getBallRect()
{
    return QRectF(x - r_BALL_SIZE/2, y - r_BALL_SIZE/2,
                 r_BALL_SIZE, r_BALL_SIZE);
}

void Ball::readJSON(const QJsonObject &json)
{
    if (json.contains("x") && json["x"].isDouble())
        x = json["x"].toDouble();

    if (json.contains("y") && json["y"].isDouble())
        y = json["y"].toDouble();
}

void Ball::writeJSON(QJsonObject &json) const
{
    json["x"] = x;
    json["y"] = y;
}
