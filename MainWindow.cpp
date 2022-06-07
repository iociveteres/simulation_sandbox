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

    interfaceLayout->addLayout(buttonLayout);

    QVBoxLayout* checkboxLayout = new QVBoxLayout(this);

    choosePlayerPersperctiveComboBox = new QComboBox(this);

    checkboxLayout->addWidget(choosePlayerPersperctiveComboBox);
    QCheckBox* addNoisesCheckbox = new QCheckBox("Внесение шумов", this);
    addNoisesCheckbox->setCheckState(Qt::CheckState::Checked);
    checkboxLayout->addWidget(addNoisesCheckbox);
    QCheckBox* limitVisionCheckbox = new QCheckBox("Ограничение поля зрения", this);
    limitVisionCheckbox->setCheckState(Qt::CheckState::Checked);
    checkboxLayout->addWidget(limitVisionCheckbox);
    QCheckBox* randomCheckbox = new QCheckBox("Сид ГСЧ по умолчанию", this);
    randomCheckbox->setCheckState(Qt::CheckState::Checked);
    checkboxLayout->addWidget(randomCheckbox);
    checkboxLayout->addStrut(20);

    QCheckBox* drawRoleRectsCheckbox = new QCheckBox("Зоны ролей", this);
    checkboxLayout->addWidget(drawRoleRectsCheckbox);
    QCheckBox* drawIntentionsCheckbox = new QCheckBox("Отобразить намерения игроков", this);
    checkboxLayout->addWidget(drawIntentionsCheckbox);


    interfaceLayout->addLayout(checkboxLayout);
    // assemble window layout
    layout->addLayout(interfaceLayout);
    setLayout(layout);

    // slots
    connect(loadButton, &QPushButton::clicked, this, &MainWindow::handleLoadButton);
    connect(playButton, &QPushButton::clicked, world, &World::handlePlayButton);

    connect(choosePlayerPersperctiveComboBox, SIGNAL(currentIndexChanged(int)),
            render, SLOT(setPlayerToDrawRects(int)));

    connect(addNoisesCheckbox, SIGNAL(stateChanged(int)),
            render, SLOT(setIntroduceNoises(int)));
    connect(limitVisionCheckbox, SIGNAL(stateChanged(int)),
            render, SLOT(setLimitVision(int)));
    connect(randomCheckbox, SIGNAL(stateChanged(int)),
            render, SLOT(setRandom(int)));

    connect(drawRoleRectsCheckbox, SIGNAL(stateChanged(int)),
            render, SLOT(setDrawRectsState(int)));
    connect(drawIntentionsCheckbox, SIGNAL(stateChanged(int)),
            render, SLOT(setDrawIntentionsState(int)));

    connect(world, &World::updateRequired, this, &MainWindow::updateField);
}

MainWindow::~MainWindow() {

}

void MainWindow::updateField() {
    render->update();
}

void MainWindow::handleLoadButton()
{
    QFileDialog dialog(this);
    QDir dir;
    dialog.setDirectory(dir.currentPath());
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setNameFilter(tr("JSON save (*.json)"));
    dialog.setProperty("selectMultiple", false);
    dialog.setProperty("selectExisting", true);

    QStringList fileNames;
    if (dialog.exec())
        fileNames = dialog.selectedFiles();

    if (!fileNames.empty()) {
        world->loadWorld(World::Json, fileNames.at(0));

        choosePlayerPersperctiveComboBox->clear();
        int i = 0;
        for (Player a: world->getTeamAlly()) {
            choosePlayerPersperctiveComboBox->addItem(QString("Игрок ") + QString::number(i));
            i++;
        }
        render->update();
    }
}
