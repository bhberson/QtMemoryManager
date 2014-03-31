#include "corestructs.h"

CoreStructs::CoreStructs(int pageSize, int numPages, QObject *parent) :
    QObject(parent),
    pageSize(pageSize),
    numPages(numPages)
{
}

CoreStructs::~CoreStructs(){

}

void CoreStructs::next(QString command)
{
    // Needs to be implemented
}
