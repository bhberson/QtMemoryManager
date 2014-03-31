#include "memorymanager.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MemoryManager window;
    window.show();

    return app.exec();
}
