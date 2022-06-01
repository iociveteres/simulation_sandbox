#ifndef BASEENTITY_H
#define BASEENTITY_H

#include <QPointF>
#include "Velocity.h"

class BaseEntity
{
protected:
    double x;
    double y;
    double angle;

    Velocity velocity;
    Acceleration acceleration;
public:
    BaseEntity();
    BaseEntity(double _x, double _y, double _angle);
    BaseEntity(double _x, double _y, double _angle,
               Velocity _velocity, Acceleration _acceleration);


    double getX() const;
    double getY() const;
    double getAngle() const;
    QPointF getCoordinatesPoint();

    void setX(double value);
    void setY(double value);
    void setAngle(double value);

    void tick();
   };

#endif // BASEENTITY_H
