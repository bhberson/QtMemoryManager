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
        proc.segments << seg;
        ++seg.type;
        seg.size = commandList[2].toInt();
        proc.segments << seg;
        insertProcess(proc);
    }
}

int CoreStructs::calcSize(double size)
{
    return ceil(size/pageSize);
}
QString CoreStructs::processMessage(const Process &process)
{
    int textPages = calcSize((double)process.segments[0].size);
    int dataPages = calcSize((double)process.segments[1].size);
    QString message;
    message.append("Loading process ");
    message.append(QString::number(process.pid));
    message.append(" into M: textSeg=");
    message.append(QString::number(process.segments[0].size));
    message.append(" (");
    message.append(QString::number(textPages));
    message.append(" pages), dataSeg=");
    message.append(QString::number(process.segments[1].size));
    message.append(" (");
    message.append(QString::number(dataPages));
    message.append(" pages)");
    return message;
}

QString CoreStructs::pageMessage(int pageNum, int type, int pid, int pageIndex)
{
    QString message;
    message.append("Load page ");
    message.append(QString::number(pageNum));
    message.append(" (");
    message.append(QString(type == Segment::Text ? "Text" : "Data"));
    message.append(") of process ");
    message.append(QString::number(pid));
    message.append(" to frame ");
    message.append(QString::number(pageIndex));
    return message;
}

void CoreStructs::insertProcess(const Process &process)
{
    int segSize;
    int pageNum = 0;

    emit showMessage(processMessage(process));

    foreach(const Segment &seg, process.segments)
    {
        segSize = seg.size;
        int segPageNum = 0;
        int pageIndex = 0;
        while(segSize > 0)
        {
            for( ; pageIndex < numPages; ++pageIndex)
            {
                if(pageTable[pageIndex].pid == -1)
                {
                    pageTable[pageIndex].type = seg.type;
                    pageTable[pageIndex].segPageNum = segPageNum;
                    pageTable[pageIndex].pid = process.pid;
                    segSize -= pageSize;
                    emit pageInserted(pageIndex, pageTable[pageIndex]);
                    emit showMessage(pageMessage(pageNum, seg.type, process.pid, pageIndex));
                    ++segPageNum;
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
