#ifndef PLAYERROLE_H
#define PLAYERROLE_H

#include "ModelConstants.h"

#include <QRectF>
#include <QList>

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
    PlayerRole(double _margin,
               double _boxPos_x, double _boxPos_y,
               double _boxWidth, double _boxLength,
               double _pPosInBox_x, double _pPosInBox_y,
               RoleName _Role);

    QRectF  getRoleRect();
    QPointF getRolePoint();
    double getRoleMargin() const;
    PlayerRole::RoleName getRoleName() const;

    QList<PlayerRole::RoleName> getNeighbourRoles();

    double getFormationMargin_x() const;
    void setFormationMargin_x(double value);

    double getFormationMargin_y() const;
    void setFormationMargin_y(double value);



private:
    double curFormationMargin_x = PITCH_LENGTH/10;
    double curFormationMargin_y = 0;
    double roleMargin;
    double boxPos_x;
    double boxPos_y;
    double boxWidth;
    double boxLength;
    double pPosInBox_x;
    double pPosInBox_y;;
    PlayerRole::RoleName roleName;

    static const QPointF goalPoint;
};

class Defender: public PlayerRole {

public:
    Defender(double margin,
           double boxPos_x, double boxPos_y,
           double boxWidth, double boxLength,
           double pPosInBox_x, double pPosInBox_y,
           RoleName Role):
        PlayerRole(margin,
                   boxPos_x, boxPos_y,
                   boxWidth, boxLength,
                   pPosInBox_x, pPosInBox_y,
                   Role) {}
};

class Semidefender: public PlayerRole {

public:
    Semidefender(double margin,
                 double boxPos_x, double boxPos_y,
                 double boxWidth, double boxLength,
                 double pPosInBox_x, double pPosInBox_y,
                 RoleName Role):
              PlayerRole(margin,
                         boxPos_x, boxPos_y,
                         boxWidth, boxLength,
                         pPosInBox_x, pPosInBox_y,
                         Role) {}
};

class Attacker: public PlayerRole {

public:
    Attacker(double margin,
             double boxPos_x, double boxPos_y,
             double boxWidth, double boxLength,
             double pPosInBox_x, double pPosInBox_y,
             RoleName Role):
          PlayerRole(margin,
                     boxPos_x, boxPos_y,
                     boxWidth, boxLength,
                     pPosInBox_x, pPosInBox_y,
                     Role) {}

};

class Goalie: public PlayerRole {

public:
    Goalie(
           double margin = 0,
           double boxPos_x = PITCH_LENGTH/16,
           double boxPos_y = PITCH_WIDTH/2,
           double boxWidth = PITCH_WIDTH/3,
           double boxLength = PITCH_LENGTH/5,
           double pPosInBox_x = 0,
           double pPosInBox_y = 0,
           RoleName Role = RoleName::Goalie):
        PlayerRole(margin,
                   boxPos_x, boxPos_y,
                   boxWidth, boxLength,
                   pPosInBox_x, pPosInBox_y,
                   Role) {}
};

class DefenderRight: public Defender {

public:
    DefenderRight(
           double margin = 0,
           double boxPos_x = PITCH_LENGTH/10,
           double boxPos_y = PITCH_WIDTH/8*7,
           double boxWidth = PITCH_WIDTH/4,
           double boxLength = PITCH_LENGTH/5,
           double pPosInBox_x = PITCH_LENGTH/20,
           double pPosInBox_y = -PITCH_WIDTH/16,
           RoleName Role = RoleName::DefenderRight):
        Defender(margin,
                 boxPos_x, boxPos_y,
                 boxWidth, boxLength,
                 pPosInBox_x, pPosInBox_y,
                 Role) {}
};


class DefenderRightCentre: public Defender {

public:
    DefenderRightCentre(
           double margin = 0,
           double boxPos_x = PITCH_LENGTH/10,
           double boxPos_y = PITCH_WIDTH/8*5,
           double boxWidth = PITCH_WIDTH/4,
           double boxLength = PITCH_LENGTH/5,
           double pPosInBox_x = PITCH_LENGTH/20,
           double pPosInBox_y = 0,
           RoleName Role = RoleName::DefenderRightCentre):
        Defender(margin,
                 boxPos_x, boxPos_y,
                 boxWidth, boxLength,
                 pPosInBox_x, pPosInBox_y,
                 Role) {}
};


class DefenderLeftCentre: public Defender {

public:
    DefenderLeftCentre(
           double margin = 0,
           double boxPos_x = PITCH_LENGTH/10,
           double boxPos_y = PITCH_WIDTH/8*3,
           double boxWidth = PITCH_WIDTH/4,
           double boxLength = PITCH_LENGTH/5,
           double pPosInBox_x = PITCH_LENGTH/20,
           double pPosInBox_y = 0,
           RoleName Role = RoleName::DefenderLeftCentre):
        Defender(margin,
                 boxPos_x, boxPos_y,
                 boxWidth, boxLength,
                 pPosInBox_x, pPosInBox_y,
                 Role) {}
};


class DefenderLeft: public Defender {

public:
    DefenderLeft(
           double margin = 0,
           double boxPos_x = PITCH_LENGTH/10,
           double boxPos_y = PITCH_WIDTH/8*1,
           double boxWidth = PITCH_WIDTH/4,
           double boxLength = PITCH_LENGTH/5,
           double pPosInBox_x = PITCH_LENGTH/20,
           double pPosInBox_y = PITCH_WIDTH/16,
           RoleName Role = RoleName::DefenderLeft):
        Defender(margin,
                 boxPos_x, boxPos_y,
                 boxWidth, boxLength,
                 pPosInBox_x, pPosInBox_y,
                 Role) {}
};


class SemidefenderRight: public Semidefender {

public:
    SemidefenderRight(
           double margin = PITCH_LENGTH/5,
           double boxPos_x = PITCH_LENGTH/10,
           double boxPos_y = PITCH_WIDTH/8*7,
           double boxWidth = PITCH_WIDTH/4,
           double boxLength = PITCH_LENGTH/5,
           double pPosInBox_x = 0,
           double pPosInBox_y = -PITCH_WIDTH/16,
           RoleName Role = RoleName::SemidefenderRight):
        Semidefender(margin,
                     boxPos_x, boxPos_y,
                     boxWidth, boxLength,
                     pPosInBox_x, pPosInBox_y,
                     Role) {}
};

class SemidefenderRightCentre: public Semidefender {

public:
    SemidefenderRightCentre(
           double margin = PITCH_LENGTH/5,
           double boxPos_x = PITCH_LENGTH/10,
           double boxPos_y = PITCH_WIDTH/8*5,
           double boxWidth = PITCH_WIDTH/4,
           double boxLength = PITCH_LENGTH/5,
           double pPosInBox_x = 0,
           double pPosInBox_y = 0,
           RoleName Role = RoleName::SemidefenderRightCentre):
        Semidefender(margin,
                     boxPos_x, boxPos_y,
                     boxWidth, boxLength,
                     pPosInBox_x, pPosInBox_y,
                     Role) {}
};


class SemidefenderLeftCentre: public Semidefender {

public:
    SemidefenderLeftCentre(
           double margin = PITCH_LENGTH/5,
           double boxPos_x = PITCH_LENGTH/10,
           double boxPos_y = PITCH_WIDTH/8*3,
           double boxWidth = PITCH_WIDTH/4,
           double boxLength = PITCH_LENGTH/5,
           double pPosInBox_x = 0,
           double pPosInBox_y = 0,
           RoleName Role = RoleName::SemidefenderLeftCentre):
        Semidefender(margin,
                     boxPos_x, boxPos_y,
                     boxWidth, boxLength,
                     pPosInBox_x, pPosInBox_y,
                     Role) {}
};


class SemidefenderLeft: public Semidefender {

public:
    SemidefenderLeft(
           double margin = PITCH_LENGTH/5,
           double boxPos_x = PITCH_LENGTH/10,
           double boxPos_y = PITCH_WIDTH/8*1,
           double boxWidth = PITCH_WIDTH/4,
           double boxLength = PITCH_LENGTH/5,
           double pPosInBox_x = 0,
           double pPosInBox_y = PITCH_WIDTH/16,
           RoleName Role = RoleName::SemidefenderLeft):
        Semidefender(margin,
                     boxPos_x, boxPos_y,
                     boxWidth, boxLength,
                     pPosInBox_x, pPosInBox_y,
                     Role) {}
};

class AttackerRight: public Attacker {

public:
    AttackerRight(
           double margin = PITCH_LENGTH/2.5,
           double boxPos_x = PITCH_LENGTH/10,
           double boxPos_y = PITCH_WIDTH/8*6,
           double boxWidth = PITCH_WIDTH/2,
           double boxLength = PITCH_LENGTH/5,
           double pPosInBox_x = 0,
           double pPosInBox_y = -PITCH_WIDTH/4,
           RoleName Role = RoleName::AttackerRight):
        Attacker(margin,
                 boxPos_x, boxPos_y,
                 boxWidth, boxLength,
                 pPosInBox_x, pPosInBox_y,
                 Role) {}
};

class AttackerLeft: public Attacker {

public:
    AttackerLeft(
           double margin = PITCH_LENGTH/2.5,
           double boxPos_x = PITCH_LENGTH/10,
           double boxPos_y = PITCH_WIDTH/8*2,
           double boxWidth = PITCH_WIDTH/2,
           double boxLength = PITCH_LENGTH/5,
           double pPosInBox_x = 0,
           double pPosInBox_y = PITCH_WIDTH/4,
           RoleName Role = RoleName::AttackerLeft):
        Attacker(margin,
                 boxPos_x, boxPos_y,
                 boxWidth, boxLength,
                 pPosInBox_x, pPosInBox_y,
                 Role) {}
};


#endif // PLAYERROLE_H
