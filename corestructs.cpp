#include "corestructs.h"

CoreStructs::CoreStructs(int pageSize, int numPages, QObject *parent) :
    QObject(parent),
    pageSize(pageSize),
    numPages(numPages),
    pageTable(0)

{
}

CoreStructs::~CoreStructs(){
    if(pageTable)
            delete pageTable;
}

void CoreStructs::initPages()
{
    if(pageTable)
        delete pageTable;
    pageTable = new Page[numPages];
    for(int pageIndex = 0; pageIndex < numPages; ++pageIndex)
    {
        pageTable[pageIndex].type = 0;
        pageTable[pageIndex].segmentPageNumber = 0;
    }
    emit pagesCreated(numPages);
}


void CoreStructs::next(QString command)
{
    // Needs to be implemented
}
