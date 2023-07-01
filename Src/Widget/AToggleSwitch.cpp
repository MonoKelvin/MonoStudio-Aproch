/****************************************************************************
 * @file    AToggleSwitch.cpp
 * @date    2023-07-01 
 * @author  MonoKelvin
 * @email   15007083506@qq.com
 * @github  https://github.com/MonoKelvin
 * @brief
 *
 * This source file is part of Aproch.
 * Copyright (C) 2020 by MonoKelvin. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *****************************************************************************/
#include "stdafx.h"
#include "AToggleSwitch.h"
#include <QPainter>
#include <QStyleOption>
#include <QPainterPath>
#include <QMouseEvent>
#include <QAction>
#include <QVariantAnimation>

#include "Style/AStyleOption.h"
#include "Platform/APlatform.h"

 // 
 // The most of the following code is copied from Qtitan.
 // 
 // Qtitan Library by Developer Machines(Microsoft - Ribbon implementation for Qt.C++)
 // Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
 // 

APROCH_NAMESPACE_BEGIN

class ASwitchAnimation;
class AToggleSwitchPrivate
{
public:
    A_DECLARE_PUBLIC(AToggleSwitch);
    explicit AToggleSwitchPrivate();
    ~AToggleSwitchPrivate();

public:
    void init();
    void clickEvent(const QPoint& pnt);
    void startAnimation(qreal startValue, qreal finalValue, int duration);
    void animationStopped();
    void animationOffset(qreal offset);
    bool isVertical() const { return m_orientation == Qt::Horizontal; }
    void initStyleOption(AToggleSwitchStyleOption* option) const;
    void aproch_actionTriggered();
    void actionChanged();
public:
    bool m_value;
    bool m_animated;
    bool m_dontDrawThumbOn;
    bool m_dontDrawThumbOff;
    bool m_drawTextLabels;
    int m_animationDuration;
    qreal m_animationOffset;
    Qt::Orientation m_orientation;
    Qt::Alignment m_alignment;
    QPointF m_pntClick;
    QString m_strLabelOff;
    QString m_strLabelOn;
    QSize m_sizeHint;

    AToggleSwitch::Shape m_shape;
    ASwitchAnimation* m_animation;
    QAction* m_defaultAction;
private:
    Q_DISABLE_COPY(AToggleSwitchPrivate);
};

class ASwitchAnimation : public QVariantAnimation
{
public:
    ASwitchAnimation(AToggleSwitchPrivate* box) : m_switchBox(box) {
        setEasingCurve(QEasingCurve::Linear);
    }
    ~ASwitchAnimation() override {}
    void updateCurrentValue(const QVariant& current) override {
        if (state() == QVariantAnimation::Running)
            m_switchBox->animationOffset(current.toReal());
    }
    void updateState(State newState, State) override {
        if (newState == QVariantAnimation::Stopped)
            m_switchBox->animationStopped();
    }
private:
    AToggleSwitchPrivate* m_switchBox;
};

AToggleSwitchPrivate::AToggleSwitchPrivate()
    : m_value(false)
    , m_animated(true)
    , m_dontDrawThumbOn(false)
    , m_dontDrawThumbOff(false)
    , m_drawTextLabels(true)
    , m_animationDuration(200)
    , m_animationOffset(0.0)
    , m_orientation(Qt::Horizontal)
    , m_alignment(Qt::Alignment(Qt::AlignLeft | Qt::AlignVCenter))
    , m_strLabelOff(QObject::tr("Off"))
    , m_strLabelOn(QObject::tr("On"))
    , m_sizeHint(QSize())
    , m_shape(AToggleSwitch::Circle)
    , m_animation(nullptr)
    , m_defaultAction(nullptr)
{
}

AToggleSwitchPrivate::~AToggleSwitchPrivate()
{
    delete m_animation;
    m_animation = nullptr;
}

void AToggleSwitchPrivate::init()
{
    A_P(AToggleSwitch);
    p.setFocusPolicy(Qt::FocusPolicy(p.style()->styleHint(QStyle::SH_Button_FocusPolicy)));
    p.setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed));
    p.setAttribute(Qt::WA_WState_OwnSizePolicy, false);
    p.setForegroundRole(QPalette::ButtonText);
    p.setBackgroundRole(QPalette::Button);
}

void AToggleSwitchPrivate::clickEvent(const QPoint& pnt)
{
    A_P(AToggleSwitch);
    p.setOn(!m_value);
    if (p.isAnimated())
    {
        m_pntClick = pnt;
        return;
    }
}

void AToggleSwitchPrivate::startAnimation(qreal startValue, qreal finalValue, int duration)
{
    if (m_animation == nullptr)
        m_animation = new ASwitchAnimation(this);
    m_animation->setStartValue(startValue);
    m_animation->setEndValue(finalValue);
    m_animation->setDuration(duration);
    m_animation->start();
}

void AToggleSwitchPrivate::animationStopped()
{
    if (m_animation != nullptr)
        m_animation->stop();
    m_animationOffset = 0.0;
}

void AToggleSwitchPrivate::animationOffset(qreal offset)
{
    A_P(AToggleSwitch);
    m_animationOffset = offset;
    p.update();
}

void AToggleSwitchPrivate::initStyleOption(AToggleSwitchStyleOption* option) const
{
    A_P(const AToggleSwitch);
    if (option == nullptr)
        return;

    option->initFrom(&p);
    if (p.isOn())
    {
        option->state |= QStyle::State_On;
        option->statusText = m_strLabelOn;
    }
    else
    { 
        option->state |= QStyle::State_Off;
        option->statusText = m_strLabelOff;
    }
#if 0
    bool keyboard = option->state & QStyle::State_KeyboardFocusChange;
    bool keyboard = p.style()->styleHint(QStyle::SH_UnderlineShortcut, option);
#endif

    option->subControls = ACommonStyle::SC_ToggleSwitchThumb | ACommonStyle::SC_ToggleSwitchGroove;
    if (m_drawTextLabels)
        option->subControls |= ACommonStyle::SC_ToggleSwitchText;

    QFontMetrics fm = option->fontMetrics;
    int widthX = aproch_horizontalAdvanceFont(fm, QLatin1Char('X'));
    QSize sizeLabelOn = p.style()->itemTextRect(fm, QRect(), Qt::TextShowMnemonic, false, m_strLabelOn).size();
    sizeLabelOn.setWidth(sizeLabelOn.width() + widthX);
    QSize sizeLabelOff = p.style()->itemTextRect(fm, QRect(), Qt::TextShowMnemonic, false, m_strLabelOff).size();
    sizeLabelOff.setWidth(sizeLabelOff.width() + widthX);

    qreal width = qMax(sizeLabelOn.width(), sizeLabelOff.width());
    qreal height = qMax(sizeLabelOn.height(), sizeLabelOff.height());
    const int h = p.style()->pixelMetric(QStyle::PM_IndicatorHeight, nullptr, &p);
    height = qMax(height, static_cast<qreal>(h));
    qreal thumbsSize = (m_shape != AToggleSwitch::Circle && 
        m_shape != AToggleSwitch::CircleSmallGroove) ?
        height * 2.0 / 3.0 : height - 1;
    option->sizeIndicator.setWidth(width + thumbsSize);
    option->sizeIndicator.setHeight(height);

    option->shape = m_shape;
    option->isAnimated = m_animated;
    option->animationOffset = m_animationOffset;
    option->dontDrawThumbOn = m_dontDrawThumbOn;
    option->dontDrawThumbOff = m_dontDrawThumbOff;
    option->orientation = m_orientation;
    option->textAlignment = m_alignment;
    option->text = p.text();
}

void AToggleSwitchPrivate::aproch_actionTriggered()
{
//    A_P(AToggleSwitch);
//    if (QAction *action = qobject_cast<QAction *>(p.sender()))
//        emit q->triggered(action);
}

void AToggleSwitchPrivate::actionChanged()
{
    A_P(AToggleSwitch);
    if (m_defaultAction == nullptr)
        return;
    p.setText(m_defaultAction->iconText());
    p.setToolTip(m_defaultAction->toolTip());
    p.setStatusTip(m_defaultAction->statusTip());
    p.setOn(m_defaultAction->isChecked());
    p.setEnabled(m_defaultAction->isEnabled());
    p.setFont(m_defaultAction->font());
    if (!p.isWindow())
        p.setVisible(m_defaultAction->isVisible());
}

/* AToggleSwitch */
AToggleSwitch::AToggleSwitch(QAction* action, QWidget* parent)
    : AToggleSwitch(parent)
{
    setDefaultAction(action);
}

AToggleSwitch::AToggleSwitch(QWidget* parent)
    : QAbstractButton(parent)
{
    A_INIT_PRIVATE(AToggleSwitch);
    A_D(AToggleSwitch);
    d.init();
}

AToggleSwitch::~AToggleSwitch()
{
    A_DELETE_PRIVATE();
}

bool AToggleSwitch::isOn() const
{
    A_D(const AToggleSwitch);
    return d.m_value;
}

void AToggleSwitch::setOn(bool set)
{
    A_D(AToggleSwitch);
    if (d.m_value == set)
        return;

    d.m_value = set;
    if (isAnimated())
        d.animationStopped();
    if (isAnimated() && d.m_animationDuration != 0)
        d.startAnimation(1.0, 0.0, d.m_animationDuration);
    d.m_sizeHint = QSize();
    update();

    if (d.m_defaultAction != nullptr)
        d.m_defaultAction->setChecked(d.m_value);

    emit toggled(d.m_value);
}

Qt::Alignment AToggleSwitch::textAlignment() const
{
    A_D(const AToggleSwitch);
    return d.m_alignment;
}

void AToggleSwitch::setTextAlignment(Qt::Alignment alignment)
{
    A_D(AToggleSwitch);
    if (d.m_alignment == alignment)
        return;
    d.m_alignment = alignment;
    d.m_sizeHint = QSize();
    update();
}

QString AToggleSwitch::labelOff() const
{
    A_D(const AToggleSwitch);
    return d.m_strLabelOff;
}

void AToggleSwitch::setLabelOff(const QString& off)
{
    A_D(AToggleSwitch);
    d.m_strLabelOff = off;
    d.m_sizeHint = QSize();
}

QString AToggleSwitch::labelOn() const
{
    A_D(const AToggleSwitch);
    return d.m_strLabelOn;
}

void AToggleSwitch::setLabelOn(const QString& on)
{
    A_D(AToggleSwitch);
    d.m_strLabelOn = on;
    d.m_sizeHint = QSize();
}

bool AToggleSwitch::isAnimated() const
{
    A_D(const AToggleSwitch);
    return d.m_animated;
}

void AToggleSwitch::setAnimated(bool animated)
{
    A_D(AToggleSwitch);
    d.m_animated = animated;
}

bool AToggleSwitch::isVisibleLabels() const
{
    A_D(const AToggleSwitch);
    return d.m_drawTextLabels;
}

void AToggleSwitch::setVisibleLabels(bool visible)
{
    A_D(AToggleSwitch);
    if (d.m_drawTextLabels != visible)
    {
        d.m_drawTextLabels = visible;
        d.m_sizeHint = QSize();
        update();
    }
}

AToggleSwitch::Shape AToggleSwitch::shape() const
{
    A_D(const AToggleSwitch);
    return d.m_shape;
}

void AToggleSwitch::setShape(AToggleSwitch::Shape shape)
{
    A_D(AToggleSwitch);
    if (d.m_shape != shape)
    {
        d.m_shape = shape;
        update(); 
    }
}

QAction* AToggleSwitch::defaultAction() const
{
    A_D(const AToggleSwitch);
    return d.m_defaultAction;
}

void AToggleSwitch::setDefaultAction(QAction* action)
{
    A_D(AToggleSwitch);
    d.m_defaultAction = action;
    if (action == nullptr)
        return;
    if (!actions().contains(action))
        addAction(action);
    d.actionChanged();
}

/*! \reimp */
QSize AToggleSwitch::sizeHint() const
{
    A_D(const AToggleSwitch);
    if (!d.m_drawTextLabels)
        return QSize(50, 20);
    if (d.m_sizeHint.isValid())
        return d.m_sizeHint;

    ensurePolished();
    QFontMetrics fm = fontMetrics();
    int widthX = aproch_horizontalAdvanceFont(fm, QLatin1Char('X'));

    QSize sizeLabelOn = style()->itemTextRect(fm, QRect(), Qt::TextShowMnemonic, false, d.m_strLabelOn).size();
    sizeLabelOn.setWidth(sizeLabelOn.width() + widthX);

    QSize sizeLabelOff = style()->itemTextRect(fm, QRect(), Qt::TextShowMnemonic, false, d.m_strLabelOff).size();
    sizeLabelOff.setWidth(sizeLabelOff.width() + widthX);

    int w = style()->pixelMetric(QStyle::PM_IndicatorWidth, nullptr, this);
    int h = style()->pixelMetric(QStyle::PM_IndicatorHeight, nullptr, this);
    qreal width = qMax(sizeLabelOn.width(), sizeLabelOff.width());
    qreal height = qMax(sizeLabelOn.height(), sizeLabelOff.height());
    height = qMax(height, static_cast<qreal>(h));
    qreal thumbsSize = (d.m_shape != AToggleSwitch::Circle &&
        d.m_shape != AToggleSwitch::CircleSmallGroove) ?
        height * 2.0 / 3.0 : height - 1;
    width = qMax(width + thumbsSize, static_cast<qreal>(w));

    if (!text().isEmpty())
    {
        QSize sizeLabelText = style()->itemTextRect(fm, QRect(), Qt::TextShowMnemonic, false, text()).size();
        int spacing = style()->pixelMetric((QStyle::PixelMetric)ACommonStyle::PM_ToggleSwitchLabelSpacing, nullptr, this);
        width += static_cast<qreal>(sizeLabelText.width() + spacing);
    }

    const qreal margin = style()->pixelMetric((QStyle::PixelMetric)ACommonStyle::PM_ToggleSwitchMargin, nullptr, this);
    
    AToggleSwitch* ts = const_cast<AToggleSwitch*>(this);
    ts->aproch_d().m_sizeHint = QSizeF(width + margin * 2, height + margin * 2).toSize();
    return d.m_sizeHint;
}

/*! \reimp */
QSize AToggleSwitch::minimumSizeHint() const
{
    return sizeHint();
}

/*! \reimp */
bool AToggleSwitch::event(QEvent* event)
{
    if (!isEnabled()) 
    {
        switch (event->type()) 
        {
            case QEvent::TabletPress:
            case QEvent::TabletRelease:
            case QEvent::TabletMove:
            case QEvent::MouseButtonPress:
            case QEvent::MouseButtonRelease:
            case QEvent::MouseButtonDblClick:
            case QEvent::MouseMove:
            case QEvent::HoverMove:
            case QEvent::HoverEnter:
            case QEvent::HoverLeave:
            case QEvent::ContextMenu:
            case QEvent::Wheel:
                return true;
            default:
                break;
        }
    }
    return QWidget::event(event);
}

/*! \reimp */
void AToggleSwitch::changeEvent(QEvent* event)
{
    A_D(AToggleSwitch);
    d.m_sizeHint = QSize();
    QWidget::changeEvent(event);
}

/*! \reimp */
void AToggleSwitch::actionEvent(QActionEvent* event)
{
    A_D(AToggleSwitch);
    QAction* action = event->action();
    switch (event->type()) 
    {
        case QEvent::ActionChanged:
            if (action == d.m_defaultAction)
                d.actionChanged(); // update toggleSwitch state
            break;
        case QEvent::ActionAdded:
            //connect(action, SIGNAL(triggered()), this, SLOT(aproch_actionTriggered()));
            break;
        case QEvent::ActionRemoved:
            if (d.m_defaultAction == action)
                d.m_defaultAction = nullptr;
            action->disconnect(this);
            break;
        default:
            ;
    }
    QWidget::actionEvent(event);
}

/*! \reimp */
void AToggleSwitch::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    A_D(AToggleSwitch);
    QPainter p(this);
    AToggleSwitchStyleOption opt;
    d.initStyleOption(&opt);
    style()->drawComplexControl(static_cast<QStyle::ComplexControl>(ACommonStyle::CC_ToggleSwitch), &opt, &p, this);
}

/*! \reimp */
void AToggleSwitch::mousePressEvent(QMouseEvent* event)
{
    if (event->button() != Qt::LeftButton) 
    {
        event->ignore();
        return;
    }

    if (rect().contains(event->pos()))
    {
//        setDown(true);
////        d.m_pressed = true;
//        repaint();
//        d.emitPressed();
        event->accept();
    }
    else 
        event->ignore();
}

/*! \reimp */
void AToggleSwitch::mouseReleaseEvent(QMouseEvent* event)
{
    A_D(AToggleSwitch);
    if ((event->buttons() & Qt::LeftButton))
        return;
    d.clickEvent(event->pos());
}

/*! \reimp */
void AToggleSwitch::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
}

APROCH_NAMESPACE_END
