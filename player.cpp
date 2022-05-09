#include "player.h"

Player::Player(Team _team)
{
    team = _team;
}

// От центра до левого верхнего края
QRect Player::getKickableAreaRect()
{
    return QRect(mul(x) - d_KICKABLE_AREA/2, mul(y) - d_KICKABLE_AREA/2,
                 d_KICKABLE_AREA, d_KICKABLE_AREA);
}

QRect Player::getPlayerWidgetRect()
{
    return QRect(mul(x) - d_PLAYER_WIDGET_SIZE/2, mul(y) - d_PLAYER_WIDGET_SIZE/2,
                 d_KICKABLE_AREA, d_KICKABLE_AREA);

}
