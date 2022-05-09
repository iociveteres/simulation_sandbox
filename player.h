#ifndef PLAYER_H
#define PLAYER_H

#include <QPainter>
#include "ModelConstants.h"

class Player
{
    enum class Team {ally, enemy};
private:
    Team team;
    bool isGoalie = false;

    int x;
    int y;
    double angle;
public:
    Player(Team team);
    void setPosition();
    void setAngle();
    QRect getKickableAreaRect();
    QRect getPlayerWidgetRect();
};

#endif // PLAYER_H
