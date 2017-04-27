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
    w.show();
    return a.exec();
}
