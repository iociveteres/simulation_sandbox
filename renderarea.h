#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QWidget>
#include <QImage>
#include <QLabel>
#include <QPainter>
#include <QHBoxLayout>
#include "ModelConstants.h"
#include "player.h"

class RenderArea : public QWidget
{
    Q_OBJECT
private:
    QImage* image;
    QLabel* label;

public:
    RenderArea(QWidget *parent = nullptr);
    void DrawCircle();
    void DrawPlayer();
    void DrawPlayer(Player player);
    void DrawField();
};

#endif // RENDERAREA_H
