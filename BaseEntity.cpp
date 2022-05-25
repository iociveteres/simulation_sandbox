#include "BaseEntity.h"

BaseEntity::BaseEntity()
{

}

BaseEntity::BaseEntity(double _x, double _y, double _angle)
{
    x = _x;
    y = _y;
    angle = _angle;
}

void BaseEntity::setX(double value)
{
    x = value;
}

void BaseEntity::setY(double value)
{
    y = value;
}

void BaseEntity::setAngle(double value)
{
    angle = value;
}

double BaseEntity::getX() const
{
    return x;
}

double BaseEntity::getY() const
{
    return y;
}

double BaseEntity::getAngle() const
{
    return angle;
}

QPointF BaseEntity::getCoordinatesPoint() {
    return QPointF(x, y);
}
