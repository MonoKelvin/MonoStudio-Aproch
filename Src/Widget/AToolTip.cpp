/****************************************************************************
 * @file    AToolTip.cpp
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
#include "AToolTip.h"

#include <QApplication>
#include <QHash>
#include <QLabel>
#include <QStylePainter>
#include <QTextDocument>
#include <QToolTip>
#include <QMouseEvent>
#include <QToolButton>

#include "Style/ACommonStyle_p.h"
#include "Style/AStyleHelper.h"

 // 
 // The most of the following code is copied from Qtitan.
 // 
 // Qtitan Library by Developer Machines(Microsoft - Ribbon implementation for Qt.C++)
 // Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
 // 

APROCH_NAMESPACE_BEGIN

/* AToolTipPrivate */
class AToolTipPrivate
{
    A_DECLARE_PUBLIC(AToolTip)
public:
    explicit AToolTipPrivate();
public:
    void updateTool();
public:
    int m_indent;
    int m_margin;
    int m_maxTipWidth;
    static bool m_showIcon;
    bool m_fadingOut;
    QLabel* m_label;
    QWidget* m_widget;
    static AToolTip* m_instance;
    static AToolTip::WrapMode m_wrapMode;
    QRect m_rect;
    QString m_text;
    QString m_textTitle;
    QIcon m_icon;
    QBasicTimer m_hideTimer, m_expireTimer;
};

AToolTip* AToolTipPrivate::m_instance = nullptr;
AToolTip::WrapMode AToolTipPrivate::m_wrapMode = AToolTip::NoWrap;
bool AToolTipPrivate::m_showIcon = true;

AToolTipPrivate::AToolTipPrivate()
    : m_indent(-1)
    , m_margin(0)
    , m_maxTipWidth(160)
    , m_fadingOut(false)
    , m_label(nullptr)
    , m_widget(nullptr)
{
}

static QString removeMnemonics(const QString &original)
{
    QString returnText(original.size(), QChar());
    int finalDest = 0;
    int currPos = 0;
    int l = original.length();
    while (l) {
        if (original.at(currPos) == QLatin1Char('&')
            && (l == 1 || original.at(currPos + 1) != QLatin1Char('&'))) {
            ++currPos;
            --l;
            if (l == 0)
                break;
        }
        else if (original.at(currPos) == QLatin1Char('(') && l >= 4 &&
            original.at(currPos + 1) == QLatin1Char('&') &&
            original.at(currPos + 2) != QLatin1Char('&') &&
            original.at(currPos + 3) == QLatin1Char(')')) {
            /* remove mnemonics its format is "\s*(&X)" */
            int n = 0;
            while (finalDest > n && returnText.at(finalDest - n - 1).isSpace())
                ++n;
            finalDest -= n;
            currPos += 4;
            l -= 4;
            continue;
        }
        returnText[finalDest] = original.at(currPos);
        ++currPos;
        ++finalDest;
        --l;
    }
    returnText.truncate(finalDest);
    return returnText;
}

void AToolTipPrivate::updateTool()
{
    A_P(AToolTip);
    if (m_label)
    {
        m_label->setText(::removeMnemonics(m_text));
        m_label->setWordWrap(/*Qt::mightBeRichText(m_text)*/true);
        m_label->adjustSize();
    }

    p.adjustSize();
    p.updateGeometry();
    p.update(p.contentsRect());
}

/*!
\class AToolTip
\inmodule QtitanBase
\brief Class AToolTip provides tool tips (balloon help) for any widget.
*/
/*!
Constructs AToolTip object with the given \a parent. Parameter \a title - is a title of the tooltip, \a text - is a text of the tooltip.
*/ 
AToolTip::AToolTip(const QString& title, const QString& text, const QIcon& icon, QWidget* parent)
    : QFrame(parent, Qt::WindowFlags(Qt::ToolTip | Qt::BypassGraphicsProxyWidget))
{
    delete AToolTipPrivate::m_instance;
    AToolTipPrivate::m_instance = this;

    A_INIT_PRIVATE(AToolTip);
    A_D(AToolTip);

    if (wordWrap() == NativeWrap)
        d.m_label = new QLabel(this);
    
    if (d.m_label)
    {
        d.m_label->setForegroundRole(QPalette::ToolTipText);
        d.m_label->setBackgroundRole(QPalette::ToolTipBase);
        d.m_label->setPalette(QToolTip::palette());
        d.m_label->setFont(QToolTip::font());
        d.m_label->ensurePolished();
        d.m_label->setMargin(1 + style()->pixelMetric(QStyle::PM_ToolTipLabelFrameWidth, nullptr, this));
        d.m_label->setFrameStyle(QFrame::NoFrame);
    }

    setForegroundRole(QPalette::ToolTipText);
    setBackgroundRole(QPalette::ToolTipBase);
//    setPalette(QToolTip::palette());

    setFont(QToolTip::font());
    ensurePolished();
    setMargin(1 + style()->pixelMetric(QStyle::PM_ToolTipLabelFrameWidth, nullptr, this));
    setFrameStyle(QFrame::NoFrame);
    qApp->installEventFilter(this);
    setWindowOpacity(style()->styleHint(QStyle::SH_ToolTipLabel_Opacity, nullptr, this) / 255.0);
    setMouseTracking(true);
    d.m_fadingOut = false;
    setIcon(isIconVisible() ? icon : QIcon());
    reuseTip(title, text);
}

/*!
Destructor of the AToolTip object.
*/ 
AToolTip::~AToolTip()
{
    AToolTipPrivate::m_instance = nullptr;
    A_DELETE_PRIVATE();
}

AToolTip* AToolTip::instance()
{
    return AToolTipPrivate::m_instance;
}

/*!
Shows tooltip in \a pos position. Parameter \a title - is a title of the tooltip, \a text - is a text of the tooltip, \a icon - the icon of the tooltip. 
Parameter \a parent - reference to the widget for calculating the position of tooltip.
*/ 
void AToolTip::showToolTip(const QPoint &pos, const QString& title, const QString& text, const QIcon& icon, QWidget* parent)
{
    QRect rect;
    // a tip does already exist
    if (AToolTipPrivate::m_instance && AToolTipPrivate::m_instance->isVisible())
    {
        // empty text means hide current tip
        if (text.isEmpty())
        {
            AToolTipPrivate::m_instance->hideTip();
            return;
        }
        else if (!AToolTipPrivate::m_instance->fadingOut())
        {
            // If the tip has changed, reuse the one
            // that is showing (removes flickering)
            QPoint localPos = pos;
            if (parent != nullptr)
                localPos = parent->mapFromGlobal(pos);
            if (AToolTipPrivate::m_instance->tipChanged(localPos, text, title, parent))
            {
                AToolTipPrivate::m_instance->setIcon(isIconVisible() ? icon : QIcon());
                AToolTipPrivate::m_instance->reuseTip(title, text);
                AToolTipPrivate::m_instance->setTipRect(parent, rect);
                AToolTipPrivate::m_instance->placeTip(pos, parent);
            }
            return;
        }
    }

    if (!text.isEmpty())
    {
        new AToolTip(title, text, icon, parent);
        AToolTipPrivate::m_instance->setTipRect(parent, rect);
        AToolTipPrivate::m_instance->placeTip(pos, parent);

#if !defined(QT_NO_EFFECTS) && !defined(Q_WS_MAC)
        if (QApplication::isEffectEnabled(Qt::UI_FadeTooltip))
            qFadeEffect(AToolTipPrivate::m_instance);
        else if (QApplication::isEffectEnabled(Qt::UI_AnimateTooltip))
            qScrollEffect(AToolTipPrivate::m_instance);
        else
            AToolTipPrivate::m_instance->showNormal();
#else
        AToolTipPrivate::m_instance->showNormal();
#endif
    }
}

/*!
Hides the tooltip window.
*/ 
void AToolTip::hideToolTip() 
{ 
    showToolTip(QPoint(), QString(), QString(), QIcon()); 
}

/*!
Returns visibility of the tooltip at the moment.
*/ 
bool AToolTip::isToolTipVisible()
{
    return (AToolTipPrivate::m_instance != nullptr && AToolTipPrivate::m_instance->isVisible());
}
/*
QString AToolTip::textToolTip()
{
    if (AToolTipPrivate::m_instance)
        return AToolTipPrivate::m_instance->text();
    return QString();
}
*/
/*!
Sets the wrap \a mode for the tolltip's text.
*/ 
void AToolTip::setWrapMode(WrapMode mode)
{
    AToolTipPrivate::m_wrapMode = mode;
}

/*!
Returns the wrap mode for the tooltip's text.
*/
AToolTip::WrapMode AToolTip::wordWrap()
{
    return AToolTipPrivate::m_wrapMode;
}

/*!
Sets the visibility of the tooltip's icon to \a visible. Method is static.
*/ 
void AToolTip::setIconVisible(bool visible)
{
    AToolTipPrivate::m_showIcon = visible;
}

/*!
Returns the visibility of the tooltip's icon. Method is static.
*/ 
bool AToolTip::isIconVisible()
{
    return AToolTipPrivate::m_showIcon;
}

/*!
Returns the margin \a mode for the tolltip's text in pixels.
*/ 
int AToolTip::margin() const
{
    A_D(const AToolTip);
    return d.m_margin;
}

/*!
Sets the margin \a mode for the tolltip's text in in pixels.
*/ 
void AToolTip::setMargin(int margin)
{
    A_D(AToolTip);
    if (d.m_margin == margin)
        return;
    d.m_margin = margin;
    d.updateTool();
}

void AToolTip::setTitle(const QString& text)
{
    A_D(AToolTip);
    if (d.m_textTitle == text)
        return;
    d.m_textTitle = text;
    d.updateTool();
}

const QString& AToolTip::title() const
{
    A_D(const AToolTip);
    return d.m_textTitle;
}

void AToolTip::setText(const QString& text)
{
    A_D(AToolTip);
    if (d.m_text == text)
        return;
    d.m_text = text;
    d.updateTool();
}

const QString& AToolTip::text() const
{
    A_D(const AToolTip);
    return d.m_text;
}

void AToolTip::setIcon(const QIcon& icon)
{
    A_D(AToolTip);
    d.m_icon = icon;
    d.updateTool();
}

const QIcon& AToolTip::icon() const
{
    A_D(const AToolTip);
    return d.m_icon;
}

void AToolTip::restartExpireTimer()
{
    A_D(AToolTip);
    int time = 10000 + 40 * qMax(0, text().length()-100);
    d.m_expireTimer.start(time, this);
    d.m_hideTimer.stop();
}

void AToolTip::reuseTip(const QString& textTitle, const QString& text)
{
    setText(text);
    setTitle(textTitle);
    QFontMetrics fm(font());
    QSize extra(1, 0);
    // Make it look good with the default AToolTip font on Mac, which has a small descent.
    if (fm.descent() == 2 && fm.ascent() >= 11)
        ++extra.rheight();

    resize(sizeHint() + extra);
    restartExpireTimer();
}

/*! \reimp */
QSize AToolTip::sizeHint() const
{
    A_D(const AToolTip);

    QFontMetrics fm = fontMetrics();

    QRect rcMargin(margin(), margin(), margin(), margin());
    QSize szMargin(3 + rcMargin.left() + rcMargin.right() + 3, 3 + rcMargin.top() + rcMargin.bottom() + 3);

    int flags = Qt::TextExpandTabs | Qt::TextHideMnemonic;

    bool drawTitle = !d.m_textTitle.isEmpty();
    bool drawImage = !d.m_icon.isNull();
    bool drawImageTop = true;
    QSize szImage(0, 0);
    QSize szTitle(0, 0);

    if (drawTitle)
    {
        QFont fontBold = font();
        fontBold.setBold(true);
        QFontMetrics fmBold(fontBold);
        int w = fmBold.averageCharWidth() * 80;
        QRect rcTitle = fmBold.boundingRect(0, 0, w, 2000, flags | Qt::TextSingleLine, d.m_textTitle);
        szTitle = QSize(rcTitle.width(), rcTitle.height() + 15);
    }

    QSize szIcon(0, 0);
    if (drawImage)
    {
        QStyleOptionFrame opt;
        opt.initFrom(this);

        if (QToolButton* button = qobject_cast<QToolButton*>(d.m_widget))
        {
            drawImageTop = button->toolButtonStyle() != Qt::ToolButtonTextUnderIcon;
            const int sz = style()->pixelMetric(drawImageTop ? QStyle::PM_SmallIconSize : QStyle::PM_LargeIconSize, nullptr, button);
            szIcon = QSize(sz, sz);
        }
        else
        {
            szIcon = d.m_icon.actualSize(opt.rect.adjusted(3, 3, -3, -3).size());
            drawImageTop = szIcon.height() <= 16;
        }

        if (drawImageTop)
        {
            if (!drawTitle)
                szImage.setWidth(szIcon.height() + 3);
            else
                szTitle.setWidth(szTitle.width() + szIcon.width() + 1);
        }
        else
            szImage.setWidth(szIcon.width() + 5);
        szImage.setHeight(szIcon.height());
    }

    QSize szText;
    if (!d.m_label)
    {
        int maxTipWidth = d.m_maxTipWidth;
        QRect rcText(QPoint(0, 0), QSize(maxTipWidth - szMargin.width(), 0));
        rcText = fm.boundingRect(rcText.left(), rcText.top(), rcText.width(), rcText.height(), flags | Qt::TextWordWrap, d.m_text);
        szText = rcText.size();
    }
    else
    {
        szText = d.m_label->sizeHint();
    }

    QSize sz(0, 0);
    sz.setHeight(qMax(szImage.height(), szText.height()));
    sz.setWidth(szImage.width() + szText.width());

    if (drawTitle)
    {
        const int siz = style()->pixelMetric(QStyle::PM_SmallIconSize, nullptr, this);
        sz.setWidth(qMax(sz.width(), szTitle.width()) + siz);
        sz.setHeight(sz.height() + szTitle.height());
    }
    sz += szMargin;
    return sz;
}

/*! \reimp */
void AToolTip::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    A_D(AToolTip);

    QStylePainter p(this);
    drawFrame(&p);

    QStyleOptionFrame opt;
    opt.initFrom(this);
    p.drawPrimitive(QStyle::PE_PanelTipLabel, opt);
    
    QRect rc = contentsRect();
    rc.adjust(margin(), margin(), -margin(), -margin());
    rc.adjust(3, 3, -3, -3);

    QRect rcTitle(rc.left() + margin(), rc.top() + margin(), rc.right() - margin(), rc.bottom());

    int flags = Qt::TextExpandTabs | Qt::TextHideMnemonic;

    bool drawTitle = !d.m_textTitle.isEmpty();
    bool drawImage = !d.m_icon.isNull();
    bool drawImageTop = true;

    QSize szImage(0, 0), szDrvImage(0, 0);
    if (drawImage)
    {
        if (QToolButton* button = qobject_cast<QToolButton*>(d.m_widget))
        {
            drawImageTop = button->toolButtonStyle() != Qt::ToolButtonTextUnderIcon;
            const int sz = style()->pixelMetric(drawImageTop ? QStyle::PM_SmallIconSize : QStyle::PM_LargeIconSize, nullptr, button);
//            const int dpi = ACommonStylePrivate::dpiScaled(1, this) / 100;
//            szImage = QSize(sz / dpi, sz / dpi);
            szDrvImage = QSize(sz, sz);
        }
        else
        {
            szImage = d.m_icon.actualSize(opt.rect.adjusted(3, 3, -3, -3).size());
            szDrvImage = szImage;
            drawImageTop = szImage.height() <= 16;
        }

        if (drawImageTop)
        {
            QPoint ptIcon = rc.topLeft();
            ptIcon.setY(rcTitle.y());
            d.m_icon.paint(&p, QRect(ptIcon, szDrvImage), Qt::AlignCenter, QIcon::Normal, QIcon::On);
            //QPixmap pm = ACommonStylePrivate::preparePixmap(d.m_icon, szDrvImage, QIcon::Normal, QIcon::On, this);
            //style()->drawItemPixmap(&p, QRect(ptIcon, szDrvImage), Qt::AlignCenter, pm);
            rcTitle.setLeft(rcTitle.left() + szDrvImage.width() + 1);
            rc.setLeft(rc.left() + szDrvImage.width() + 3);
        }
    }
    if (drawTitle)
    {
        QFont oldFont = p.font();
        QFont fnt = oldFont;
        fnt.setBold(true);
        p.setFont(fnt);
        style()->drawItemText(&p, rcTitle, flags | Qt::TextSingleLine, opt.palette, isEnabled(), d.m_textTitle, QPalette::ToolTipText);
        p.setFont(oldFont);
        rc.setTop(rc.top() + p.boundingRect(rcTitle, flags | Qt::TextSingleLine, d.m_textTitle).height());
        rc.adjust(0, 12, 0, 0);
    }

    if (drawImage && !drawImageTop)
    {
        QPoint ptIcon = rc.topLeft();
        QPixmap pm = ACommonStylePrivate::preparePixmap(d.m_icon, szDrvImage, QIcon::Normal, QIcon::On, this);
        style()->drawItemPixmap(&p, QRect(ptIcon, szDrvImage), Qt::AlignCenter, pm);
        rc.setLeft(rc.left() + szDrvImage.width() + 4);
    }

    if (d.m_label)
        d.m_label->move(rc.topLeft());
    else
        style()->drawItemText(&p, rc, flags | Qt::TextWordWrap, opt.palette, isEnabled(), d.m_text, QPalette::ToolTipText);
}

/*! \reimp */
void AToolTip::resizeEvent(QResizeEvent* event)
{
    QStyleHintReturnMask frameMask;
    QStyleOption option;
    option.initFrom(this);
    if (style()->styleHint(QStyle::SH_ToolTip_Mask, &option, this, &frameMask))
        setMask(frameMask.region);

    QFrame::resizeEvent(event);
}

/*! \reimp */
void AToolTip::mouseMoveEvent(QMouseEvent* event)
{
    A_D(AToolTip);
    if (d.m_rect.isNull())
        return;
    QPoint pos = event->globalPos();
    if (d.m_widget)
        pos = d.m_widget->mapFromGlobal(pos);
    if (!d.m_rect.contains(pos))
        hideTip();
    QFrame::mouseMoveEvent(event);
}

void AToolTip::hideTip()
{
    A_D(AToolTip);
    if (!d.m_hideTimer.isActive())
        d.m_hideTimer.start(300, this);
}

void AToolTip::closeToolTip()
{
    close(); // to trigger QEvent::Close which stops the animation
    deleteLater();
}

void AToolTip::setTipRect(QWidget* w, const QRect& r)
{
    A_D(AToolTip);
    bool update = false;
    if (!d.m_rect.isNull() && !w)
        qWarning("AToolTip::setTipRect: Cannot pass null widget if rect is set");
    else
    {
        update = d.m_widget != w;
        d.m_widget = w;
        d.m_rect = r;
    }
    if (update)
        d.updateTool();
}

/*! \reimp */
void AToolTip::timerEvent(QTimerEvent* event)
{
    A_D(AToolTip);
    if (event->timerId() == d.m_hideTimer.timerId() || event->timerId() == d.m_expireTimer.timerId())
    {
        d.m_hideTimer.stop();
        d.m_expireTimer.stop();
        closeToolTip();
    }
}

/*! \reimp */
bool AToolTip::event(QEvent* event)
{
    A_D(AToolTip);
    bool result = QFrame::event(event);
    if (event->type() == QEvent::PaletteChange)
    {
        if (d.m_label)
            d.m_label->setPalette(palette());
    }
    return result;
}

/*! \reimp */
bool AToolTip::eventFilter(QObject *o, QEvent* event)
{
    A_D(AToolTip);
    switch (event->type()) 
    {
        case QEvent::Leave:
                hideTip();
            break;
        case QEvent::WindowActivate:
        case QEvent::WindowDeactivate:
        case QEvent::MouseButtonPress:
        case QEvent::MouseButtonRelease:
        case QEvent::MouseButtonDblClick:
        case QEvent::FocusIn:
        case QEvent::FocusOut:
        case QEvent::Wheel:
                closeToolTip();
            break;
        case QEvent::MouseMove:
            if (o == d.m_widget && !d.m_rect.isNull() && !d.m_rect.contains(static_cast<QMouseEvent*>(event)->pos()))
                hideTip();
            break;
        default:
            break;
    }
    return false;
}

void AToolTip::placeTip(const QPoint& pos, QWidget* w)
{
    QRect screenRC = aproch_screenGeometry(pos, w);
    QPoint p = pos;
    if (p.x() + this->width() > screenRC.x() + screenRC.width())
        p.rx() -= 4 + this->width();
    if (p.y() + this->height() > screenRC.y() + screenRC.height())
        p.ry() -= 24 + this->height();
    if (p.y() < screenRC.y())
        p.setY(screenRC.y());
    if (p.x() + this->width() > screenRC.x() + screenRC.width())
        p.setX(screenRC.x() + screenRC.width() - this->width());
    if (p.x() < screenRC.x())
        p.setX(screenRC.x());
    if (p.y() + this->height() > screenRC.y() + screenRC.height())
        p.setY(screenRC.y() + screenRC.height() - this->height());

    this->move(p);
}

bool AToolTip::tipChanged(const QPoint& pos, const QString& text, const QString& textTitle, QObject* o)
{
    A_D(AToolTip);
    if (AToolTipPrivate::m_instance->text() != text)
        return true;

    if (AToolTipPrivate::m_instance->title() != textTitle)
        return true;

    if (o != d.m_widget)
        return true;

    if (!d.m_rect.isNull())
        return !d.m_rect.contains(pos);
    else
        return false;
}

bool AToolTip::fadingOut() const
{
    A_D(const AToolTip);
    return d.m_fadingOut;
}

APROCH_NAMESPACE_END