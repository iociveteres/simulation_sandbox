#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    render = new RenderArea();
    setFixedSize(1440, 900);

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(render);
    setLayout(layout);
}

MainWindow::~MainWindow()
{
}

