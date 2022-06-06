#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QWidget>
#include <QImage>
#include <QLabel>
#include <QPainter>
#include <QHBoxLayout>
#include "ModelConstants.h"
#include "Player.h"
#include "PlayerAI.h"
#include "World.h"

class RenderArea : public QWidget
{
    Q_OBJECT
private:
    QImage* image = NULL;
    QLabel* label = NULL;
    World* world = NULL;
    PlayerAI playerToDrawRects = PlayerAI();

public:
    RenderArea(QWidget *parent = nullptr, World* _world = nullptr);
    void update();

    void DrawCircle();
    void drawPlayer(Player player);
    void drawIntentions(PlayerAI player);
    void drawField();
    void drawBall(Ball* ball);
    void drawRoleRects(QVector<PlayerRole> roles);
    void drawSpeed(QPainter &painter, BaseEntity entity, bool isBall);
    void drawWorld();

    QRectF getRectFAtCenter(double x, double y, double awidth, double aheight);
    QRectF getRectFCircleAtCenter(double x, double y, double radius);




public slots:
    void setPlayerToDrawRects(int i);
    void setDrawRectsState(int i);
    void setDrawIntentionsState(int i);
    void setIntroduceNoises(int i);
    void setLimitVision(int i);
    void setRandom(int i);
};

#endif // RENDERAREA_H
