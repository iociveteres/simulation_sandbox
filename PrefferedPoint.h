#ifndef PREFFEREDPOINT_H
#define PREFFEREDPOINT_H

#include <QPointF>

class PrefferedPoint : public QPointF
{
    //using QPointF::QPointF;
public:
    PrefferedPoint(qreal xpos, qreal ypos);
};

#endif // PREFFEREDPOINT_H
