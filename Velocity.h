#ifndef VELOCITY_H
#define VELOCITY_H

#include <QJsonObject>

class Velocity
{
public:
    double x = 0;
    double y = 0;

    Velocity();
    void readJSON(const QJsonObject &json);
    void writeJSON(QJsonObject &json) const;
};

class Acceleration
{
public:
    double x = 0;
    double y = 0;

    Acceleration();
    void readJSON(const QJsonObject &json);
    void writeJSON(QJsonObject &json) const;
};
#endif // VELOCITY_H
