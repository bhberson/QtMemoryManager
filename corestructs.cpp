#include "corestructs.h"
#include <cmath>
#include <QTextStream>
#include <QDebug>

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
        pageTable[pageIndex].segPageNum = 0;
        pageTable[pageIndex].pid = -1;
    }
    emit pagesCreated(numPages);
}

void CoreStructs::next(QString command)
{
    emit showMessage("~~~$ " + command);
    QStringList commandList = command.split(" ");
    int pid = commandList[0].toInt();

    if(commandList[1] == "Halt")
        removeProcess(pid);
    else
    {
        int secondCommand = commandList[1].toInt();
        Segment seg = {0, secondCommand};
        Process proc;
        proc.pid = pid;
        if (commandList.size() > 2)
        {
            proc.segments << seg;
            ++seg.type;
            seg.size = commandList[2].toInt();
            proc.segments << seg;
        }
        insertProcess(proc);
    }
}

void CoreStructs::insertProcess(const Process &process)
{
    int textPages = ceil((double)process.segments[0].size / pageSize);
    int dataPages = ceil((double)process.segments[1].size / pageSize);
    emit showMessage("Loading program " + QString::number(process.pid) + " into RAM: text=" + QString::number(process.segments[0].size) + " (" + QString::number(textPages) + " page" + QString(textPages > 1 ? "s" : "") +  "), data=" + QString::number(process.segments[1].size) + " (" + QString::number(dataPages) + " page" + QString(dataPages > 1 ? "s" : "") +  ")");
    int segSize;
    int pageNum = 0;
    foreach(const Segment &seg, process.segments)
    {
        segSize = seg.size;
        int segmentPageNumber = 0;
        int pageIndex = 0;
        while(segSize > 0)
        {
            for( ; pageIndex < numPages; ++pageIndex)
            {
                if(pageTable[pageIndex].pid == -1)
                {
                    pageTable[pageIndex].type = seg.type;
                    pageTable[pageIndex].segPageNum = segmentPageNumber;
                    pageTable[pageIndex].pid = process.pid;
                    segSize -= pageSize;
                    emit pageInserted(pageIndex, pageTable[pageIndex]);
                    emit showMessage("Load page " + QString::number(pageNum) + " (" + QString(seg.type == Segment::Text ? "T" : "D") + ") of process " + QString::number(process.pid) + " to frame " + QString::number(pageIndex));
                    ++segmentPageNumber;
                    ++pageNum;
                    break;
                }
            }
        }
    }
}

void CoreStructs::removeProcess(int pid)
{
    emit showMessage("End of Process " + QString::number(pid));
    for(int pageIndex = 0; pageIndex < numPages; ++pageIndex)
    {
        if(pageTable[pageIndex].pid == pid)
        {
            pageTable[pageIndex].pid = -1;
            emit pageRemoved(pageIndex);
        }
    }
}
