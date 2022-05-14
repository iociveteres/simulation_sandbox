#include "MainWindow.h"
#include "World.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    bool newWorld = true;
    bool json = true;

    World world;
    if (newWorld)
        world.populate();
    else if (!world.loadWorld(json ? World::Json : World::Binary))
            return 1;

    QTextStream(stdout) << "world ended in the following state:\n";

    if (!world.saveWorld(json ? World::Json : World::Binary))
       QTextStream(stdout) << "Succesfull save\n";

    QApplication a(argc, argv);
    MainWindow w(nullptr, &world);

    w.show();
    return a.exec();
}
