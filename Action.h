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
    Action(ActionType _actionType, int _executorId, int _againstId);
    bool operator==(const Action &rhs) const;

    PrefferedPoint getPrefferedPoint() const;
    int getExecutorId() const;
    int getAgainstId() const;

    void setPrefferedPoint(const PrefferedPoint &value);


private:
    ActionType actionType;
    int executorId;
    int againstId;
    PrefferedPoint prefferedPoint;

    double desirebility;
};

#endif // ACTION_H
