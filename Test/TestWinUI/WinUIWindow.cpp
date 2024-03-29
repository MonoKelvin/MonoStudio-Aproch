#include "WinUIWindow.h"
#include "AprochAPI.h"

#include <QRandomGenerator>
#include <QSpinBox>
#include <QCheckBox>
#include <QRadioButton>
#include <QPushButton>
#include <QComboBox>

#include <QApplication>
#include <QClipboard>
//#include <QProcess>

WinUIWindow::WinUIWindow(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    QLineEdit* le111 = new QLineEdit(this);
    le111->setPlaceholderText(AStr("placeholder text"));
    layout()->addWidget(le111);

    ATextBox* tb111 = new ATextBox(this);
    tb111->setPlaceholderText(AStr("placeholder text"));
    layout()->addWidget(tb111);

    if (0)
    {
        new QBoxLayout(QBoxLayout::TopToBottom, this);
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
    }

    if(0){
        AIntDataManager* IntDM = new AIntDataManager(this);
        AData* intData = IntDM->addData(AStr("name"));
        IntDM->setValue(intData, 10);
        IntDM->setRange(intData, -10, 20);

        QBoxLayout* widgetLayout = new QBoxLayout(QBoxLayout::LeftToRight);
        QWidget* spinBox001 = new QSpinBox(this);
        QWidget* spinBox002 = new QSpinBox(this);
        QWidget* spinBox003 = new QSpinBox(this);
        QWidget* spinBox004 = new QSpinBox(this);
        QWidget* spinBox005 = new QSpinBox(this);
        widgetLayout->addWidget(spinBox001);
        widgetLayout->addWidget(spinBox002);
        widgetLayout->addWidget(spinBox003);
        widgetLayout->addWidget(spinBox004);
        widgetLayout->addWidget(spinBox005);
        layout()->addItem(widgetLayout);

        ASpinBoxBindMethod::addBind(ADWBindParameter(intData, spinBox001, QString(), EDataBindType::FirstTime));
        ASpinBoxBindMethod::addBind(ADWBindParameter(intData, spinBox002, QString(), EDataBindType::OneWay));
        ASpinBoxBindMethod::addBind(ADWBindParameter(intData, spinBox003, QString(), EDataBindType::OneWayRevise));
        ASpinBoxBindMethod::addBind(ADWBindParameter(intData, spinBox004, QString(), EDataBindType::TwoWay));
        ASpinBoxBindMethod::addBind(ADWBindParameter(intData, spinBox005, QString(), EDataBindType::TwoWay));
    }

    if(0)
    {
        ADoubleDataManager* DoubleDM = new ADoubleDataManager(this);
        AData* dblData = DoubleDM->addData(AStr("name"));
        DoubleDM->setValue(dblData, 3.14);
        DoubleDM->setRange(dblData, -8.0, 16.0);

        QBoxLayout* widgetLayout = new QBoxLayout(QBoxLayout::LeftToRight);
        QWidget* spinBox001 = new QDoubleSpinBox(this);
        QWidget* spinBox002 = new QDoubleSpinBox(this);
        QWidget* spinBox003 = new QDoubleSpinBox(this);
        QWidget* spinBox004 = new QDoubleSpinBox(this);
        QWidget* spinBox005 = new QDoubleSpinBox(this);
        widgetLayout->addWidget(spinBox001);
        widgetLayout->addWidget(spinBox002);
        widgetLayout->addWidget(spinBox003);
        widgetLayout->addWidget(spinBox004);
        widgetLayout->addWidget(spinBox005);
        layout()->addItem(widgetLayout);

        ASpinBoxBindMethod::addBind(ADWBindParameter(dblData, spinBox001, QString(), EDataBindType::FirstTime));
        ASpinBoxBindMethod::addBind(ADWBindParameter(dblData, spinBox002, QString(), EDataBindType::OneWay));
        ASpinBoxBindMethod::addBind(ADWBindParameter(dblData, spinBox003, QString(), EDataBindType::OneWayRevise));
        ASpinBoxBindMethod::addBind(ADWBindParameter(dblData, spinBox004, QString(), EDataBindType::TwoWay));
        ASpinBoxBindMethod::addBind(ADWBindParameter(dblData, spinBox005, QString(), EDataBindType::TwoWay));
    }
    
    if(0)
    {
        AStringDataManager* StringDM = new AStringDataManager(this);
        AData* dt = StringDM->addData(AStr("name"));
        StringDM->setValue(dt, AStr("string"));

        QBoxLayout* widgetLayout = new QBoxLayout(QBoxLayout::LeftToRight);
        QWidget* widget001 = new QLineEdit(this);
        QWidget* widget002 = new QLineEdit(this);
        QWidget* widget003 = new QLineEdit(this);
        QWidget* widget004 = new QLineEdit(this);
        QWidget* widget005 = new QLineEdit(this);
        widgetLayout->addWidget(widget001);
        widgetLayout->addWidget(widget002);
        widgetLayout->addWidget(widget003);
        widgetLayout->addWidget(widget004);
        widgetLayout->addWidget(widget005);
        layout()->addItem(widgetLayout);

        ASpinBoxBindMethod::addBind(ADWBindParameter(dt, widget001, QString(), EDataBindType::FirstTime));
        ASpinBoxBindMethod::addBind(ADWBindParameter(dt, widget002, QString(), EDataBindType::OneWay));
        ASpinBoxBindMethod::addBind(ADWBindParameter(dt, widget003, QString(), EDataBindType::OneWayRevise));
        ASpinBoxBindMethod::addBind(ADWBindParameter(dt, widget004, QString(), EDataBindType::TwoWay));
        ASpinBoxBindMethod::addBind(ADWBindParameter(dt, widget005, QString(), EDataBindType::TwoWay));
    }

    if (0)
    {
        ABoolDataManager* BoolDM = new ABoolDataManager(this);
        AData* dt = BoolDM->addData(AStr("name"));
        BoolDM->setValue(dt, true);

        QBoxLayout* widgetLayout = new QBoxLayout(QBoxLayout::LeftToRight);
        QWidget* widget001 = new QCheckBox(this);
        QWidget* widget002 = new QCheckBox(this);
        QWidget* widget003 = new QCheckBox(this);
        QWidget* widget004 = new QCheckBox(this);
        QWidget* widget005 = new QCheckBox(this);
        widgetLayout->addWidget(widget001);
        widgetLayout->addWidget(widget002);
        widgetLayout->addWidget(widget003);
        widgetLayout->addWidget(widget004);
        widgetLayout->addWidget(widget005);
        layout()->addItem(widgetLayout);

        ASpinBoxBindMethod::addBind(ADWBindParameter(dt, widget001, QString(), EDataBindType::FirstTime));
        ASpinBoxBindMethod::addBind(ADWBindParameter(dt, widget002, QString(), EDataBindType::OneWay));
        ASpinBoxBindMethod::addBind(ADWBindParameter(dt, widget003, QString(), EDataBindType::OneWayRevise));
        ASpinBoxBindMethod::addBind(ADWBindParameter(dt, widget004, QString(), EDataBindType::TwoWay));
        ASpinBoxBindMethod::addBind(ADWBindParameter(dt, widget005, QString(), EDataBindType::TwoWay));
    }

    if (0)
    {
        ABoolDataManager* BoolDM = new ABoolDataManager(this);
        AData* dt = BoolDM->addData(AStr("name"));
        BoolDM->setValue(dt, true);

        QBoxLayout* widgetLayout = new QBoxLayout(QBoxLayout::LeftToRight);
        QWidget* widget001 = new QRadioButton(this);
        QWidget* widget002 = new QRadioButton(this);
        QWidget* widget003 = new QRadioButton(this);
        QWidget* widget004 = new QRadioButton(this);
        QWidget* widget005 = new QRadioButton(this);
        widgetLayout->addWidget(widget001);
        widgetLayout->addWidget(widget002);
        widgetLayout->addWidget(widget003);
        widgetLayout->addWidget(widget004);
        widgetLayout->addWidget(widget005);
        layout()->addItem(widgetLayout);

        ASpinBoxBindMethod::addBind(ADWBindParameter(dt, widget001, QString(), EDataBindType::FirstTime));
        ASpinBoxBindMethod::addBind(ADWBindParameter(dt, widget002, QString(), EDataBindType::OneWay));
        ASpinBoxBindMethod::addBind(ADWBindParameter(dt, widget003, QString(), EDataBindType::OneWayRevise));
        ASpinBoxBindMethod::addBind(ADWBindParameter(dt, widget004, QString(), EDataBindType::TwoWay));
        ASpinBoxBindMethod::addBind(ADWBindParameter(dt, widget005, QString(), EDataBindType::TwoWay));
    }

    if (0)
    {
        AIntDataManager* IntDM = new AIntDataManager(this);
        AStringListDataManager* StringListDM = new AStringListDataManager(this);
        AData* dt = StringListDM->addData(AStr("name"));
        StringListDM->setValue(dt, QStringList({ "Apple", "Orange", "Banana", "Grape", "Watermelon" }));

        AData* currentIndexDt = IntDM->addData(AStr("currentIndex"));

        QPushButton* changeItemsBtn = new QPushButton(tr("ChangeItems"), this);
        connect(changeItemsBtn, &QPushButton::clicked, [=](bool) {
            QStringList strlist = StringListDM->value(dt);
            if (strlist.empty())
                strlist.push_back("Apple");
            else if (strlist.size() >= 10)
                strlist.pop_back();
            else
            {
                if (QRandomGenerator::global()->generate() % 2)
                    strlist.push_back(AStr("New Item %1").arg(QRandomGenerator::global()->generate()));
                else
                    strlist.pop_back();
            }
            StringListDM->setValue(dt, strlist);
        });

        QBoxLayout* widgetLayout = new QBoxLayout(QBoxLayout::LeftToRight);
        QWidget* widget001 = new QComboBox(this);
        QWidget* widget002 = new QComboBox(this);
        QWidget* widget003 = new QComboBox(this);
        QWidget* widget004 = new QComboBox(this);
        QWidget* widget005 = new QComboBox(this);
        widgetLayout->addWidget(widget001);
        widgetLayout->addWidget(widget002);
        widgetLayout->addWidget(widget003);
        widgetLayout->addWidget(widget004);
        widgetLayout->addWidget(widget005);
        layout()->addItem(widgetLayout);

        ASpinBoxBindMethod::addBind(ADWBindParameter(dt, widget001, QString(), EDataBindType::OneWay));
        ASpinBoxBindMethod::addBind(ADWBindParameter(dt, widget002, QString(), EDataBindType::OneWay));
        ASpinBoxBindMethod::addBind(ADWBindParameter(dt, widget003, QString(), EDataBindType::OneWay));
        ASpinBoxBindMethod::addBind(ADWBindParameter(dt, widget004, QString(), EDataBindType::OneWay));
        ASpinBoxBindMethod::addBind(ADWBindParameter(dt, widget005, QString(), EDataBindType::OneWay));

        ASpinBoxBindMethod::addBind(ADWBindParameter(currentIndexDt, widget001, AComboBoxBindMethod::CurrentIndex, EDataBindType::FirstTime));
        ASpinBoxBindMethod::addBind(ADWBindParameter(currentIndexDt, widget002, AComboBoxBindMethod::CurrentIndex, EDataBindType::OneWay));
        ASpinBoxBindMethod::addBind(ADWBindParameter(currentIndexDt, widget003, AComboBoxBindMethod::CurrentIndex, EDataBindType::OneWayRevise));
        ASpinBoxBindMethod::addBind(ADWBindParameter(currentIndexDt, widget004, AComboBoxBindMethod::CurrentIndex, EDataBindType::TwoWay));
        ASpinBoxBindMethod::addBind(ADWBindParameter(currentIndexDt, widget005, AComboBoxBindMethod::CurrentIndex, EDataBindType::TwoWay));
    }
}

WinUIWindow::~WinUIWindow()
{
}