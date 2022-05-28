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

    QVBoxLayout* buttonLayout = new QVBoxLayout(this);
    QPushButton* loadButton = new QPushButton("Открыть", this);
    buttonLayout->addWidget(loadButton);
    QPushButton* playButton = new QPushButton("Просчитать", this);
    buttonLayout->addWidget(playButton);
    choosePlayerPersperctiveComboBox = new QComboBox(this);

    interfaceLayout->addLayout(buttonLayout);

    QVBoxLayout* checkboxLayout = new QVBoxLayout(this);
    QCheckBox* drawRoleRectsCheckbox = new QCheckBox("Зоны ролей", this);
    checkboxLayout->addWidget(drawRoleRectsCheckbox);
    QCheckBox* drawGoalDefendPositionsCheckbox = new QCheckBox("Точки защиты ворот", this);
    checkboxLayout->addWidget(drawGoalDefendPositionsCheckbox);
    QCheckBox* drawTangentLinesCheckbox = new QCheckBox("Касательные от мяча", this);
    checkboxLayout->addWidget(drawTangentLinesCheckbox);

    interfaceLayout->addLayout(checkboxLayout);
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

    int i = 0;
    for (Player a: world->getTeamAlly()) {
        choosePlayerPersperctiveComboBox->addItem(QString("Игрок ") + QString(i));
    }
    render->update();
}
