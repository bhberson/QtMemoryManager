#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include <QMainWindow>

namespace Ui {
class MemoryManager;
}

class MemoryManager : public QMainWindow
{
    Q_OBJECT

public:
    explicit MemoryManager(QWidget *parent = 0);
    ~MemoryManager();

private:
    Ui::MemoryManager *ui;
};

#endif // MEMORYMANAGER_H
