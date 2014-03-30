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

public slots:
    void loadInputFile();
    void next();
    void createPages(int numPages);


private:
    Ui::MemoryManager *ui;

private slots:
    void start();
    void insertPage(int pageIndex, Page page);
    void removePage(int pageIndex);
    void showError(const QString &title, const QString &message);
    void showMessage(const QString &message);


};

#endif // MEMORYMANAGER_H
