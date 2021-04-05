#include "clipboardtracker.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ClipboardTracker w;
    w.show();
    return a.exec();
}
