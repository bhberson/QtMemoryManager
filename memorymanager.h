#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include <QMainWindow>
#include "ui_memorymanager.h"
#include "corestructs.h"


namespace Ui {
class MemoryManager;
}

class MemoryManager : public QMainWindow, private Ui::MemoryManager
{
    Q_OBJECT

public:
    explicit MemoryManager(QWidget *parent = 0);
    ~MemoryManager();

public slots:
    void loadInput();
    void next();
    void createPages(int numPages);


private:
    CoreStructs *cs;
private slots:
    void start();
    void insertPage(int pageIndex, Page page);
    void removePage(int pageIndex);
    void showError(const QString &title, const QString &message);
    void showMessage(const QString &message);


};

#endif // MEMORYMANAGER_H
