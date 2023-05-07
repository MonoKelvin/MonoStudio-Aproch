#include "WinUIWindow.h"
#include "AprochAPI.h"

#include <QRandomGenerator>
#include <QSpinBox>
#include <QCheckBox>
#include <QRadioButton>
#include <QPushButton>
#include <QComboBox>

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
    ADataContainer* dc = new ADataContainer(true, this);

    // int
    AData* intData = dc->addData(EMetaType::Int, tr("int"));
    dc->setValue(intData, 10);

    QWidget* spinBox001 = new QSpinBox(this);
    layout()->addWidget(spinBox001);
    QWidget* spinBox002 = new QSpinBox(this);
    layout()->addWidget(spinBox002);
    QWidget* spinBox003 = new QSpinBox(this);
    layout()->addWidget(spinBox003);
    QWidget* spinBox004 = new QSpinBox(this);
    layout()->addWidget(spinBox004);

    dwb->bind(ADWBindParameter(intData, spinBox001, QString(), EDataBindType::OneWay));
    dwb->bind(ADWBindParameter(intData, spinBox002, QString(), EDataBindType::TwoWay));
    dwb->bind(ADWBindParameter(intData, spinBox003, QString(), EDataBindType::OneWayRevise));
    dwb->bind(ADWBindParameter(intData, spinBox004, QString(), EDataBindType::FirstTime));

    // string
    AData* strData = dc->addData(EMetaType::QString, tr("string"));
    dc->setValue(strData, QString("aproch"));

    QWidget* lineedit001 = new QLineEdit(this);
    layout()->addWidget(lineedit001);
    QWidget* lineedit002 = new QLineEdit(this);
    layout()->addWidget(lineedit002);

    dwb->bind(ADWBindParameter(strData, lineedit001, QString(), EDataBindType::OneWay));
    dwb->bind(ADWBindParameter(strData, lineedit002, QString(), EDataBindType::TwoWay));

    // double
    AData* dblData = dc->addData(EMetaType::Double, tr("double"));
    dc->setValue(dblData, 3.14);

    QWidget* dsb001 = new QDoubleSpinBox(this);
    layout()->addWidget(dsb001);
    QWidget* dsb002 = new QDoubleSpinBox(this);
    layout()->addWidget(dsb002);

    dwb->bind(ADWBindParameter(dblData, dsb001, QString(), EDataBindType::OneWay));
    dwb->bind(ADWBindParameter(dblData, dsb002, QString(), EDataBindType::TwoWay));

    // bool
    AData* boolData = dc->addData(EMetaType::Bool, tr("bool"));
    dc->setValue(boolData, false);

    QWidget* checkBox001 = new QCheckBox(this);
    QWidget* checkBox002 = new QCheckBox(this);
    layout()->addWidget(checkBox001);
    layout()->addWidget(checkBox002);

    dwb->bind(ADWBindParameter(boolData, checkBox001, QString(), EDataBindType::OneWay));
    dwb->bind(ADWBindParameter(boolData, checkBox002, QString(), EDataBindType::TwoWay));

    // radio
    QRadioButton* radioBtn001 = new QRadioButton(this);
    QRadioButton* radioBtn002 = new QRadioButton(this);
    radioBtn001->setAutoExclusive(false);
    radioBtn002->setAutoExclusive(false);
    layout()->addWidget(radioBtn001);
    layout()->addWidget(radioBtn002);

    dwb->bind(ADWBindParameter(boolData, radioBtn001, QString(), EDataBindType::OneWay));
    dwb->bind(ADWBindParameter(boolData, radioBtn002, QString(), EDataBindType::TwoWay));

    // size
    {
        ASizeDataManager* sizeDM = dc->getDataManager<ASizeDataManager>(EMetaType::QSize);
        AData* sizeData = dc->addData(EMetaType::QSize, tr("size"));
        dc->setValue(sizeData, QSize(1366, 768));

        QLabel* sizeLable = new QLabel(this);
        connect(dc, &ADataContainer::valueChanged, [=](AData* data, const QVariant& v)
        {
            if (data == sizeData)
                sizeLable->setText(data->getDataManager()->toText(data) + 
                                   AStr(" old:(%1, %2)").arg(v.toSize().width()).arg(v.toSize().height()));
        });
        QPushButton* changeSizeBtn = new QPushButton(tr("ChangeSize"), this);
        connect(changeSizeBtn, &QPushButton::clicked, [=](bool) {
            dc->setValue(sizeData, QSize(QRandomGenerator::global()->bounded(100),
            QRandomGenerator::global()->bounded(100)));
        });

        QWidget* sizeSpinBoxW = new QSpinBox(this);
        QWidget* sizeSpinBoxH = new QSpinBox(this);

        QBoxLayout* sizeLayout = new QBoxLayout(QBoxLayout::LeftToRight);
        sizeLayout->addWidget(sizeLable);
        sizeLayout->addWidget(changeSizeBtn);
        sizeLayout->addWidget(sizeSpinBoxW);
        sizeLayout->addWidget(sizeSpinBoxH);
        layout()->addItem(sizeLayout);

        dwb->bind(ADWBindParameter(sizeDM->getWidth(sizeData), sizeSpinBoxW, QString(), EDataBindType::TwoWay));
        dwb->bind(ADWBindParameter(sizeDM->getHeight(sizeData), sizeSpinBoxH, QString(), EDataBindType::TwoWay));
    }

    // stringlist
    {
        AData* currentIndexData = dc->addData(EMetaType::Int, tr("currentIndex"));

        AData* strlistData = dc->addData(EMetaType::QStringList, tr("stringlist"));
        dc->setValue(strlistData, QStringList({"Apple", "Orange", "Banana", "Grape", "Watermelon"}));

        QPushButton* changeItemsBtn = new QPushButton(tr("ChangeItems"), this);
        connect(changeItemsBtn, &QPushButton::clicked, [=](bool) {
            QStringList strlist = strlistData->getValue().toStringList();
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
            dc->setValue(strlistData, strlist);
        });

        QWidget* combox001 = new QComboBox(this);
        QWidget* combox002 = new QComboBox(this);

        QBoxLayout* sizeLayout = new QBoxLayout(QBoxLayout::LeftToRight);
        sizeLayout->addWidget(changeItemsBtn);
        sizeLayout->addWidget(combox001);
        sizeLayout->addWidget(combox002);
        layout()->addItem(sizeLayout);

        dwb->bind(ADWBindParameter(strlistData, combox001, AComboBoxBindMethod::Items, EDataBindType::TwoWay));
        dwb->bind(ADWBindParameter(strlistData, combox002, AComboBoxBindMethod::Items, EDataBindType::TwoWay));

        dwb->bind(ADWBindParameter(currentIndexData, combox001, AComboBoxBindMethod::CurrentIndex));
        dwb->bind(ADWBindParameter(currentIndexData, combox002, AComboBoxBindMethod::CurrentIndex));
    
        //dwb->bind(ADWBindParameter(strData, combox001, AComboBoxBindMethod::CurrentText));
        //dwb->bind(ADWBindParameter(strData, combox002, AComboBoxBindMethod::CurrentText));
    }
}

WinUIWindow::~WinUIWindow()
{
}
