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
    QImage* image;
    QLabel* label;
    World* world;

public:
    RenderArea(QWidget *parent = nullptr, World* _world = nullptr);
    void update();

    void DrawCircle();
    void drawPlayer(Player player);
    void drawIntentions(PlayerAI player);
    void drawField();
    void drawBall(Ball* ball);
    void drawRoleRects(QVector<PlayerRole> roles);

    void drawWorld(bool drawRoleRects);

    QRectF getRectFAtCenter(double x, double y, double awidth, double aheight);
    QRectF getRectFCircleAtCenter(double x, double y, double radius);


};

#endif // RENDERAREA_H
