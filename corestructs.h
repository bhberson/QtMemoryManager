#ifndef CORESTRUCTS_H
#define CORESTRUCTS_H

#include <QObject>

struct Segment
{
    enum Type
    {
        Text,
        Data
    };

    int type;
    int size;
};

struct Process
{
    int pid;
    QList<Segment> segments;
};

struct Page
{
    int type;
    int segPageNum;
    int pid;
};

class CoreStructs : public QObject
{
    Q_OBJECT
public:
    explicit CoreStructs(int pageSize, int numPages, QObject *parent = 0);
    ~CoreStructs();
    void initPages();

signals:
    void pagesCreated(int numPages);
    void lineBeingParsed(int lineNumber);
    void pageInserted(int pageIndex, Page page);
    void pageRemoved(int pageIndex);
    void showError(const QString &title, const QString &message);
    void showMessage(const QString &message);

public slots:
    void next(QString command);

private:
    int pageSize;
    int numPages;
    Page *pageTable;
    void insertProcess(const Process &process);
    void removeProcess(int pid);

};

#endif // CORESTRUCTS_H
