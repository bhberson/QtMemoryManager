#include "memorymanager.h"
#include "ui_memorymanager.h"
#include <QFileDialog>
#include <QString>
#include <QFile>
#include <QLabel>
#include <QMessageBox>
#include <QTextStream>

MemoryManager::MemoryManager(QWidget *parent) :
    QMainWindow(parent),
    cs(new CoreStructs(512, 8, this))
{
    setupUi(this);
    //all UI connections
    connect(actionOpen, SIGNAL(triggered()), this, SLOT(loadInput()));
    connect(nextButton, SIGNAL(clicked()), this, SLOT(start()));
    connect(startButton, SIGNAL(clicked()), this, SLOT(loadInput()));
    connect(cs, SIGNAL(pagesCreated(int)), this, SLOT(createPages(int)));
    connect(cs, SIGNAL(pageInserted(int,Page)), this, SLOT(insertPage(int,Page)));
    connect(cs, SIGNAL(pageRemoved(int)), this, SLOT(removePage(int)));
    connect(cs, SIGNAL(showError(QString,QString)), this, SLOT(showError(QString,QString)));
    connect(cs, SIGNAL(showMessage(QString)), this, SLOT(showMessage(QString)));

    cs->initPages();
}

MemoryManager::~MemoryManager()
{
    delete cs;
}

void MemoryManager::showError(const QString &title, const QString &message)
{
    QMessageBox(QMessageBox::Critical, title, message, QMessageBox::Ok, this).exec();
}

void MemoryManager::showMessage(const QString &message)
{
    outputTextEdit->appendPlainText(message);
}

void MemoryManager::loadInput()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Please select an acceptable file to input", QString(), "*.txt *.dat *.data", 0, QFileDialog::ReadOnly);
    if(fileName.isEmpty())
        return;
    QFile inputFile(fileName);
    if(!inputFile.open(QFile::ReadOnly | QFile::Text))
        return;
    //displays the read in file to the GUI
    QTextStream inputTextStream(&inputFile);
    inputTextEdit->setPlainText(inputTextStream.readAll());
}

void MemoryManager::next()
{
    QList<QTextEdit::ExtraSelection> extraSelections = inputTextEdit->extraSelections();
    extraSelections[0].cursor.movePosition(QTextCursor::Down);
    //Moves line by line to be read
    extraSelections[0].cursor.select(QTextCursor::LineUnderCursor);
    QString selText = extraSelections[0].cursor.selectedText();
    if(selText.isEmpty())
    {
        showMessage("The End");
        disconnect(nextButton, SIGNAL(clicked()), this, SLOT(next()));
        connect(nextButton, SIGNAL(clicked()), this, SLOT(start()));
        return;
    }

    extraSelections[0].cursor.clearSelection();
    inputTextEdit->setExtraSelections(extraSelections);
    //core data struct call
    cs->next(selText);
}

void MemoryManager::createPages(int numPages)
{
    QLayoutItem *child;
    while ((child = pageTableLayout->takeAt(0)) != 0)
       delete child;

    QLabel *label;
    //makes all available gui page tables "Free"
    for(int i = 0; i < numPages; ++i)
    {
        label = new QLabel("Free");
        label->setFrameShape(QLabel::Box);
        label->setFrameShadow(QLabel::Raised);
        label->setAlignment(Qt::AlignCenter);
        pageTableLayout->addWidget(label);
    }
}

void MemoryManager::insertPage(int pageIndex, Page page)
{
    ((QLabel*)pageTableLayout->itemAt(pageIndex)->widget())->setText(QString(page.type == Segment::Text ? "Text" : "Data") + "-" + QString::number(page.segPageNum) + " of P" + QString::number(page.pid));
}

void MemoryManager::removePage(int pageIndex)
{
    ((QLabel*)pageTableLayout->itemAt(pageIndex)->widget())->setText("Free");
}

//created just for the first line to start the process before swtiching to "next"
void MemoryManager::start()
{
    QTextEdit::ExtraSelection selectedLine;
    selectedLine.format.setBackground(QColor(0xcc, 0x88, 0x33, 0xff ));
    selectedLine.format.setProperty(QTextFormat::FullWidthSelection, true);
    selectedLine.cursor = QTextCursor(inputTextEdit->document());
    selectedLine.cursor.setPosition(0);

    QList<QTextEdit::ExtraSelection> extraSelections;
    extraSelections << selectedLine;

    inputTextEdit->setExtraSelections(extraSelections);

    disconnect(nextButton, SIGNAL(clicked()), this, SLOT(start()));
    connect(nextButton, SIGNAL(clicked()), this, SLOT(next()));

    extraSelections[0].cursor.select(QTextCursor::LineUnderCursor);

    cs->next(extraSelections[0].cursor.selectedText());
}
