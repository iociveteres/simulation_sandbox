#ifndef PLAYER_H
#define PLAYER_H

#include <QPainter>

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
};

#endif // PLAYER_H
