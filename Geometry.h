#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <QLineF>
#include <QPointF>
#include <cmath>

double distance(QPointF a, QPointF b);
double distance(double ax, double ay, double bx, double by);
double distanceLineAndPoint(QLineF l, QPointF p);

#endif // GEOMETRY_H
