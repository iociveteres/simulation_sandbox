#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include "Player.h"

class World
{
private:
    std::vector<Player> teamAlly;
    std::vector<Player> teamEnemy;

    Ball ball;

public:
    World();
};

#endif // WORLD_H
