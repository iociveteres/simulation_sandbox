#include "Ball.h"

double Ball::getX() const
{
    return x;
}

double Ball::getY() const
{
    return y;
}

Ball::Ball(double _x, double _y)
{
    x = _x;
    y = _y;

}

QRectF Ball::getBallRect()
{
    //double a = r_BALL_SIZE;
    return QRectF(x - r_BALL_SIZE/2, y - r_BALL_SIZE/2,
                 r_BALL_SIZE, r_BALL_SIZE);
}
