#include "MainWindow.h"
#include "World.h"

#include <QApplication>
#include <QDir>

int main(int argc, char *argv[])
{
    //    QTextStream(stdout) << "Succesfull save\n";
    bool newWorld = false;
    bool json = true;

    QDir dir;
    qDebug() << "Current dir: " << dir.currentPath() << "\n";

    World world;
    if (newWorld)
        world.populate();
    else if (!world.loadWorld(json ? World::Json : World::Binary))
            return 1;

    if (!newWorld)
        if (!world.saveWorld(json ? World::Json : World::Binary))
            ;

    QApplication a(argc, argv);
    MainWindow w(nullptr, &world);
    qDebug() << "App path : " << a.applicationDirPath();

    w.show();
    return a.exec();
}
