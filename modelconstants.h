#ifndef MODELCONSTANTS_H
#define MODELCONSTANTS_H

#include <math.h>

const double r_PLAYER_SIZE = 0.3;
const double r_PLAYER_WIDGET_SIZE = 1.0;
const double r_KICKABLE_MARGIN = 0.7;
const double r_BALL_SIZE = 0.085;
const double r_KICKABLE_AREA = r_PLAYER_SIZE + r_KICKABLE_MARGIN + r_BALL_SIZE;

const double r_PITCH_LENGTH = 105.0;
const double r_PITCH_WIDTH = 68.0;
const double r_PITCH_MARGIN = 5.0;
const double r_GOAL_AREA_LENGTH = 5.5;
const double r_GOAL_AREA_WIDTH = 18.32;
const double r_GOAL_WIDTH = 14.02;
const double r_GOAL_DEPTH = 2.44;




int mul(double value) {
    return floor(value*10);
}

const int d_PLAYER_SIZE = mul(r_PLAYER_SIZE);
const int d_PLAYER_WIDGET_SIZE = mul(r_PLAYER_WIDGET_SIZE);
const int d_BALL_SIZE = mul(r_BALL_SIZE);
const int d_KICKABLE_AREA = mul(r_KICKABLE_AREA);

const int d_PITCH_LENGTH = mul(r_PITCH_LENGTH);
const int d_PITCH_WIDTH = mul(r_PITCH_WIDTH);
const int d_PITCH_MARGIN = mul(r_PITCH_MARGIN);
const int d_GOAL_AREA_LENGTH = mul(r_GOAL_AREA_LENGTH);
const int d_GOAL_AREA_WIDTH = mul(r_GOAL_AREA_WIDTH);
const int d_GOAL_WIDTH = mul(r_GOAL_WIDTH);
const int d_GOAL_DEPTH = mul(r_GOAL_DEPTH);



const int d_TOTAL_IMAGE_WIDTH = d_PITCH_LENGTH + d_PITCH_MARGIN;
const int d_TOTAL_IMAGE_HEIGHT = d_PITCH_WIDTH + d_PITCH_MARGIN;

#endif // MODELCONSTANTS_H
