#include "Action.h"

Action::Action()
{

}

PrefferedPoint Action::getPrefferedPoint() const
{
    return prefferedPoint;
}
// check that two same actions are intended against same opponent
// player, point and desirebility may be different by design
bool Action::operator==(const Action &rhs) const
{
    if (actionType != rhs.actionType)
        return false;
    if (againstId != rhs.actionType)
        return false;
    return true;
}

void Action::setPrefferedPoint(const PrefferedPoint &value)
{
    prefferedPoint = value;
}

int Action::getExecutorId() const
{
    return executorId;
}

int Action::getAgainstId() const
{
    return againstId;
}
