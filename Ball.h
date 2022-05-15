#ifndef BALL_H
#define BALL_H

#include <QRectF>
#include <QJsonObject>
#include "ModelConstants.h"

class Ball
{
private:
    double x;
    double y;
public:
    Ball();
    Ball(double _x, double _y);
    double getX() const;
    double getY() const;
    QRectF getBallRect();
    void writeJSON(QJsonObject &json) const;
    void readJSON(const QJsonObject &json);
};

#endif // BALL_H
