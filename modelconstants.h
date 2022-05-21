#ifndef MODELCONSTANTS_H
#define MODELCONSTANTS_H

#include <math.h>

const double r_PITCH_LENGTH = 105.0;
const double r_PITCH_WIDTH = 68.0;
const double r_PITCH_MARGIN = 5.0;
const double r_CENTER_CIRCLE_R = 9.15;
const double r_PENALTY_AREA_LENGTH = 16.5;
const double r_PENALTY_AREA_WIDTH = 40.32;
const double r_GOAL_AREA_LENGTH = 5.5;
const double r_GOAL_AREA_WIDTH = 18.32;
const double r_GOAL_WIDTH = 14.02;
const double r_GOAL_DEPTH = 2.44;
const double r_PENALTY_SPOT_DIST = 11.0;
const double r_PENALTY_ARC_R = 9.17;
const double r_CORNER_ARC_R = 1.0 * 2;

const double r_PLAYER_SIZE = 0.3;
const double r_PLAYER_WIDGET_SIZE = 1.0;
const double r_KICKABLE_MARGIN = 0.7;
const double r_BALL_SIZE = 0.085;
const double r_KICKABLE_AREA = r_PLAYER_SIZE + r_KICKABLE_MARGIN + r_BALL_SIZE;

const int r_TOTAL_FIELD_LENGTH = r_PITCH_LENGTH + 2*r_PITCH_MARGIN;
const int r_TOTAL_FIELD_WIDTH = r_PITCH_WIDTH + 2*r_PITCH_MARGIN;
//view distance
const double team_far_length = 40.0;
const double team_too_far_length = 60.0;

const double quantize_step = 0.1;
const double quantize_step_l = 0.01;



int toPixmapCoord(double value);

const int d_PITCH_LENGTH = toPixmapCoord(r_PITCH_LENGTH);
const int d_PITCH_WIDTH = toPixmapCoord(r_PITCH_WIDTH);
const int d_PITCH_MARGIN = toPixmapCoord(r_PITCH_MARGIN);

const int d_TOTAL_IMAGE_WIDTH = d_PITCH_LENGTH + 2*d_PITCH_MARGIN;
const int d_TOTAL_IMAGE_HEIGHT = d_PITCH_WIDTH + 2*d_PITCH_MARGIN;

#endif // MODELCONSTANTS_H
