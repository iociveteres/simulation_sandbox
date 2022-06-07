#ifndef PLAYERAI_H
#define PLAYERAI_H

#include "Player.h"
#include "PlayerWorldModel.h"
#include "Geometry.h"
#include "Globals.h"
#include <tuple>
#include <memory>
// Класс PLayerAI реализует "умное" поведение агента
// через построение модели окружающего мира и
// определение оптимальных действий на основе собранной
// информации
class PlayerAI:
        public Player
{
    Q_GADGET;

private:
    // текущий отступ от левого края поля до левого края построения
    double roleMargin;
    // выбранное действие игрока
    Action intention;
    // модель мира игрока
    PlayerWorldModel *worldModel = NULL;
    // ссылка на объект, хранящий состояние мира
    // необходим для построения модели мира
    World *worldLink = NULL;

public:
    // конструкторы
    PlayerAI();
    PlayerAI(Team team, World *_world);
    PlayerAI(Team _team, int _x, int _y, double _angle, World *_world);
    // получение текущего отступа от левого края поля
    // для роли игрока
    void cycle();
    // установка роли
    void setRole(PlayerRole role);
    // получить прямоугольник в координатах окна отрисовки,
    // обозначающий зону, в которой игрок может ударить мяч
    QRectF getIntentionsKickableAreaRect();

    static int getPlayerCount();
    Action getIntention() const;
    PlayerWorldModel *getWorldModel() const;
    double getRoleMargin() const;

    // создание списков действий, доступных для
    // союзников,
    QMap<int, QList<Action> > makePrefferedActionsListOthers();
    // самого игрока,
    QList<Action> makePrefferedActionsListMe();
    // игрока с точки зрения соседних по построению ролей
    QList<Action> makePrefferedActionsListNeighbours();
    // определение предпочтительного действия
    Action determinePrefferedIntention();

    // вычисление эвристик и создание действий
    // опеки
    Action checkMarking(Player enemy, Player player);
    // защиты ворот
    Action checkDefendGoal(Player player);
    // перехвата мяча
    Action checkIntercept(Player player);
    // ожидания в защите
    Action checkWaitDefensive(Player player);

    // поиск точки для опеки соперника
    QPointF getPointMarking(Action a, Player player);

    // защита ворот
    // определение зоны для защиты ворот относительно положения мяча
    QRectF findDefendGoalArea();
    // вычисление уровня закрытия ворот для отдельной точки
    double getGoalCoverageRatingFromPlayerPos(QPointF playerPos);
    // определение зоны для защиты ворот относительно положения мяча
    std::tuple<QPointF, QPointF> getPointsDefendGoal(Player player);

    // определение возможности и поиск оптимальной точки для перехвата мяча
    std::tuple<bool, QPointF> getPointIntercept(Player player);

    // получене точки для ожидания
    QPointF getPointWait(Player player);
    // сравнение позиции союзника с той, которая должна быть по построению
    bool checkAllyIsInPlace(PlayerRole::RoleName roleName);    
private:
    Team team;
};

#endif // PLAYERAI_H
