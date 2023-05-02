#include "WinUIWindow.h"
#include "AprochAPI.h"

WinUIWindow::WinUIWindow(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    ATextBlock* newBlock = new ATextBlock(AStr("I am a Text"), this);
    layout()->addWidget(newBlock);

    ATextBlock* newBlock2 = new ATextBlock(AStr("Lorem ipsum dolor, sit amet consectetur adipisicing elit."), this);
    layout()->addWidget(newBlock2);
    newBlock2->setText(AStr("Lorem ipsum dolor, sit amet consectetur adipisicing elit."), Qt::ElideRight);

    ATextBox* textBox = new ATextBox(this);
    layout()->addWidget(textBox);

    ATextBox* textBox2 = new ATextBox(this);
    textBox2->setStyleSheet(AStr("border-radius: 7px; border-bottom:4px solid red outset;"));
    layout()->addWidget(textBox2);

    // test data
    ADataContainer* dc = new ADataContainer(this);
    AIntegerDataManager* intDM = new AIntegerDataManager(dc);
    ASizeDataManager* sizeDM = new ASizeDataManager(dc);

    AData* dt = intDM->addData(tr("test1"));

    ASpinBoxFactory* sbf = new ASpinBoxFactory(dc);

    QWidget* spinBox001 = sbf->createEditor(dt, this);
    layout()->addWidget(spinBox001);
    QWidget* spinBox002 = sbf->createEditor(dt, this);
    layout()->addWidget(spinBox002);

    sbf->unbindData(dt, spinBox002);

    QWidget* spinBox003 = sbf->createEditor(dt, this);
    layout()->addWidget(spinBox003);
}

WinUIWindow::~WinUIWindow()
{
}
