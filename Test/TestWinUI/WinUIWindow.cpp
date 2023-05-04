#include "WinUIWindow.h"
#include "AprochAPI.h"

#include <QSpinBox>

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

    // data manager
    ADataContainer* dc = new ADataContainer(this);
    AIntegerDataManager* intDM = new AIntegerDataManager(dc);
    ADoubleDataManager* dblDM = new ADoubleDataManager(dc);
    AStringDataManager* stringDM = new AStringDataManager(dc);
    ASizeDataManager* sizeDM = new ASizeDataManager(dc);

    // int
    AData* dt = intDM->addData(tr("int"));
    dc->setValue(dt, 10);

    QWidget* spinBox001 = new QSpinBox(this);
    layout()->addWidget(spinBox001);
    QWidget* spinBox002 = new QSpinBox(this);
    layout()->addWidget(spinBox002);
    QWidget* spinBox003 = new QSpinBox(this);
    layout()->addWidget(spinBox003);
    QWidget* spinBox004 = new QSpinBox(this);
    layout()->addWidget(spinBox004);

    dwb->bind(ADWBindParameter(dt, spinBox001, QString(), EDataBindType::OneWay));
    dwb->bind(ADWBindParameter(dt, spinBox002, QString(), EDataBindType::TwoWay));
    dwb->bind(ADWBindParameter(dt, spinBox003, QString(), EDataBindType::OneWayRevise));
    dwb->bind(ADWBindParameter(dt, spinBox004, QString(), EDataBindType::FirstTime));

    // string
    AData* strData = stringDM->addData(tr("string"));
    dc->setValue(strData, QString("aproch"));

    QWidget* lineedit001 = new QLineEdit(this);
    layout()->addWidget(lineedit001);
    QWidget* lineedit002 = new QLineEdit(this);
    layout()->addWidget(lineedit002);

    dwb->bind(ADWBindParameter(strData, lineedit001, QString(), EDataBindType::OneWay));
    dwb->bind(ADWBindParameter(strData, lineedit002, QString(), EDataBindType::TwoWay));

    // double
    AData* dblData = dblDM->addData(tr("double"));
    dc->setValue(dblData, 3.14);

    QWidget* dsb001 = new QDoubleSpinBox(this);
    layout()->addWidget(dsb001);
    QWidget* dsb002 = new QDoubleSpinBox(this);
    layout()->addWidget(dsb002);

    dwb->bind(ADWBindParameter(dblData, dsb001, QString(), EDataBindType::OneWay));
    dwb->bind(ADWBindParameter(dblData, dsb002, QString(), EDataBindType::TwoWay));
}

WinUIWindow::~WinUIWindow()
{
}
