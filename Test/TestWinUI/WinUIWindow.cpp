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
#include <QTextEdit>
#include <QLineEdit>
#include <QMenuBar>
#include <QActionGroup>

#include <QWindowKit/QWKWidgets/widgetwindowagent.h>

using namespace aproch;

WinUIWindow::WinUIWindow(QWidget *parent)
    : aproch::AWindow(parent)
{
    ui.setupUi(this);
    {
        auto menuBar = [this]() {
            auto menuBar = new QMenuBar();

            // Virtual menu
            auto file = new AMenu(tr("File(&F)"), menuBar);
            file->addAction(new QAction(tr("New(&N)"), menuBar));
            file->addAction(new QAction(tr("Open(&O)"), menuBar));
            file->addSeparator();

            auto edit = new QMenu(tr("Edit(&E)"), menuBar);
            edit->addAction(new QAction(tr("Undo(&U)"), menuBar));
            edit->addAction(new QAction(tr("Redo(&R)"), menuBar));

#ifdef Q_OS_WIN
            auto dwmBlurAction = new QAction(tr("Enable DWM blur"), menuBar);
            dwmBlurAction->setCheckable(true);
            connect(dwmBlurAction, &QAction::toggled, this, [this](bool checked) {
                setBackgroundMaterial(EWinBackgroundMaterial::DWMBlur, checked);
            });

            auto acrylicAction = new QAction(tr("Enable acrylic material"), menuBar);
            acrylicAction->setCheckable(true);
            connect(acrylicAction, &QAction::toggled, this, [this](bool checked) {
                setBackgroundMaterial(EWinBackgroundMaterial::Acrylic, checked);
            });

            auto micaAction = new QAction(tr("Enable mica"), menuBar);
            micaAction->setCheckable(true);
            connect(micaAction, &QAction::toggled, this, [this](bool checked) {
                setBackgroundMaterial(EWinBackgroundMaterial::Mica, checked);
            });

            auto micaAltAction = new QAction(tr("Enable mica alt"), menuBar);
            micaAltAction->setCheckable(true);
            connect(micaAltAction, &QAction::toggled, this, [this](bool checked) {
                setBackgroundMaterial(EWinBackgroundMaterial::MicaAlt, checked);
            });

            QActionGroup* actionGroup = new QActionGroup(menuBar);
            actionGroup->addAction(dwmBlurAction);
            actionGroup->addAction(acrylicAction);
            actionGroup->addAction(micaAction);
            actionGroup->addAction(micaAltAction);
#endif

            // Real menu
            auto settings = new QMenu(tr("Settings(&S)"), menuBar);

#ifdef Q_OS_WIN
            settings->addSeparator();
            settings->addAction(dwmBlurAction);
            settings->addAction(acrylicAction);
            settings->addAction(micaAction);
            settings->addAction(micaAltAction);
#elif defined(Q_OS_MAC)
            settings->addAction(darkBlurAction);
            settings->addAction(lightBlurAction);
            settings->addAction(noBlurAction);
#endif

            menuBar->addMenu(file);
            menuBar->addMenu(edit);
            menuBar->addMenu(settings);
            return menuBar;
        }();

        getCaptionBar()->setMenuBar(menuBar);

        mWinAgent->setHitTestVisible(menuBar, true);

        setBackgroundMaterial(EWinBackgroundMaterial::MicaAlt);
        setMaterialTheme(ATheme::Dark);
    }

    QWidget* cw = new QWidget(this);
    setCentralWidget(cw);

    QBoxLayout* theLayout = new QBoxLayout(QBoxLayout::TopToBottom, cw);

    /*QLineEdit* le111 = new QLineEdit(this);
    le111->setPlaceholderText(AStr("placeholder text"));
    theLayout->addWidget(le111);

    ATextBox* tb111 = new ATextBox(this);
    tb111->setPlaceholderText(AStr("placeholder text"));
    theLayout->addWidget(tb111);*/

    QPushButton* pb111 = new QPushButton(AStr("按钮QPushButton001"), this);
    theLayout->addWidget(pb111);

    /*QTextEdit* te001 = new QTextEdit(this);
    te001->setPlaceholderText(AStr("placeholder text"));
    theLayout->addWidget(te001);*/

    if (0)
    {
        /*new QBoxLayout(QBoxLayout::TopToBottom, this);
        ATextBlock* newBlock = new ATextBlock(AStr("I am a Text"), this);
        theLayout->addWidget(newBlock);

        ATextBlock* newBlock2 = new ATextBlock(AStr("Lorem ipsum dolor, sit amet consectetur adipisicing elit."), this);
        theLayout->addWidget(newBlock2);
        newBlock2->setText(AStr("Lorem ipsum dolor, sit amet consectetur adipisicing elit."), Qt::ElideRight);

        ATextBox* textBox = new ATextBox(this);
        theLayout->addWidget(textBox);

        ATextBox* textBox2 = new ATextBox(this);
        theLayout->addWidget(textBox2);*/
    }

    if (1)
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
        theLayout->addItem(widgetLayout);

        ASpinBoxBindMethod::addBind(ADWBindParameter(intData, spinBox001, QString(), EDataBindType::FirstTime));
        ASpinBoxBindMethod::addBind(ADWBindParameter(intData, spinBox002, QString(), EDataBindType::OneWay));
        ASpinBoxBindMethod::addBind(ADWBindParameter(intData, spinBox003, QString(), EDataBindType::OneWayRevise));
        ASpinBoxBindMethod::addBind(ADWBindParameter(intData, spinBox004, QString(), EDataBindType::TwoWay));
        ASpinBoxBindMethod::addBind(ADWBindParameter(intData, spinBox005, QString(), EDataBindType::TwoWay));
    }

    if (1)
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
        theLayout->addItem(widgetLayout);

        ASpinBoxBindMethod::addBind(ADWBindParameter(dblData, spinBox001, QString(), EDataBindType::FirstTime));
        ASpinBoxBindMethod::addBind(ADWBindParameter(dblData, spinBox002, QString(), EDataBindType::OneWay));
        ASpinBoxBindMethod::addBind(ADWBindParameter(dblData, spinBox003, QString(), EDataBindType::OneWayRevise));
        ASpinBoxBindMethod::addBind(ADWBindParameter(dblData, spinBox004, QString(), EDataBindType::TwoWay));
        ASpinBoxBindMethod::addBind(ADWBindParameter(dblData, spinBox005, QString(), EDataBindType::TwoWay));
    }

    if (1)
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
        theLayout->addItem(widgetLayout);

        ASpinBoxBindMethod::addBind(ADWBindParameter(dt, widget001, QString(), EDataBindType::FirstTime));
        ASpinBoxBindMethod::addBind(ADWBindParameter(dt, widget002, QString(), EDataBindType::OneWay));
        ASpinBoxBindMethod::addBind(ADWBindParameter(dt, widget003, QString(), EDataBindType::OneWayRevise));
        ASpinBoxBindMethod::addBind(ADWBindParameter(dt, widget004, QString(), EDataBindType::TwoWay));
        ASpinBoxBindMethod::addBind(ADWBindParameter(dt, widget005, QString(), EDataBindType::TwoWay));
    }

    if (1)
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
        theLayout->addItem(widgetLayout);

        ASpinBoxBindMethod::addBind(ADWBindParameter(dt, widget001, QString(), EDataBindType::FirstTime));
        ASpinBoxBindMethod::addBind(ADWBindParameter(dt, widget002, QString(), EDataBindType::OneWay));
        ASpinBoxBindMethod::addBind(ADWBindParameter(dt, widget003, QString(), EDataBindType::OneWayRevise));
        ASpinBoxBindMethod::addBind(ADWBindParameter(dt, widget004, QString(), EDataBindType::TwoWay));
        ASpinBoxBindMethod::addBind(ADWBindParameter(dt, widget005, QString(), EDataBindType::TwoWay));
    }

    if (1)
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
        theLayout->addItem(widgetLayout);

        ASpinBoxBindMethod::addBind(ADWBindParameter(dt, widget001, QString(), EDataBindType::FirstTime));
        ASpinBoxBindMethod::addBind(ADWBindParameter(dt, widget002, QString(), EDataBindType::OneWay));
        ASpinBoxBindMethod::addBind(ADWBindParameter(dt, widget003, QString(), EDataBindType::OneWayRevise));
        ASpinBoxBindMethod::addBind(ADWBindParameter(dt, widget004, QString(), EDataBindType::TwoWay));
        ASpinBoxBindMethod::addBind(ADWBindParameter(dt, widget005, QString(), EDataBindType::TwoWay));
    }

    if (1)
    {
        AIntDataManager* IntDM = new AIntDataManager(this);
        AStringListDataManager* StringListDM = new AStringListDataManager(this);
        AData* dt = StringListDM->addData(AStr("name"));
        StringListDM->setValue(dt, QStringList({ "Apple", "Orange", "Banana", "Grape", "Watermelon" }));

        AData* currentIndexDt = IntDM->addData(AStr("currentIndex"));

        QPushButton* changeItemsBtn = new QPushButton(tr("ChangeItems"), this);
        changeItemsBtn->move((width() - changeItemsBtn->width()) / 2, height() - changeItemsBtn->height() - 10);
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
        theLayout->addItem(widgetLayout);

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

#include <QGraphicsDropShadowEffect>

AMenu::AMenu(QWidget* parent)
    : AMenu("", parent)
{
}

AMenu::AMenu(const QString& title, QWidget* parent)
    : QMenu(title, parent)
{
    setWindowFlags(windowFlags() /*| Qt::FramelessWindowHint*/ | Qt::NoDropShadowWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setAutoFillBackground(false);

    int BlurRadius = 40;
    int XOffset = 2;
    int YOffset = 4;

    QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect(this);
    effect->setBlurRadius(BlurRadius);
    effect->setColor("#55111111");
    effect->setOffset(XOffset, YOffset);
    setGraphicsEffect(effect);

    QString strMenuStyle = QString("QMenu{background-color:#fff;color:#111;border-radius:6px;margin:%1px %2px %3px %4px;}")
        .arg(BlurRadius - YOffset).arg(BlurRadius + XOffset).arg(BlurRadius + YOffset).arg(BlurRadius - XOffset);
    this->setStyleSheet(strMenuStyle);
}
//
//void AMenu::paintEvent(QPaintEvent* evt)
//{
//    APROCH_USE_STYLE_SHEET();
//
//    QPainter painter(this);
//
//    /*painter.setRenderHint(QPainter::Antialiasing);
//    painter.setPen(Qt::NoPen);
//    painter.setBrush(Qt::gray);
//    painter.drawRoundedRect(rect(), 10, 10);*/
//
//    QMenu::paintEvent(evt);
//}

void AMenu::showEvent(QShowEvent* evt)
{
    //move(x() - 32, y() - 13);
    return QMenu::showEvent(evt);
}
