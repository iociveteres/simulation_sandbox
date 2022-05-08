#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include "renderarea.h"

class MainWindow
        :public QWidget
{
private:
    RenderArea* render;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};

#endif // MAINWINDOW_H
