#include "corestructs.h"

coreStructs::coreStructs(int pageSize, int numPages, QObject *parent) :
    QObject(parent),
    pageSize(pageSize),
    numPages(numPages)
{
}

coreStructs::~coreStructs(){

}

void coreStructs::next(QString command)
{
    // Needs to be implemented
}
