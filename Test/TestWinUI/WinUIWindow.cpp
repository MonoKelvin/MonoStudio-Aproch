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

    // data binding
    ADataWidgetBinding* dwb = new ADataWidgetBinding(this);
    ASpinbBoxBindMethod* spinBoxBinding = new ASpinbBoxBindMethod(dwb);
    ALineEditBindMethod* lineEditBinding = new ALineEditBindMethod(dwb);
    ADataWidgetBinding::addBindMethod<QSpinBox>(spinBoxBinding);
    ADataWidgetBinding::addBindMethod<QLineEdit>(lineEditBinding);

    // data manager
    ADataContainer* dc = new ADataContainer(this);
    AIntegerDataManager* intDM = new AIntegerDataManager(dc);
    AStringDataManager* stringDM = new AStringDataManager(dc);
    ASizeDataManager* sizeDM = new ASizeDataManager(dc);

    // factory
    ASpinBoxFactory* sbf = new ASpinBoxFactory(dwb, this);
    ALineEditFactory* lef = new ALineEditFactory(dwb, this);

    // data
    AData* dt = intDM->addData(tr("test1"));
    dc->setValue(dt, 10);

    QWidget* spinBox001 = sbf->createEditor(dt, this);
    layout()->addWidget(spinBox001);
    QWidget* spinBox002 = sbf->createEditor(dt, this);
    layout()->addWidget(spinBox002);
    QWidget* spinBox003 = sbf->createEditor(dt, this, QString(), EDataBindType::OneWay);
    layout()->addWidget(spinBox003);
    QWidget* spinBox004 = sbf->createEditor(dt, this, QString(), EDataBindType::FirstTime);
    layout()->addWidget(spinBox004);

    AData* strData = stringDM->addData(tr("string"));
    dc->setValue(strData, QString("aproch"));

    QWidget* lineedit001 = lef->createEditor(strData, this);
    layout()->addWidget(lineedit001);
    QWidget* lineedit002 = lef->createEditor(strData, this);
    layout()->addWidget(lineedit002);
}

WinUIWindow::~WinUIWindow()
{
}
