#include "PlayerRole.h"

const QPointF goalPoint = QPointF(0, r_PITCH_WIDTH/2);

PlayerRole::PlayerRole()
{

}

PlayerRole::PlayerRole(double _margin,
                       double _boxPos_x, double _boxPos_y,
                       double _boxWidth, double _boxLength,
                       double _pPosInBox_x, double _pPosInBox_y,
                       PlayerRole::RoleName _Role)
{
    roleMargin = _margin;
    boxPos_x = _boxPos_x;
    boxPos_y = _boxPos_y;
    boxWidth = _boxWidth;
    boxLength = _boxLength;
    pPosInBox_x = _pPosInBox_x;
    pPosInBox_y = _pPosInBox_y;
    playerRole = _Role;
}

QRectF PlayerRole::getRoleRect()
{
    return QRectF(r_PITCH_MARGIN + boxPos_x - boxLength/2 + roleMargin + curFormationMargin_x,
                  r_PITCH_MARGIN + boxPos_y - boxWidth/2 + curFormationMargin_y,
                  boxLength, boxWidth);
}

QPointF PlayerRole::getRolePoint()
{
    return QPointF(r_PITCH_MARGIN + boxPos_x + roleMargin + pPosInBox_x + curFormationMargin_x,
                   r_PITCH_MARGIN + boxPos_y + pPosInBox_y + curFormationMargin_y);
}

double PlayerRole::getRoleMargin() const
{
    return roleMargin;
}

QList<PlayerRole::RoleName> PlayerRole::getNeighbourRoles()
{
    QList<PlayerRole::RoleName> neighbourRoles;
    switch (this->playerRole) {
    case PlayerRole::DefenderLeft:
        neighbourRoles.append(PlayerRole::DefenderLeftCentre);
        neighbourRoles.append(PlayerRole::SemidefenderLeft);
        break;
    case PlayerRole::DefenderLeftCentre:
        neighbourRoles.append(PlayerRole::DefenderLeft);
        neighbourRoles.append(PlayerRole::DefenderRightCentre);
        neighbourRoles.append(PlayerRole::SemidefenderLeftCentre);
        break;
    case PlayerRole::DefenderRightCentre:
        neighbourRoles.append(PlayerRole::DefenderLeftCentre);
        neighbourRoles.append(PlayerRole::DefenderRight);
        neighbourRoles.append(PlayerRole::SemidefenderRightCentre);
        break;
    case PlayerRole::DefenderRight:
        neighbourRoles.append(PlayerRole::DefenderLeftCentre);
        neighbourRoles.append(PlayerRole::SemidefenderRight);
        break;
    case PlayerRole::SemidefenderLeft:
        neighbourRoles.append(PlayerRole::SemidefenderLeftCentre);
        neighbourRoles.append(PlayerRole::DefenderLeft);
        neighbourRoles.append(PlayerRole::AttackerLeft);
        break;
    case PlayerRole::SemidefenderLeftCentre:
        neighbourRoles.append(PlayerRole::SemidefenderRightCentre);
        neighbourRoles.append(PlayerRole::DefenderLeftCentre);
        neighbourRoles.append(PlayerRole::SemidefenderLeft);
        neighbourRoles.append(PlayerRole::AttackerLeft);
        break;
    case PlayerRole::SemidefenderRightCentre:
        neighbourRoles.append(PlayerRole::SemidefenderRight);
        neighbourRoles.append(PlayerRole::DefenderRightCentre);
        neighbourRoles.append(PlayerRole::SemidefenderLeftCentre);
        neighbourRoles.append(PlayerRole::AttackerRight);
        break;
    case PlayerRole::SemidefenderRight:
        neighbourRoles.append(PlayerRole::DefenderRight);
        neighbourRoles.append(PlayerRole::SemidefenderRightCentre);
        neighbourRoles.append(PlayerRole::AttackerRight);
        break;
    case PlayerRole::AttackerLeft:
        neighbourRoles.append(PlayerRole::SemidefenderLeft);
        neighbourRoles.append(PlayerRole::SemidefenderLeftCentre);
         neighbourRoles.append(PlayerRole::AttackerRight);
        break;
    case PlayerRole::AttackerRight:
        neighbourRoles.append(PlayerRole::SemidefenderRight);
        neighbourRoles.append(PlayerRole::SemidefenderRightCentre);
        neighbourRoles.append(PlayerRole::AttackerLeft);
        break;
    case PlayerRole::Goalie:
        // no such roles
        break;
    case PlayerRole::Unassigned:
        // no such roles
        break;
    }
    return neighbourRoles;
}

double PlayerRole::getFormationMargin_x() const
{
    return curFormationMargin_x;
}

void PlayerRole::setFormationMargin_x(double value)
{
    curFormationMargin_x = value;
}

double PlayerRole::getFormationMargin_y() const
{
    return curFormationMargin_y;
}

void PlayerRole::setFormationMargin_y(double value)
{
    curFormationMargin_y = value;
}

