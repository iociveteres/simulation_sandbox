#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent, World* world)
    : QWidget(parent)
{
    render = new RenderArea(nullptr, world);
    setFixedSize(1440, 900);

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(render);
    setLayout(layout);
}

MainWindow::~MainWindow()
{
}

