#include "memorymanager.h"
#include "ui_memorymanager.h"
#include <QFileDialog>
#include <QString>
#include <QFile>
#include <QTextStream>

MemoryManager::MemoryManager(QWidget *parent) :
    QMainWindow(parent),
    cs(new CoreStructs(512, 8, this))
{
    setupUi(this);
    connect(actionOpen, SIGNAL(triggered()), this, SLOT(loadInput()));
    connect(nextButton, SIGNAL(clicked()), this, SLOT(start()));
    connect(startButton, SIGNAL(clicked()), this, SLOT(loadInput()));
}

MemoryManager::~MemoryManager()
{
    delete cs;
}


void MemoryManager::loadInput()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Please select an acceptable file to input", QString(), "*.txt *.dat", 0, QFileDialog::ReadOnly);
    if(fileName.isEmpty())
        return;
    QFile inputFile(fileName);
    if(!inputFile.open(QFile::ReadOnly | QFile::Text))
        return;

    QTextStream inputTextStream(&inputFile);
    inputTextEdit->setPlainText(inputTextStream.readAll());
}
void MemoryManager::next()
{
    // Needs to Be Implemented
}

void MemoryManager::createPages(int numPages)
{
    // Needs to Be Implemented
}

void MemoryManager::start()
{
    // Needs to Be Implemented
}

void MemoryManager::insertPage(int pageIndex)
{
    // Needs to Be Implemented
}

void MemoryManager::removePage(int pageIndex)
{
    // Needs to Be Implemented
}

void MemoryManager::showError(const QString &title, const QString &message)
{
    // Needs to Be Implemented
}

void MemoryManager::showMessage(const QString &message)
{
    // Needs to Be Implemented
}
