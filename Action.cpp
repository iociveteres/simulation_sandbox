#include "Action.h"

Action::Action()
{
    int a;
}

Action::Action(ActionType _actionType, int _executorId,
               int _againstId, double _desirebility, double _cost)
{
    actionType = _actionType;
    executorId = _executorId;
    againstId = _againstId;
    desirebility = _desirebility;
    cost = _cost;
}

Action::Action(ActionType _actionType, int _executorId,
               int _againstId, double _desirebility,
               QPointF point, double _cost)
{
    actionType = _actionType;
    executorId = _executorId;
    againstId = _againstId;
    desirebility = _desirebility;
    prefferedPoint = point;
    cost = _cost;
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

double Action::getCost() const
{
    return cost;
}

void Action::setCost(double value)
{
    cost = value;
}

double Action::getDesirebility() const
{
    return desirebility;
}

void Action::setDesirebility(double value)
{
    desirebility = value;
}

int Action::getExecutorId() const
{
    return executorId;
}

int Action::getAgainstId() const
{
    return againstId;
}
