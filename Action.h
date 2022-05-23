#ifndef ACTION_H
#define ACTION_H

#include "PrefferedPoint.h"

class Action
{

public:
    enum ActionType {
        Mark,
        DefendGoal,
        Wait
    };

    Action();
    PrefferedPoint getPrefferedPoint() const;
    bool operator==(const Action &rhs) const;

private:
    ActionType actionType;
    int executorId;
    int againstId;
    PrefferedPoint prefferedPoint;



};

#endif // ACTION_H
