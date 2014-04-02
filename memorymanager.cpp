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
    connect(actionOpen, SIGNAL(triggered()), this, SLOT(loadInput()));
    connect(nextButton, SIGNAL(clicked()), this, SLOT(start()));
    connect(startButton, SIGNAL(clicked()), this, SLOT(loadInput()));
    connect(cs, SIGNAL(pagesCreated(int)), this, SLOT(createPages(int)));
    connect(cs, SIGNAL(showError(QString,QString)), this, SLOT(showError(QString,QString)));
    connect(cs, SIGNAL(showMessage(QString)), this, SLOT(showMessage(QString)));
    connect(cs, SIGNAL(pageInserted(int,Page)), this, SLOT(insertPage(int,Page)));
    connect(cs, SIGNAL(pageRemoved(int)), this, SLOT(removePage(int)));
    cs->initPages();
}

MemoryManager::~MemoryManager()
{
    delete cs;
}


void MemoryManager::loadInput()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Please select an acceptable file to input", QString(), "*.txt *.dat *.data", 0, QFileDialog::ReadOnly);
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
    QList<QTextEdit::ExtraSelection> extraSelections = inputTextEdit->extraSelections();
    extraSelections[0].cursor.movePosition(QTextCursor::Down);

    extraSelections[0].cursor.select(QTextCursor::LineUnderCursor);
    QString selText = extraSelections[0].cursor.selectedText();
    if(selText.isEmpty())
    {
        showMessage("End of simulation");
        disconnect(nextButton, SIGNAL(clicked()), this, SLOT(next()));
        return;
    }

    // Render highlighted line
    extraSelections[0].cursor.clearSelection();
    inputTextEdit->setExtraSelections(extraSelections);

    // Tell the memory manager to step
    cs->next(selText);
}

void MemoryManager::createPages(int numPages)
{
    QLayoutItem *child;
    while ((child = pageTableLayout->takeAt(0)) != 0)
       delete child;

    QLabel *label;
    for(int i = 0; i < numPages; ++i)
    {
        label = new QLabel("Free");
        label->setFrameShape(QLabel::Box);
        label->setFrameShadow(QLabel::Raised);
        label->setAlignment(Qt::AlignCenter);
        pageTableLayout->addWidget(label);
    }
}

void MemoryManager::start()
{
    QTextEdit::ExtraSelection selection;
    selection.format.setBackground(QColor(0xc0,0xff,0xc0,0x80));
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.cursor = QTextCursor(inputTextEdit->document());
    selection.cursor.setPosition(0);

    QList<QTextEdit::ExtraSelection> extraSelections;
    extraSelections << selection;

    inputTextEdit->setExtraSelections(extraSelections);

        // Change connections
    disconnect(nextButton, SIGNAL(clicked()), this, SLOT(start()));
    connect(nextButton, SIGNAL(clicked()), this, SLOT(next()));

        // Change button text
    nextButton->setText("&Next");

        // Select the current line
    extraSelections[0].cursor.select(QTextCursor::LineUnderCursor);

        // Tell the memory manager to execute the first step
    cs->next(extraSelections[0].cursor.selectedText());
}

void MemoryManager::insertPage(int pageIndex, Page page)
{
    ((QLabel*)pageTableLayout->itemAt(pageIndex)->widget())->setText(QString(page.type == Segment::Code ? "Code" : "Data") + "-" + QString::number(page.segmentPageNumber) + " of P" + QString::number(page.pid));
}

void MemoryManager::removePage(int pageIndex)
{
    ((QLabel*)pageTableLayout->itemAt(pageIndex)->widget())->setText("Free");
}

void MemoryManager::showError(const QString &title, const QString &message)
{
    QMessageBox(QMessageBox::Critical, title, message, QMessageBox::Ok, this).exec();
}

void MemoryManager::showMessage(const QString &message)
{
    outputTextEdit->appendPlainText(message);
}
