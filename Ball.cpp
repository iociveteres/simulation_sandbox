#include "Ball.h"

Ball::Ball()
{

}

Ball::Ball(double _x, double _y):
    BaseEntity(_x, _y, 0)
{

}

Ball::Ball(Ball &_ball):
    BaseEntity(_ball.x, _ball.y, 0, _ball.velocity, _ball.acceleration)
{

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
    return QRectF(PITCH_MARGIN + x - BALL_WIDGET_SIZE, PITCH_MARGIN + y - BALL_WIDGET_SIZE / 2,
                 BALL_WIDGET_SIZE, BALL_WIDGET_SIZE);
}

void Ball::readJSON(const QJsonObject &json)
{
    if (json.contains("x") && json["x"].isDouble())
        x = json["x"].toDouble();

    if (json.contains("y") && json["y"].isDouble())
        y = json["y"].toDouble();
}

void Ball::tick()
{
    velocity.x += acceleration.x;
    velocity.y += acceleration.y;

    this->x += velocity.x;
    this->y += velocity.y;

    velocity.x *= BALL_DECAY;
    velocity.y *= BALL_DECAY;

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

void Ball::writeJSON(QJsonObject &json) const
{
    json["x"] = x;
    json["y"] = y;
}
