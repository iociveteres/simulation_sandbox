#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent, World* _world)
    : QWidget(parent)
{
    world = _world;
    render = new RenderArea(nullptr, _world);
    setFixedSize(1440, 850);
    // window layout
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(5,5,5,5);
    layout->addWidget(render);
    // right side interface layout
    QVBoxLayout* interfaceLayout = new QVBoxLayout(this);

    QPushButton* loadButton = new QPushButton("Открыть", this);
    interfaceLayout->addWidget(loadButton);
    QPushButton* playButton = new QPushButton("Просчитать", this);
    interfaceLayout->addWidget(playButton);

    // assemble window layout
    layout->addLayout(interfaceLayout);
    setLayout(layout);

    // slots
    connect(loadButton, &QPushButton::clicked, this, &MainWindow::handleLoadButton);
    connect(playButton, &QPushButton::clicked, world, &World::handlePlayButton);
}

MainWindow::~MainWindow()
{
}

void MainWindow::handleLoadButton()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setNameFilter(tr("JSON save (*.json)"));
    dialog.setProperty("selectMultiple", false);
    dialog.setProperty("selectExisting", true);

    QStringList fileNames;
    if (dialog.exec())
        fileNames = dialog.selectedFiles();

    world->loadWorld(World::Json, fileNames.at(0));
    render->update();
}
