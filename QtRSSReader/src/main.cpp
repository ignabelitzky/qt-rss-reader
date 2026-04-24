#include "src/ui/mainwindow.h"

#include <QApplication>
#include "db/databasemanager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if (!DatabaseManager::initialize())
    {
        return -1;
    }

    MainWindow w;
    w.show();
    return QCoreApplication::exec();
}
