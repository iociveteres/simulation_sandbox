#include "Velocity.h"

Velocity::Velocity()
{

}

void Velocity::readJSON(const QJsonObject &json)
{
    if (json.contains("x") && json["x"].isDouble())
        x = json["x"].toDouble();
    if (json.contains("y") && json["y"].isDouble())
        x = json["y"].toDouble();
}

Acceleration::Acceleration()
{

}

void Acceleration::readJSON(const QJsonObject &json)
{
    if (json.contains("x") && json["x"].isDouble())
        x = json["x"].toDouble();
    if (json.contains("y") && json["y"].isDouble())
        x = json["y"].toDouble();
}
