#include "Geometry.h"

#include <QPainterPath>
#include <algorithm>
#include <array>

double limit(double value, double bot, double top) {
    if (value >= top)
        return top;
    if (value <= bot)
        return bot;
    return value;
}

double distance(QPointF a, QPointF b)
{
    return sqrt(pow(b.x() - a.x(), 2)+pow(b.y() - a.y(), 2));
}

double distance(double ax, double ay, double bx, double by)
{
    return sqrt(pow(bx - ax, 2)+pow(by - ay, 2));
}

double vectorLength(double ax, double ay)
{
    return sqrt(pow(ax, 2)+pow(ay, 2));
}

double distanceLineAndPoint(QLineF l, QPointF p) {
    return abs((l.x2()-l.x1())*(l.y1()-p.y())-(l.x1()-p.x())*(l.y2()-l.y1()))
            /sqrt(pow(l.x2()-l.x1(), 2) + pow(l.y2()-l.y1(),2));
}

// https://stackoverflow.com/questions/6091728/line-segment-circle-intersection
// Distance is mesuared from point a
// This can be interpreted as finding intersection of segment
// and circle of radius of dist
QPointF findPointInDistFromEndOfSegment(QPointF a, QPointF b, double dist) {
    double m = atan2(b.y()-a.y(),b.x()-a.x());
    double x = a.x() + dist*cos(m);
    double y = a.y() + dist*sin(m);
    return QPointF(x, y);

}
// https://stackoverflow.com/questions/49968720/find-tangent-points-in-a-circle-from-a-point
std::tuple<QPointF, QPointF> getTangentPointsFromLineAndCircle(QPointF p, QPointF c, double r) {
    double b = sqrt(pow(p.x() - c.x(),2) + pow(p.y()-c.y(),2));
    double th = acos(r/b);
    double d = atan2(p.y()-c.y(), p.x()-c.x());
    double d1 = d + th;
    double d2 = d - th;
    return std::make_tuple(
                QPointF(c.x()+r*cos(d1), c.y()+r*sin(d1)),
                QPointF(c.x()+r*cos(d2), c.y()+r*sin(d2)));
}

// https://en.wikipedia.org/wiki/Line–line_intersection#Given_two_points_on_each_line
QPointF intersectTwoLines(QPointF A, QPointF B, QPointF C, QPointF D) {
    double denominator = (A.x()-B.x())*(C.y()-D.y()) - (A.y()-B.y())*(C.x()-D.x());
    double x = ((A.x()*B.y() - A.y()*B.x()) * (C.x() - D.x())
                - (A.x() - B.x()) * (C.x()*D.y() - C.y()*D.x()))
                / denominator;
    double y = ((A.x()*B.y() - A.y()*B.x()) * (C.y() - D.y())
                - (A.y() - B.y()) * (C.x()*D.y() - C.y()*D.x()))
                / denominator;
    return QPointF(x, y);
}

double howGoodTwoLinesFit(QPointF A, QPointF B, QPointF C, QPointF D) {
    double first, second;
    if (A.y() < C.y())
        first = C.y();
    else
        first = A.y();

    if (B.y() < D.y())
        second = B.y();
    else
        second = D.y();

    double length = distance(C, D);
    double coverage = (second - first)/length;

    return coverage;
}

