#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <QLineF>
#include <QPointF>
#include <cmath>

double limit(double value, double top, double bot);
double distance(QPointF a, QPointF b);
double distance(double ax, double ay, double bx, double by);
double vectorLength(double ax, double ay);
double distanceLineAndPoint(QLineF l, QPointF p);
QPointF findPointInDistFromEndOfSegment(QPointF a, QPointF b, double dist);
std::tuple<QPointF, QPointF> getTangentPointsFromLineAndCircle(QPointF p, QPointF c, double r);
QPointF intersectTwoLines(QPointF A, QPointF B, QPointF C, QPointF D);
double howGoodTwoLinesFit(QPointF A, QPointF B, QPointF C, QPointF D);

#endif // GEOMETRY_H
