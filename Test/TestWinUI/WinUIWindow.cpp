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
#include <QDialog>

#include <QWindowKit/QWKWidgets/widgetwindowagent.h>

using namespace aproch;

#ifdef _DEBUG

ShadowWidget::ShadowWidget(QWidget* parent)
    : QDialog(parent)
{ 
    setWindowFlag(Qt::FramelessWindowHint);
    setWindowFlag(Qt::NoDropShadowWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_StyledBackground);
}

void ShadowWidget::paintEvent(QPaintEvent* evt)
{
    qreal blurRadius = 30.0;
    QPointF offset(0, 6.0);
    QColor shadowColor(8, 12, 20, 80);
    SCornerF borderRadius = { 12 };

    QPainter p(this);
    AGraphicsToolkit::drawShadow(&p, size(), blurRadius, offset, shadowColor, borderRadius);

    p.setBrush(Qt::lightGray);
    p.setPen(Qt::NoPen);
    p.drawRoundedRect(blurRadius - offset.x(), blurRadius - offset.y(),
                       width() - 2 * blurRadius, height() - 2 * blurRadius, 12, 12);
}

#endif

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

            auto edit = new AMenu(tr("Edit(&E)"), menuBar);
            edit->addAction(new QAction(tr("Undo(&U)"), menuBar));
            edit->addAction(new QAction(tr("Redo(&R)"), menuBar));

#ifdef Q_OS_WIN
            auto dwmBlurAction = new QAction(tr("Enable DWM blur"), menuBar);
            dwmBlurAction->setCheckable(true);
            connect(dwmBlurAction, &QAction::toggled, this, [this](bool checked) {
                if (checked)
                    setWinUIMaterial(DWMBlur);
            });

            auto acrylicAction = new QAction(tr("Enable acrylic material"), menuBar);
            acrylicAction->setCheckable(true);
            connect(acrylicAction, &QAction::toggled, this, [this](bool checked) {
                if (checked)
                    setWinUIMaterial(Acrylic);
            });

            auto micaAction = new QAction(tr("Enable mica"), menuBar);
            micaAction->setCheckable(true);
            connect(micaAction, &QAction::toggled, this, [this](bool checked) {
                if (checked)
                    setWinUIMaterial(Mica);
            });

            auto micaAltAction = new QAction(tr("Enable mica alt"), menuBar);
            micaAltAction->setCheckable(true);
            connect(micaAltAction, &QAction::toggled, this, [this](bool checked) {
                if (checked)
                    setWinUIMaterial(MicaAlt);
            });

            auto themeAction = new QAction(tr("Switch Theme"), menuBar);
            themeAction->setCheckable(true);
            connect(themeAction, &QAction::toggled, this, [this](bool checked) {
                auto theme = getWinUITheme() == EThemeType::Light ? EThemeType::Dark : EThemeType::Light;
                setWinUITheme(theme);
            });

            QActionGroup* actionGroup = new QActionGroup(menuBar);
            actionGroup->addAction(dwmBlurAction);
            actionGroup->addAction(acrylicAction);
            actionGroup->addAction(micaAction);
            actionGroup->addAction(micaAltAction);
#endif

            // Real menu
            auto settings = new AMenu(tr("Settings(&S)"), menuBar);

#ifdef Q_OS_WIN
            settings->addSeparator();
            settings->addAction(dwmBlurAction);
            settings->addAction(acrylicAction);
            settings->addAction(micaAction);
            settings->addAction(micaAltAction);
            settings->addAction(themeAction);
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

        setWinUIMaterial(Mica);
        //setWinUITheme(EThemeType::Dark);
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

    QBoxLayout* cbLayout = new QBoxLayout(QBoxLayout::LeftToRight, cw);
    theLayout->addLayout(cbLayout);
    {
        AFontComboBox* fc = new AFontComboBox(cw);
        cbLayout->addWidget(fc);
        aproch::AFontSizeComboBox* fsc = new aproch::AFontSizeComboBox(cw);
        cbLayout->addWidget(fsc);
    }

    QBoxLayout* abLayout = new QBoxLayout(QBoxLayout::LeftToRight, cw);
    theLayout->addLayout(abLayout);
    {
        aproch::AButton* ab111 = new aproch::AButton(AStr("Button"), this);
        abLayout->addWidget(ab111);

        aproch::AButton* ab112 = new aproch::AButton(AStr("Checked Button"), this);
        ab112->setCheckable(true);
        ab112->setChecked(true);
        abLayout->addWidget(ab112);

        aproch::AButton* ab113 = new aproch::AButton(AStr("Disabled AButton"), this);
        ab113->setEnabled(false);
        abLayout->addWidget(ab113);

        aproch::AButton* ab114 = new aproch::AButton(AStr("Disabled Checked Button"), this);
        ab114->setCheckable(true);
        ab114->setChecked(true);
        ab114->setEnabled(false);
        abLayout->addWidget(ab114);


        aproch::AMenu* m1 = new aproch::AMenu(ab111);
        auto act1 = m1->addAction(AStr("Action1"));
        act1->setIcon(AFontIcon::icon("\uE705"));
        act1->setShortcut(Qt::CTRL | Qt::Key_F1); 
        auto act2 = m1->addAction(AStr("Action2"));
        act2->setShortcut(Qt::CTRL | Qt::SHIFT | Qt::Key_P);
        m1->addAction(AStr("Action3"));
        m1->addSeparator();

        QActionGroup* ag1 = new QActionGroup(m1);
        ag1->setExclusive(true);
        auto act3 = m1->addAction(AStr("Checked Action1"));
        //act3->setIcon(AFontIcon::icon("\uE77E")); // 不能设置图标，否则会占用互斥勾选的小部件
        act3->setShortcut(Qt::CTRL | Qt::SHIFT | Qt::Key_M);
        act3->setCheckable(true);
        act3->setActionGroup(ag1);
        auto act4 = m1->addAction(AStr("Checked Action2"));
        //act4->setIcon(AFontIcon::icon("\uE77A"));
        act4->setCheckable(true);
        act4->setChecked(true);
        act4->setActionGroup(ag1);

        auto act5 = m1->addAction(AStr("Disabled Action"));
        act5->setEnabled(false);

        auto m11 = m1->addMenu(AStr("Actions"));
        m11->setIcon(AFontIcon::icon("\uE838"));
        m11->addAction(AStr("Sub Action1"));
        m11->addAction(AStr("Sub Action2"));
        auto m21 = m11->addMenu(AStr("Sub Menu2"));
        m21->setIcon(AFontIcon::icon("\uE7F1"));
        m21->addAction(AStr("SubSubAction1"));
        m21->addAction(AStr("SubSubAction2"));
        m21->addAction(AStr("SubSubAction3"));
        m11->addAction(AStr("Sub Action4"));
        m11->addAction(AStr("Sub Action5"));
        m11->addAction(AStr("Sub Action6"));
        
        //ab111->setMenu(m1);
        connect(ab111, &QPushButton::clicked, [m1]() {
            m1->popup(QCursor::pos());
        });
    }

    {
        aproch::AIconButton* aib1 = new aproch::AIconButton(AStr("\uE8F4"), this);
        connect(aib1, &aproch::AIconButton::clicked, [=]() {
            /*ShadowWidget s;
            s.resize(200, 200);
            s.exec();*/
        });
        abLayout->addWidget(aib1);

        aproch::AIconButton* aib2 = new aproch::AIconButton(AStr("\uE713"), this);
        aib2->setCheckable(true);
        aib2->setChecked(true);
        abLayout->addWidget(aib2);

        aproch::AIconButton* aib3 = new aproch::AIconButton(AStr("\uE8F4"), this);
        aib3->setEnabled(false);
        abLayout->addWidget(aib3);

        aproch::AIconButton* aib4 = new aproch::AIconButton(AStr("\uE713"), this);
        aib4->setCheckable(true);
        aib4->setChecked(true);
        aib4->setEnabled(false);
        abLayout->addWidget(aib4);
    }

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

        ADataWidgetBindMethod::addBind(ADWBindParameter(intData, spinBox001, QString(), EDataBindType::FirstTime));
        ADataWidgetBindMethod::addBind(ADWBindParameter(intData, spinBox002, QString(), EDataBindType::OneWay));
        ADataWidgetBindMethod::addBind(ADWBindParameter(intData, spinBox003, QString(), EDataBindType::OneWayRevise));
        ADataWidgetBindMethod::addBind(ADWBindParameter(intData, spinBox004, QString(), EDataBindType::TwoWay));
        ADataWidgetBindMethod::addBind(ADWBindParameter(intData, spinBox005, QString(), EDataBindType::TwoWay));
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

        ADataWidgetBindMethod::addBind(ADWBindParameter(dblData, spinBox001, QString(), EDataBindType::FirstTime));
        ADataWidgetBindMethod::addBind(ADWBindParameter(dblData, spinBox002, QString(), EDataBindType::OneWay));
        ADataWidgetBindMethod::addBind(ADWBindParameter(dblData, spinBox003, QString(), EDataBindType::OneWayRevise));
        ADataWidgetBindMethod::addBind(ADWBindParameter(dblData, spinBox004, QString(), EDataBindType::TwoWay));
        ADataWidgetBindMethod::addBind(ADWBindParameter(dblData, spinBox005, QString(), EDataBindType::TwoWay));
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

        ADataWidgetBindMethod::addBind(ADWBindParameter(dt, widget001, QString(), EDataBindType::FirstTime));
        ADataWidgetBindMethod::addBind(ADWBindParameter(dt, widget002, QString(), EDataBindType::OneWay));
        ADataWidgetBindMethod::addBind(ADWBindParameter(dt, widget003, QString(), EDataBindType::OneWayRevise));
        ADataWidgetBindMethod::addBind(ADWBindParameter(dt, widget004, QString(), EDataBindType::TwoWay));
        ADataWidgetBindMethod::addBind(ADWBindParameter(dt, widget005, QString(), EDataBindType::TwoWay));
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

        ADataWidgetBindMethod::addBind(ADWBindParameter(dt, widget001, QString(), EDataBindType::FirstTime));
        ADataWidgetBindMethod::addBind(ADWBindParameter(dt, widget002, QString(), EDataBindType::OneWay));
        ADataWidgetBindMethod::addBind(ADWBindParameter(dt, widget003, QString(), EDataBindType::OneWayRevise));
        ADataWidgetBindMethod::addBind(ADWBindParameter(dt, widget004, QString(), EDataBindType::TwoWay));
        ADataWidgetBindMethod::addBind(ADWBindParameter(dt, widget005, QString(), EDataBindType::TwoWay));
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

        ADataWidgetBindMethod::addBind(ADWBindParameter(dt, widget001, QString(), EDataBindType::FirstTime));
        ADataWidgetBindMethod::addBind(ADWBindParameter(dt, widget002, QString(), EDataBindType::OneWay));
        ADataWidgetBindMethod::addBind(ADWBindParameter(dt, widget003, QString(), EDataBindType::OneWayRevise));
        ADataWidgetBindMethod::addBind(ADWBindParameter(dt, widget004, QString(), EDataBindType::TwoWay));
        ADataWidgetBindMethod::addBind(ADWBindParameter(dt, widget005, QString(), EDataBindType::TwoWay));
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
        QWidget* widget001 = new AComboBox(this);
        QWidget* widget002 = new AComboBox(this);
        QWidget* widget003 = new AComboBox(this);
        QWidget* widget004 = new AComboBox(this);
        QWidget* widget005 = new AComboBox(this);
        widgetLayout->addWidget(widget001);
        widgetLayout->addWidget(widget002);
        widgetLayout->addWidget(widget003);
        widgetLayout->addWidget(widget004);
        widgetLayout->addWidget(widget005);
        theLayout->addItem(widgetLayout);

        ADataWidgetBindMethod::addBind(ADWBindParameter(dt, widget001, QString(), EDataBindType::OneWay));
        ADataWidgetBindMethod::addBind(ADWBindParameter(dt, widget002, QString(), EDataBindType::OneWay));
        ADataWidgetBindMethod::addBind(ADWBindParameter(dt, widget003, QString(), EDataBindType::OneWay));
        ADataWidgetBindMethod::addBind(ADWBindParameter(dt, widget004, QString(), EDataBindType::OneWay));
        ADataWidgetBindMethod::addBind(ADWBindParameter(dt, widget005, QString(), EDataBindType::OneWay));

        ADataWidgetBindMethod::addBind(ADWBindParameter(currentIndexDt, widget001, AComboBoxBindMethod::CurrentIndex, EDataBindType::FirstTime));
        ADataWidgetBindMethod::addBind(ADWBindParameter(currentIndexDt, widget002, AComboBoxBindMethod::CurrentIndex, EDataBindType::OneWay));
        ADataWidgetBindMethod::addBind(ADWBindParameter(currentIndexDt, widget003, AComboBoxBindMethod::CurrentIndex, EDataBindType::OneWayRevise));
        ADataWidgetBindMethod::addBind(ADWBindParameter(currentIndexDt, widget004, AComboBoxBindMethod::CurrentIndex, EDataBindType::TwoWay));
        ADataWidgetBindMethod::addBind(ADWBindParameter(currentIndexDt, widget005, AComboBoxBindMethod::CurrentIndex, EDataBindType::TwoWay));
    }

    if (1)
    {
        QBoxLayout* tsLayout = new QBoxLayout(QBoxLayout::LeftToRight);
        AToggleSwitch* ts1 = new AToggleSwitch(AStr("ON"), this);
        ts1->setOffText(AStr("OFF"));
        AToggleSwitch* ts2 = new AToggleSwitch(this);
        AToggleSwitch* ts3 = new AToggleSwitch(AStr("disabled"), this);
        ts3->setEnabled(false);
        AToggleSwitch* ts4 = new AToggleSwitch(this);
        ts4->setOnText(AStr("disabled-checked"));
        ts4->setOffText(AStr("disabled-unchecked long text test"));
        ts4->setChecked(true);
        ts4->setEnabled(false);
        AToggleSwitch* ts5 = new AToggleSwitch(AStr("Right To Left"), this);
        ts5->setLayoutDirection(Qt::RightToLeft);
        AToggleSwitch* ts6 = new AToggleSwitch(this);
        ts6->setChecked(true);
        AButton* tsbtn = new AButton(AStr("Toggle"), this);
        connect(tsbtn, &AButton::clicked, [=]() {
            ts1->setChecked(!ts1->isChecked());
            ts2->setChecked(!ts2->isChecked());
            ts3->setChecked(!ts3->isChecked());
            ts4->setChecked(!ts4->isChecked());
            ts5->setChecked(!ts5->isChecked());
            ts6->setChecked(!ts6->isChecked());
        });
        tsLayout->addWidget(ts1);
        tsLayout->addWidget(ts2);
        tsLayout->addWidget(ts3);
        tsLayout->addWidget(ts4);
        tsLayout->addWidget(ts5);
        tsLayout->addWidget(ts6);
        tsLayout->addWidget(tsbtn);
        tsLayout->addSpacerItem(new QSpacerItem(1,1, QSizePolicy::Expanding));
        theLayout->addItem(tsLayout);
    }
}

WinUIWindow::~WinUIWindow()
{
}
