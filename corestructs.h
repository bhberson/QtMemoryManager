#ifndef CORESTRUCTS_H
#define CORESTRUCTS_H

#include <QObject>

class coreStructs : public QObject
{
    Q_OBJECT
public:
    explicit coreStructs(int pageSize, int numPages, QObject *parent = 0);
    ~coreStructs();

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

};

#endif // CORESTRUCTS_H
