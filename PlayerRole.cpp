#include "PlayerRole.h"

PlayerRole::PlayerRole()
{

}

PlayerRole::PlayerRole(double _margin, double _boxPos_x, double _boxPos_Y, double _boxWidth, double _boxLength, PlayerRole::RoleName _Role)
{
    roleMargin = _margin;
    boxPos_x = _boxPos_x;
    boxPos_y = _boxPos_Y;
    boxWidth = _boxWidth;
    boxLength = _boxLength;
    playerRole = _Role;
}

QRectF PlayerRole::getRoleRect()
{
    return QRectF(r_PITCH_MARGIN + boxPos_x - boxLength/2 + roleMargin + marginFromGoal,
                  r_PITCH_MARGIN + boxPos_y - boxWidth/2,
                  boxLength, boxWidth);
}

QPointF PlayerRole::getRolePoint()
{
    return QPointF(r_PITCH_MARGIN + boxPos_x + roleMargin + marginFromGoal,
                   r_PITCH_MARGIN + boxPos_y);
}

double PlayerRole::getRoleMargin() const
{
    return roleMargin;
}

double PlayerRole::getMarginFromGoal() const
{
    return marginFromGoal;
}

void PlayerRole::setMarginFromGoal(double value)
{
    marginFromGoal = value;
}

