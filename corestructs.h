#ifndef CORESTRUCTS_H
#define CORESTRUCTS_H

#include <QObject>

struct Page
{
    int type;
    int segmentPageNumber;
    int pid;
};

struct Segment
{
    enum Type
    {
        Code,
        Data
    };

    int type;
    int size;
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
    void pageInserted(int pageIndex);
    void pageRemoved(int pageIndex);
    void showError(const QString &title, const QString &message);
    void showMessage(const QString &message);


public slots:
    void next(QString command);

private:
    int pageSize;
    int numPages;
    Page *pageTable;

};

#endif // CORESTRUCTS_H
