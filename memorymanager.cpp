#include "memorymanager.h"
#include "ui_memorymanager.h"

MemoryManager::MemoryManager(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MemoryManager)
{
    ui->setupUi(this);
}

MemoryManager::~MemoryManager()
{
    delete ui;
}
