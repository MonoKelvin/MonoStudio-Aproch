#include "WinUIWindow.h"
#include "AprochAPI.h"

#include <QRandomGenerator>
#include <QSpinBox>
#include <QCheckBox>
#include <QRadioButton>
#include <QPushButton>
#include <QComboBox>

#include <QItemEditorCreator>
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

    auto* factory = QItemEditorFactory::defaultFactory();

    {
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


    /*QList<AAbstractDataManager*> dataMgrList;
    {
        ABoolDataManager* BoolDM = new ABoolDataManager;
        ACharDataManager* CharDM = new ACharDataManager;
        AUCharDataManager* UCharDM = new AUCharDataManager;
        AShortDataManager* ShortDM = new AShortDataManager;
        AUShortDataManager* UShortDM = new AUShortDataManager;
        AIntegerDataManager* IntegerDM = new AIntegerDataManager;
        AUIntegerDataManager* UIntegerDM = new AUIntegerDataManager;
        ALongDataManager* LongDM = new ALongDataManager;
        AULongDataManager* ULongDM = new AULongDataManager;
        ALongLongDataManager* LongLongDM = new ALongLongDataManager;
        AULongLongDataManager* ULongLongDM = new AULongLongDataManager;
        AFloatDataManager* FloatDM = new AFloatDataManager;
        ADoubleDataManager* DoubleDM = new ADoubleDataManager;
        AStringDataManager* StringDM = new AStringDataManager;
        ASizeDataManager* SizeDM = new ASizeDataManager;
        AStringListDataManager* StringListDM = new AStringListDataManager;

        dataMgrList.push_back(BoolDM);
        dataMgrList.push_back(CharDM);
        dataMgrList.push_back(UCharDM);
        dataMgrList.push_back(ShortDM);
        dataMgrList.push_back(UShortDM);
        dataMgrList.push_back(IntegerDM);
        dataMgrList.push_back(UIntegerDM);
        dataMgrList.push_back(LongDM);
        dataMgrList.push_back(ULongDM);
        dataMgrList.push_back(LongLongDM);
        dataMgrList.push_back(ULongLongDM);
        dataMgrList.push_back(FloatDM);
        dataMgrList.push_back(DoubleDM);
        dataMgrList.push_back(StringDM);
        dataMgrList.push_back(SizeDM);
        dataMgrList.push_back(StringListDM);
    }

    // data binding
    ADataWidgetBinding* dwb = new ADataWidgetBinding(this);

    // data manager
    ADataContainer* dc = new ADataContainer(dataMgrList, this);

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

        QLabel* sizeLable = new QLabel(this);
        connect(dc, &ADataContainer::valueChanged, [=](AData* data, const QVariant& v)
        {
            if (data == sizeData)
                sizeLable->setText(data->toString() + AStr(" old:(%1, %2)").arg(v.toSize().width()).arg(v.toSize().height()));
        });
        QPushButton* changeSizeBtn = new QPushButton(tr("ChangeSize"), this);
        connect(changeSizeBtn, &QPushButton::clicked, [=](bool) {
            dc->setValue(sizeData, QSize(QRandomGenerator::global()->bounded(100),
            QRandomGenerator::global()->bounded(100)));
        });

        dc->setValue(sizeData, QSize(1366, 768));

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
    }*/
}

WinUIWindow::~WinUIWindow()
{
}
