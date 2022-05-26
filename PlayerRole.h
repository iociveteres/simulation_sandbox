#ifndef PLAYERROLE_H
#define PLAYERROLE_H

#include "ModelConstants.h"

#include <QRectF>

class PlayerRole
{
public:
    enum RoleName {
        Unassigned,
        Goalie,
        DefenderRight,
        DefenderRightCentre,
        DefenderLeftCentre,
        DefenderLeft,
        SemidefenderRight,
        SemidefenderRightCentre,
        SemidefenderLeftCentre,
        SemidefenderLeft,
        AttackerRight,
        AttackerLeft
    };
    PlayerRole();
    PlayerRole(double _margin, double _boxPos_x, double _boxPos_Y,
               double _boxWidth, double _boxLength, RoleName _Role);
    RoleName getPlayerRole() const;
    QRectF  getRoleRect();
    QPointF getRolePoint();
    double getRoleMargin() const;

private:
    double roleMargin;
    double boxPos_x;
    double boxPos_y;
    double boxWidth;
    double boxLength;
    RoleName playerRole;
};

class Defender: public PlayerRole {

public:
    Defender(double margin,
           double boxPos_x,
           double boxPos_y,
           double boxWidth,
           double boxLength,
           RoleName Role):
        PlayerRole(margin, boxPos_x, boxPos_y, boxWidth, boxLength, Role) {}
};

class Semidefender: public PlayerRole {

public:
    Semidefender(double margin,
           double boxPos_x,
           double boxPos_y,
           double boxWidth,
           double boxLength,
           RoleName Role):
        PlayerRole(margin, boxPos_x, boxPos_y, boxWidth, boxLength, Role) {}
};

class Attacker: public PlayerRole {

public:
    Attacker(double margin,
           double boxPos_x,
           double boxPos_y,
           double boxWidth,
           double boxLength,
           RoleName Role):
        PlayerRole(margin, boxPos_x, boxPos_y, boxWidth, boxLength, Role) {}

};

class Goalie: public PlayerRole {

public:
    Goalie(
           double margin = 0,
           double boxPos_x = r_PITCH_LENGTH/16,
           double boxPos_y = r_PITCH_WIDTH/2,
           double boxWidth = r_PITCH_WIDTH/3,
           double boxLength = r_PITCH_LENGTH/5,
           RoleName Role = RoleName::Goalie):
        PlayerRole(margin, boxPos_x, boxPos_y, boxWidth, boxLength, Role) {}
};

class DefenderRight: public Defender {

public:
    DefenderRight(
           double margin = 0,
           double boxPos_x = r_PITCH_LENGTH/10,
           double boxPos_y = r_PITCH_WIDTH/8*7,
           double boxWidth = r_PITCH_WIDTH/4,
           double boxLength = r_PITCH_LENGTH/5,
           RoleName Role = RoleName::DefenderRight):
        Defender(margin, boxPos_x, boxPos_y, boxWidth, boxLength, Role) {}
};


class DefenderRightCentre: public Defender {

public:
    DefenderRightCentre(
           double margin = 0,
           double boxPos_x = r_PITCH_LENGTH/10,
           double boxPos_y = r_PITCH_WIDTH/8*5,
           double boxWidth = r_PITCH_WIDTH/4,
           double boxLength = r_PITCH_LENGTH/5,
           RoleName Role = RoleName::DefenderRightCentre):
        Defender(margin, boxPos_x, boxPos_y, boxWidth, boxLength, Role) {}
};


class DefenderLeftCentre: public Defender {

public:
    DefenderLeftCentre(
           double margin = 0,
           double boxPos_x = r_PITCH_LENGTH/10,
           double boxPos_y = r_PITCH_WIDTH/8*3,
           double boxWidth = r_PITCH_WIDTH/4,
           double boxLength = r_PITCH_LENGTH/5,
           RoleName Role = RoleName::DefenderLeftCentre):
        Defender(margin, boxPos_x, boxPos_y, boxWidth, boxLength, Role) {}
};


class DefenderLeft: public Defender {

public:
    DefenderLeft(
           double margin = 0,
           double boxPos_x = r_PITCH_LENGTH/10,
           double boxPos_y = r_PITCH_WIDTH/8*1,
           double boxWidth = r_PITCH_WIDTH/4,
           double boxLength = r_PITCH_LENGTH/5,
           RoleName Role = RoleName::DefenderRight):
        Defender(margin, boxPos_x, boxPos_y, boxWidth, boxLength, Role) {}
};


class SemidefenderRight: public Semidefender {

public:
    SemidefenderRight(
           double margin = r_PITCH_LENGTH/5,
           double boxPos_x = r_PITCH_LENGTH/10,
           double boxPos_y = r_PITCH_WIDTH/8*7,
           double boxWidth = r_PITCH_WIDTH/4,
           double boxLength = r_PITCH_LENGTH/5,
           RoleName Role = RoleName::SemidefenderRight):
        Semidefender(margin, boxPos_x, boxPos_y, boxWidth, boxLength, Role) {}
};

class SemidefenderRightCentre: public Semidefender {

public:
    SemidefenderRightCentre(
           double margin = r_PITCH_LENGTH/5,
           double boxPos_x = r_PITCH_LENGTH/10,
           double boxPos_y = r_PITCH_WIDTH/8*5,
           double boxWidth = r_PITCH_WIDTH/4,
           double boxLength = r_PITCH_LENGTH/5,
           RoleName Role = RoleName::SemidefenderRightCentre):
        Semidefender(margin, boxPos_x, boxPos_y, boxWidth, boxLength, Role) {}
};


class SemidefenderLeftCentre: public Semidefender {

public:
    SemidefenderLeftCentre(
           double margin = r_PITCH_LENGTH/5,
           double boxPos_x = r_PITCH_LENGTH/10,
           double boxPos_y = r_PITCH_WIDTH/8*3,
           double boxWidth = r_PITCH_WIDTH/4,
           double boxLength = r_PITCH_LENGTH/5,
           RoleName Role = RoleName::SemidefenderLeftCentre):
        Semidefender(margin, boxPos_x, boxPos_y, boxWidth, boxLength, Role) {}
};


class SemidefenderLeft: public Semidefender {

public:
    SemidefenderLeft(
           double margin = r_PITCH_LENGTH/5,
           double boxPos_x = r_PITCH_LENGTH/10,
           double boxPos_y = r_PITCH_WIDTH/8*1,
           double boxWidth = r_PITCH_WIDTH/4,
           double boxLength = r_PITCH_LENGTH/5,
           RoleName Role = RoleName::SemidefenderLeft):
        Semidefender(margin, boxPos_x, boxPos_y, boxWidth, boxLength, Role) {}
};

class AttackerRight: public Attacker {

public:
    AttackerRight(
           double margin = r_PITCH_LENGTH/2.5,
           double boxPos_x = r_PITCH_LENGTH/10,
           double boxPos_y = r_PITCH_WIDTH/8*6,
           double boxWidth = r_PITCH_WIDTH/2,
           double boxLength = r_PITCH_LENGTH/5,
           RoleName Role = RoleName::AttackerRight):
        Attacker(margin, boxPos_x, boxPos_y, boxWidth, boxLength, Role) {}
};

class AttackerLeft: public Attacker {

public:
    AttackerLeft(
           double margin = r_PITCH_LENGTH/2.5,
           double boxPos_x = r_PITCH_LENGTH/10,
           double boxPos_y = r_PITCH_WIDTH/8*2,
           double boxWidth = r_PITCH_WIDTH/2,
           double boxLength = r_PITCH_LENGTH/5,
           RoleName Role = RoleName::AttackerLeft):
        Attacker(margin, boxPos_x, boxPos_y, boxWidth, boxLength, Role) {}
};



#endif // PLAYERROLE_H
