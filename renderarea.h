#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QWidget>
#include <QImage>
#include <QLabel>
#include <QPainter>
#include <QHBoxLayout>

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
};

#endif // RENDERAREA_H
