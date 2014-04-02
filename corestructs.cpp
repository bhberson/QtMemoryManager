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
        pageTable[pageIndex].segmentPageNumber = 0;
        pageTable[pageIndex].pid = -1;
    }
    emit pagesCreated(numPages);
}


void CoreStructs::next(QString command)
{
    emit showMessage("==> " + command);
    QTextStream inputParser(&command, QIODevice::ReadOnly | QIODevice::Text);
    int pid;
    int secondCommand;
    inputParser >> pid >> secondCommand;
    if(inputParser.status() == QTextStream::ReadCorruptData)
    {
        emit showError("Parsing error.", "Error in parsing current line.");
        return;
    }
    if(secondCommand == -1)
        removeProcess(pid);
    else
    {
        Segment seg = {0, secondCommand};
        Process proc;
        proc.pid = pid;
        while(inputParser.status() == QTextStream::Ok)
        {
            proc.segments << seg;
            ++seg.type;
            inputParser >> seg.size;
        }
        insertProcess(proc);
    }
}

void CoreStructs::insertProcess(const Process &process)
{
    int codePages = ceil((double)process.segments[0].size / pageSize);
    int dataPages = ceil((double)process.segments[1].size / pageSize);
    emit showMessage("Loading program " + QString::number(process.pid) + " into RAM: code=" + QString::number(process.segments[0].size) + " (" + QString::number(codePages) + " page" + QString(codePages > 1 ? "s" : "") +  "), data=" + QString::number(process.segments[1].size) + " (" + QString::number(dataPages) + " page" + QString(dataPages > 1 ? "s" : "") +  ")");
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
                    pageTable[pageIndex].segmentPageNumber = segmentPageNumber;
                    pageTable[pageIndex].pid = process.pid;
                    segSize -= pageSize;
                    emit pageInserted(pageIndex, pageTable[pageIndex]);
                    emit showMessage("Load page " + QString::number(pageNum) + " (" + QString(seg.type == Segment::Code ? "C" : "D") + ") of process " + QString::number(process.pid) + " to frame " + QString::number(pageIndex));
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
    emit showMessage("End of Program " + QString::number(pid));
    for(int pageIndex = 0; pageIndex < numPages; ++pageIndex)
    {
        if(pageTable[pageIndex].pid == pid)
        {
            pageTable[pageIndex].pid = -1;
            emit pageRemoved(pageIndex);
        }
    }
}
