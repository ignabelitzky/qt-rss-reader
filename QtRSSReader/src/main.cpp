#include "src/ui/mainwindow.h"

#include <QApplication>
#include <QFile>
#include "db/databasemanager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if (!DatabaseManager::initialize())
    {
        return -1;
    }

    QFile file(":/styles/style.qss");
    if (file.open(QFile::ReadOnly))
    {
        QString style = file.readAll();
        a.setStyleSheet(style);
    }

    MainWindow w;
    w.show();
    return QCoreApplication::exec();
}
