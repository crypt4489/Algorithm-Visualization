#include "window.h"
#include <displayarea.h>
#include <sortingmanager.h>
#include <QApplication>
#include <QThread>
#

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Window w;
    w.show();
    return a.exec();
}
