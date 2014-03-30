#include "memorymanager.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MemoryManager w;
    w.show();

    return a.exec();
}
