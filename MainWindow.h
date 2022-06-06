#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QFileDialog>
#include <QCheckBox>
#include <QComboBox>
#include "World.h"
#include "RenderArea.h"

class MainWindow
        :public QWidget
{
private:
    RenderArea* render = NULL;
    World* world = NULL;
    QComboBox* choosePlayerPersperctiveComboBox = NULL;

private slots:
    void handleLoadButton();
    void updateField();

public:
    MainWindow(QWidget *parent = nullptr);
    MainWindow(QWidget *parent, World *world);
    ~MainWindow();;
};

#endif // MAINWINDOW_H
