#include "stdafx.h"
#include "AButton.h"

#include <QApplication>
#include <QPainterPath>
#include "Style/AUIStyleObject.h"

APROCH_NAMESPACE_BEGIN

class AButtonPrivate
{
public:
    QColor normalBkColor;
    QColor hoveredBkColor;
    QColor pressedBkColor;
    QColor checkedBkColor;
    QColor checkHoveredBkColor;

    QColor normalColor;
    QColor hoveredColor;
    QColor pressedColor;
    QColor checkedColor;
    QColor checkHoveredColor;

    QColor currentBgColor;
    QColor currentColor;

    QEasingCurve::Type animationType = QEasingCurve::OutQuart;

    QParallelAnimationGroup* colorAniGroup = nullptr;
    QVariantAnimation* textColorAni = nullptr;
    QVariantAnimation* bgColorAni = nullptr;

    Qt::Alignment textAlignment = Qt::AlignLeft | Qt::AlignVCenter;
    bool isPressed = false;
};

AButton::AButton(QWidget* parent)
    : AButton(QString(), parent)
{
}

AButton::AButton(const QString& text, QWidget* parent)
    : QPushButton(text, parent)
    , d_ptr(new AButtonPrivate)
{
    // variant
    d_ptr->normalBkColor = palette().color(QPalette::ColorRole::Button);
    d_ptr->hoveredBkColor = palette().color(QPalette::ColorRole::Highlight);
    d_ptr->pressedBkColor = palette().color(QPalette::ColorRole::Accent).darker(120);
    d_ptr->checkedBkColor = d_ptr->pressedBkColor;
    d_ptr->checkHoveredBkColor = d_ptr->hoveredBkColor;

    d_ptr->normalColor = palette().color(QPalette::ColorRole::ButtonText);
    d_ptr->hoveredColor = palette().color(QPalette::ColorRole::BrightText);
    d_ptr->pressedColor = palette().color(QPalette::ColorRole::BrightText);
    d_ptr->checkedColor = d_ptr->pressedColor;
    d_ptr->checkHoveredColor = d_ptr->hoveredColor;

    d_ptr->currentBgColor = d_ptr->normalBkColor;
    d_ptr->currentColor = d_ptr->normalColor;

    // background animation
    d_ptr->bgColorAni = new QVariantAnimation(this);
    d_ptr->bgColorAni->setDuration(200);
    d_ptr->bgColorAni->setEasingCurve(QEasingCurve::OutCubic);
    connect(d_ptr->bgColorAni, &QVariantAnimation::valueChanged, [this](const QVariant& val) {
        d_ptr->currentBgColor = val.value<QColor>();
        update();
    });

    // text animation
    d_ptr->textColorAni = new QVariantAnimation(this);
    d_ptr->textColorAni->setDuration(200);
    d_ptr->textColorAni->setEasingCurve(QEasingCurve::OutCubic);
    connect(d_ptr->textColorAni, &QVariantAnimation::valueChanged, [this](const QVariant& val) {
        d_ptr->currentColor = val.value<QColor>();
        update();
    });

    // animation group
    d_ptr->colorAniGroup = new QParallelAnimationGroup(this);
    d_ptr->colorAniGroup->addAnimation(d_ptr->textColorAni);
    d_ptr->colorAniGroup->addAnimation(d_ptr->bgColorAni);
}

AButton::~AButton()
{
}

QColor AButton::getNormalBkColor() const
{
    return d_ptr->normalBkColor;
}

void AButton::setNormalBkColor(const QColor& color)
{
    d_ptr->normalBkColor = color;

    d_ptr->bgColorAni->setStartValue(d_ptr->currentBgColor);
    d_ptr->bgColorAni->setEndValue(isChecked() ? d_ptr->checkedBkColor : d_ptr->normalBkColor);
    d_ptr->bgColorAni->start();

    update();
}

QColor AButton::getHoveredBkColor() const
{
    return d_ptr->hoveredBkColor;
}

void AButton::setHoveredBkColor(const QColor& color)
{
    d_ptr->hoveredBkColor = color;

    d_ptr->bgColorAni->setStartValue(d_ptr->currentBgColor);
    d_ptr->bgColorAni->setEndValue(isChecked() ? d_ptr->checkHoveredBkColor : d_ptr->hoveredBkColor);
    d_ptr->bgColorAni->start();

    update();
}

QColor AButton::getPressedBkColor() const
{
    return d_ptr->pressedBkColor;
}

void AButton::setPressedBkColor(const QColor& color)
{
    d_ptr->pressedBkColor = color;
    update();
}

QColor AButton::getCheckedBkColor() const
{
    return d_ptr->checkedBkColor;
}

void AButton::setCheckedBkColor(const QColor& color)
{
    d_ptr->checkedBkColor = color;
    update();
}

QColor AButton::getCheckHoveredBkColor() const
{
    return d_ptr->checkHoveredBkColor;
}

void AButton::setCheckHoveredBkColor(const QColor& color)
{
    d_ptr->checkHoveredBkColor = color;
    update();
}

QColor AButton::getNormalColor() const
{
    return d_ptr->normalColor;
}

void AButton::setNormalColor(const QColor& color)
{
    d_ptr->normalColor = color;

    d_ptr->textColorAni->setStartValue(d_ptr->currentColor);
    d_ptr->textColorAni->setEndValue(isChecked() ? d_ptr->checkedColor : d_ptr->normalColor);
    d_ptr->textColorAni->start();

    update();
}

QColor AButton::getHoveredColor() const
{
    return d_ptr->hoveredColor;
}

void AButton::setHoveredColor(const QColor& color)
{
    d_ptr->hoveredColor = color;

    d_ptr->textColorAni->setStartValue(d_ptr->currentColor);
    d_ptr->textColorAni->setEndValue(isChecked() ? d_ptr->checkHoveredColor : d_ptr->hoveredColor);
    d_ptr->textColorAni->start();

    update();
}

QColor AButton::getPressedColor() const
{
    return d_ptr->pressedColor;
}

void AButton::setPressedColor(const QColor& color)
{
    d_ptr->pressedColor = color;
    update();
}

QColor AButton::getCheckedColor() const
{
    return d_ptr->checkedColor;
}

void AButton::setCheckedColor(const QColor& color)
{
    d_ptr->checkedColor = color;
    update();
}

QColor AButton::getCheckHoveredColor() const
{
    return d_ptr->checkHoveredColor;
}

void AButton::setCheckHoveredColor(const QColor& color)
{
    d_ptr->checkHoveredColor = color;
    update();
}

int AButton::getAnimationDuration() const
{
    return d_ptr->bgColorAni->duration();
}

void AButton::setAnimationDuration(int duration)
{
    d_ptr->bgColorAni->setDuration(duration);
}

QEasingCurve::Type AButton::getAnimationType() const
{
    return d_ptr->bgColorAni->easingCurve().type();
}

void AButton::setAnimationType(QEasingCurve::Type type)
{
    d_ptr->bgColorAni->setEasingCurve(type);
}

void AButton::setTextAlignment(Qt::Alignment a)
{
    d_ptr->textAlignment = a;
    update();
}

Qt::Alignment AButton::getTextAlignment() const
{
    return d_ptr->textAlignment;
}

void AButton::paintEvent(QPaintEvent* e)
{
    //QStyleOptionButton styleOption;
    //initStyleOption(&styleOption);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    /** -------------------------------------------------- */

    // draw background
    painter.setPen(Qt::NoPen);
    painter.setBrush(d_ptr->currentBgColor);

    QPainterPath path;
    AGraphicsToolkit::drawRoundedRect(path, e->rect(), AprochDefaultBorderRadius);

    painter.drawPath(path);

    /** -------------------------------------------------- */

    // draw text
    painter.setPen(QPen(d_ptr->currentColor));
    painter.setBrush(Qt::NoBrush);

    QTextOption textOption;
    textOption.setAlignment(d_ptr->textAlignment);

    //QStyleOption opt;
    //int paddingLeft = QApplication::style()->pixelMetric(QStyle::PM_ButtonMargin, &opt);

    painter.drawText(e->rect().marginsRemoved(AprochDefaultPaddings), text(), textOption);
}

bool AButton::event(QEvent* e)
{
    const bool checked = isChecked();

    if (e->type() == QEvent::Enter)
    {
        d_ptr->bgColorAni->setStartValue(d_ptr->currentBgColor);
        d_ptr->bgColorAni->setEndValue(checked ? d_ptr->checkHoveredBkColor : d_ptr->hoveredBkColor);
        d_ptr->textColorAni->setStartValue(d_ptr->currentColor);
        d_ptr->textColorAni->setEndValue(checked ? d_ptr->checkHoveredColor : d_ptr->hoveredColor);
        d_ptr->colorAniGroup->start();
    }
    else if (e->type() == QEvent::Leave)
    {
        d_ptr->bgColorAni->setStartValue(d_ptr->currentBgColor);
        d_ptr->bgColorAni->setEndValue(checked ? d_ptr->checkedBkColor : d_ptr->normalBkColor);
        d_ptr->textColorAni->setStartValue(d_ptr->currentColor);
        d_ptr->textColorAni->setEndValue(checked ? d_ptr->checkedColor : d_ptr->normalColor);
        d_ptr->colorAniGroup->start();
    }
    else if (e->type() == QEvent::MouseButtonPress || e->type() == QEvent::MouseButtonDblClick)
    {
        QMouseEvent* me = (QMouseEvent*)e;
        if (!(me->buttons() & Qt::LeftButton))
            return QPushButton::event(e);

        d_ptr->bgColorAni->setStartValue(d_ptr->currentBgColor);
        d_ptr->bgColorAni->setEndValue(d_ptr->pressedBkColor);
        d_ptr->textColorAni->setStartValue(d_ptr->currentColor);
        d_ptr->textColorAni->setEndValue(d_ptr->pressedColor);
        d_ptr->colorAniGroup->start();
        d_ptr->isPressed = true;
    }
    else if (e->type() == QEvent::MouseButtonRelease)
    {
        if (!d_ptr->isPressed)
            return QPushButton::event(e);

        d_ptr->bgColorAni->setStartValue(d_ptr->currentBgColor);
        d_ptr->bgColorAni->setEndValue(checked ? d_ptr->checkHoveredBkColor : d_ptr->hoveredBkColor);
        d_ptr->textColorAni->setStartValue(d_ptr->currentColor);
        d_ptr->textColorAni->setEndValue(checked ? d_ptr->checkHoveredColor : d_ptr->hoveredColor);
        d_ptr->colorAniGroup->start();
        d_ptr->isPressed = false;
    }

    return QPushButton::event(e);
}

APROCH_NAMESPACE_END
