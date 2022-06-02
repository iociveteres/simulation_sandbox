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

BaseEntity::BaseEntity(double _x, double _y, double _angle,
                       Velocity _velocity, Acceleration _acceleration)
{
    x = _x;
    y = _y;
    angle = _angle;
    velocity = _velocity;
    acceleration = _acceleration;
}

BaseEntity::BaseEntity(const BaseEntity &b)
{
    x = b.x;
    y = b.y;
    y = b.angle;
    velocity = b.velocity;
    acceleration = b.acceleration;
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

Velocity BaseEntity::getVelocity() const
{
    return velocity;
}


void BaseEntity::setVelocity(const Velocity &value)
{
    velocity = value;
}

Acceleration BaseEntity::getAcceleration() const
{
    return acceleration;
}

void BaseEntity::setAcceleration(const Acceleration &value)
{
    acceleration = value;
}

QPointF BaseEntity::getPVelocity() const
{
    return QPointF(velocity.x, velocity.y);
}

QPointF BaseEntity::getPAcceleration() const
{
    return QPointF(acceleration.x, acceleration.y);
}

void BaseEntity::tick()
{
    velocity.x += acceleration.x;
    velocity.y += acceleration.y;

    this->x += velocity.x;
    this->y += velocity.y;

    acceleration.x = 0;
    acceleration.y = 0;
}
