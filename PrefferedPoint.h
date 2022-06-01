#ifndef PREFFEREDPOINT_H
#define PREFFEREDPOINT_H

#include <QPointF>
// needed to overload equal sign
class PrefferedPoint : public QPointF
{
    //using QPointF::QPointF;
public:
    PrefferedPoint();
    PrefferedPoint(qreal xpos, qreal ypos);

};

#endif // PREFFEREDPOINT_H
