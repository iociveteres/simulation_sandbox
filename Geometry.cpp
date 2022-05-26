#include "Geometry.h"

double distance(QPointF a, QPointF b)
{
    return sqrt(pow(b.x() - a.x(), 2)+pow(b.y() - a.y(), 2));
}

double distance(double ax, double ay, double bx, double by)
{
    return sqrt(pow(bx - ax, 2)+pow(by - ay, 2));
}

double distanceLineAndPoint(QLineF l, QPointF p) {
    return abs((l.x2()-l.x1())*(l.y1()-p.y())-(l.x1()-p.x())*(l.y2()-l.y1()))
            /sqrt(pow(l.x2()-l.x1(), 2) + pow(l.y2()-l.y1(),2));
}
