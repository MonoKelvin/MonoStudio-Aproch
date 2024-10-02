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
    QColor disabledBkColor;

    QColor normalColor;
    QColor hoveredColor;
    QColor pressedColor;
    QColor checkedColor;
    QColor checkHoveredColor;
    QColor disabledColor;

    QColor currentBkColor;
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
    setAttribute(Qt::WA_StyledBackground);
    
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

    d_ptr->currentBkColor = d_ptr->normalBkColor;
    d_ptr->currentColor = d_ptr->normalColor;

    // background animation
    d_ptr->bgColorAni = new QVariantAnimation(this);
    d_ptr->bgColorAni->setDuration(200);
    d_ptr->bgColorAni->setEasingCurve(QEasingCurve::OutCubic);
    connect(d_ptr->bgColorAni, &QVariantAnimation::valueChanged, [this](const QVariant& val) {
        d_ptr->currentBkColor = val.value<QColor>();
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

    d_ptr->bgColorAni->setStartValue(d_ptr->currentBkColor);
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

    d_ptr->bgColorAni->setStartValue(d_ptr->currentBkColor);
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

QColor AButton::getDisabledBkColor() const
{
    return d_ptr->disabledBkColor;
}

void AButton::setDisabledBkColor(const QColor& color)
{
    d_ptr->disabledBkColor = color;
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

QColor AButton::getDisabledColor() const
{
    return d_ptr->disabledColor;
}

void AButton::setDisabledColor(const QColor& color)
{
    d_ptr->disabledColor = color;
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
    if (!isEnabled())
    {
        d_ptr->currentBkColor = isChecked() ? d_ptr->checkedBkColor.darker(120) : d_ptr->disabledBkColor;
        d_ptr->currentColor = isChecked() ? d_ptr->checkedColor.darker(120) : d_ptr->disabledColor;
    }

    QStyleOptionButton styleOption;
    initStyleOption(&styleOption);

    auto r = styleOption.rect;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    /** -------------------------------------------------- */

    // draw background
    drawBackground(&painter);

    /** draw icon */

    QRect textRect = r;
    if (!styleOption.icon.isNull())
    {
        QIcon::Mode imode = QIcon::Normal;
        if (styleOption.state & QStyle::State_Active)
            imode = QIcon::Active;
        else if (styleOption.state & QStyle::State_Selected)
            imode = QIcon::Selected;
        else if (!this->isEnabled())
            imode = QIcon::Disabled;

        QIcon::State istate = (styleOption.state & QStyle::State_On) ? QIcon::On : QIcon::Off;

        QRect iconRect(AprochDefaultPaddings.left(), (r.height() - styleOption.iconSize.height()) / 2,
                       styleOption.iconSize.width(), styleOption.iconSize.height());
        painter.drawPixmap(iconRect, styleOption.icon.pixmap(styleOption.iconSize, imode, istate));
        textRect.setLeft(iconRect.right());
    }


    /** -------------------------------------------------- */

    // draw text
    painter.setPen(QPen(d_ptr->currentColor));
    painter.setBrush(Qt::NoBrush);

    QTextOption textOption;
    textOption.setAlignment(d_ptr->textAlignment);

    painter.drawText(textRect.marginsRemoved(AprochDefaultPaddings), text(), textOption);
}

void AButton::drawBackground(QPainter* painter)
{
    painter->setPen(Qt::NoPen);
    painter->setBrush(d_ptr->currentBkColor);

    QPainterPath path;
    AGraphicsToolkit::drawRoundedRect(path, rect(), AprochDefaultBorderRadius);

    painter->drawPath(path);
}

QColor AButton::getCurrentBackgroundColor() const
{
    return d_ptr->currentBkColor;
}

QColor AButton::getCurrentTextColor() const
{
    return d_ptr->currentColor;
}

bool AButton::event(QEvent* e)
{
    if (!isEnabled())
        return QPushButton::event(e);

    QPushButton::event(e);

    const bool checked = isChecked();
    if (e->type() == QEvent::Enter)
    {
        d_ptr->bgColorAni->setStartValue(d_ptr->currentBkColor);
        d_ptr->bgColorAni->setEndValue(checked ? d_ptr->checkHoveredBkColor : d_ptr->hoveredBkColor);
        d_ptr->textColorAni->setStartValue(d_ptr->currentColor);
        d_ptr->textColorAni->setEndValue(checked ? d_ptr->checkHoveredColor : d_ptr->hoveredColor);
        d_ptr->colorAniGroup->start();
    }
    else if (e->type() == QEvent::Leave)
    {
        d_ptr->bgColorAni->setStartValue(d_ptr->currentBkColor);
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

        d_ptr->bgColorAni->setStartValue(d_ptr->currentBkColor);
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

        d_ptr->bgColorAni->setStartValue(d_ptr->currentBkColor);
        d_ptr->bgColorAni->setEndValue(checked ? d_ptr->checkHoveredBkColor : d_ptr->hoveredBkColor);
        d_ptr->textColorAni->setStartValue(d_ptr->currentColor);
        d_ptr->textColorAni->setEndValue(checked ? d_ptr->checkHoveredColor : d_ptr->hoveredColor);
        d_ptr->colorAniGroup->start();
        d_ptr->isPressed = false;
    }
}

APROCH_NAMESPACE_END
