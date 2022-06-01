#ifndef MODELCONSTANTS_H
#define MODELCONSTANTS_H

#include <math.h>

const double BALL_DECAY = 0.94;
const double BALL_SPEED_MAX = 3.0;
const double BALL_ACCEL_MAX = 2.7;


const double PLAYER_DECAY = 0.4;
const double PLAYER_SPEED_MAX = 1.05;
const double PLAYER_ACCEL_MAX = 1.0;

const double PITCH_LENGTH = 105.0;
const double PITCH_WIDTH = 68.0;
const double PITCH_MARGIN = 5.0;
const double CENTER_CIRCLE_R = 9.15;
const double PENALTY_AREA_LENGTH = 16.5;
const double PENALTY_AREA_WIDTH = 40.32;
const double GOAL_AREA_LENGTH = 5.5;
const double GOAL_AREA_WIDTH = 18.32;
const double GOAL_WIDTH = 14.02;
const double GOAL_DEPTH = 2.44;
const double PENALTY_SPOT_DIST = 11.0;
const double PENALTY_ARC_R = 9.17;
const double CORNER_ARC_R = 1.0 * 2;

const double PLAYER_SIZE = 1;
const double PLAYER_WIDGET_SIZE = 1.0;
const double KICKABLE_MARGIN = 0.7;
const double BALL_SIZE = 1;
const double KICKABLE_AREA = PLAYER_SIZE + KICKABLE_MARGIN + BALL_SIZE;

const int TOTAL_FIELD_LENGTH = PITCH_LENGTH + 2*PITCH_MARGIN;
const int TOTAL_FIELD_WIDTH = PITCH_WIDTH + 2*PITCH_MARGIN;
//view distance
const double team_far_length = 40.0;
const double team_too_far_length = 60.0;

const double quantize_step = 0.1;
const double quantize_step_l = 0.01;

#endif // MODELCONSTANTS_H
