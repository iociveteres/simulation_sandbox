#ifndef ACTION_H
#define ACTION_H

#include "PrefferedPoint.h"

class Action
{
public:
    enum ActionType {
        Mark,
        DefendGoal,
        Wait,
        Intercept
    };

    struct desirebilityDescending;
    struct costAscending;

    Action();
    Action(ActionType _actionType, int _executorId,
           int _againstId, double _desirebility, double _cost);
    Action(ActionType _actionType, int _executorId,
           int _againstId, double _desirebility,
           QPointF point, double _cost);
    bool operator==(const Action &rhs) const;

    QPointF getPrefferedPoint() const;
    int getExecutorId() const;
    int getAgainstId() const;

    void setPrefferedPoint(const QPointF &value);

    const static int NoEnemyCode = -1000;
    double getCost() const;
    void setCost(double value);

    double getDesirebility() const;
    void setDesirebility(double value);

    ActionType getActionType() const;

    int getDefendGoalPart() const;
    void setDefendGoalPart(int value);

private:
    ActionType actionType;
    int executorId;
    int againstId;
    QPointF prefferedPoint;
    int defendGoalPart = 0;

    double desirebility;
    double cost;
};

#endif // ACTION_H
