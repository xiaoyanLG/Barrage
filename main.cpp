#include "mainwindow.h"
#include "qtsingleapplication.h"

int main(int argc, char *argv[])
{
    QtSingleApplication a(argc, argv);
    if (a.sendMessage("Wake"))
    {
        return 0;
    }
    a.setQuitOnLastWindowClosed(false);
    MainWindow w;
    w.setWindowTitle(QObject::tr("Barrage"));
    w.show();
    a.setActivationWindow(&w);
    return a.exec();
}
