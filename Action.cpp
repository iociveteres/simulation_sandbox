#include "Action.h"

Action::Action()
{

}

Action::Action(ActionType _actionType, int _executorId, int _againstId, double _desirebility)
{
    actionType = _actionType;
    executorId = _executorId;
    againstId = _againstId;
    desirebility = _desirebility;
}

// check that two same actions are intended against same opponent
// player, point and desirebility may be different by design
bool Action::operator==(const Action &rhs) const
{
    if (actionType == ActionType::DefendGoal)
        if (getPrefferedPoint() != rhs.getPrefferedPoint())
            return false;
    if (actionType != rhs.actionType)
        return false;
    if (againstId != -1000)
        if (againstId != rhs.againstId)
            return false;

    return true;
}

QPointF Action::getPrefferedPoint() const
{
    return prefferedPoint;
}

void Action::setPrefferedPoint(const QPointF &value)
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
