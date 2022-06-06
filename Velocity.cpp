#include "Velocity.h"

Velocity::Velocity()
{

}

void Velocity::readJSON(const QJsonObject &json)
{
    if (json.contains("x") && json["x"].isDouble())
        x = json["x"].toDouble();
    if (json.contains("y") && json["y"].isDouble())
        y = json["y"].toDouble();
}

void Velocity::writeJSON(QJsonObject &json) const
{
    json["x"] = x;
    json["y"] = y;
}

Acceleration::Acceleration()
{

}

void Acceleration::readJSON(const QJsonObject &json)
{
    if (json.contains("x") && json["x"].isDouble())
        x = json["x"].toDouble();
    if (json.contains("y") && json["y"].isDouble())
        y = json["y"].toDouble();
}

void Acceleration::writeJSON(QJsonObject &json) const
{
    json["x"] = x;
    json["y"] = y;
}
