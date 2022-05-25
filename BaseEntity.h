#ifndef BASEENTITY_H
#define BASEENTITY_H

#include <QPointF>

class BaseEntity
{
protected:
    double x;
    double y;
    double angle;
public:
    BaseEntity();
    BaseEntity(double _x, double _y, double _angle);

    double getX() const;
    double getY() const;
    double getAngle() const;
    QPointF getCoordinatesPoint();

    void setX(double value);
    void setY(double value);
    void setAngle(double value);
};

#endif // BASEENTITY_H
