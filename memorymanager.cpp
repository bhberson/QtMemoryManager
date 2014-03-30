#include "memorymanager.h"
#include "ui_memorymanager.h"

MemoryManager::MemoryManager(QWidget *parent) :
    QMainWindow(parent),
    ui(new coreStructs(512, 8, this))
{
    ui->setupUi(this);
}

MemoryManager::~MemoryManager()
{
    delete ui;
}


void MemoryManager::loadInput()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Please select an acceptable file to input", QString(), "*.txt *.dat", 0, QFileDialog::ReadOnly);
    if(fileName.isEmpty())
        return;
    QFile inputFile(filNeame);
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

void MemoryManager::insertPage(int pageIndex, Page page)
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
