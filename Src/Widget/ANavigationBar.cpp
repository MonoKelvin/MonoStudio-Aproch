/****************************************************************************
 * @file    ACaptionBar.cpp
 * @date    2021-10-27
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
#include "ANavigationBar.h"

namespace aproch
{
    ANavigationBar::ANavigationBar(QWidget* parent)
        : QAbstractScrollArea(parent)
    {
        _init(Qt::Vertical);
    }

    ANavigationBar::ANavigationBar(Qt::Orientation ori, QWidget* parent)
        : QAbstractScrollArea(parent)
    {
        _init(ori);
    }

    ANavigationBar::~ANavigationBar()
    {
    }

    int ANavigationBar::getHScrollValue() const noexcept
    {
        return horizontalScrollBar()->value();
    }

    void ANavigationBar::setHScrollValue(int value)
    {
        horizontalScrollBar()->setValue(value);
        _updateContentPosition();
    }

    int ANavigationBar::getVScrollValue() const noexcept
    {
        return verticalScrollBar()->value();
    }

    void ANavigationBar::setVScrollValue(int value)
    {
        verticalScrollBar()->setValue(value);
        _updateContentPosition();
    }

    void ANavigationBar::addItem(const QString& text, const QIcon& icon, int index)
    {
        QPushButton* btn = new QPushButton(icon, text, this);

        btn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        btn->setCheckable(true);
        btn->setObjectName(AOBJNAME_NAVBAR_ITEM);

#ifndef QT_NO_CURSOR
        btn->setCursor(Qt::PointingHandCursor);
#endif

#ifndef QT_NO_TOOLTIP
        btn->setToolTip(btn->text());
#endif

        mLayout->insertWidget(index, btn);
        mNavGroup->addButton(btn);

        _updateItemId();
    }

    int ANavigationBar::getItemCount() const noexcept
    {
        return mNavGroup->buttons().size();
    }

    void ANavigationBar::setItemSize(int w, int h, int index)
    {
        const int len = mNavGroup->buttons().size();
        if (index < 0 || index >= len)
        {
            if (getOrientation() == Qt::Vertical)
            {
                for (auto& btn : mNavGroup->buttons())
                {
                    btn->setFixedHeight(h);
                }
            }
            else
            {
                for (auto& btn : mNavGroup->buttons())
                {
                    btn->setFixedWidth(w);
                }
            }
        }
        else
        {
            QAbstractButton* btn = mNavGroup->button(index);
            if (getOrientation() == Qt::Vertical)
            {
                btn->setFixedHeight(h);
            }
            else
            {
                btn->setFixedWidth(w);
            }
        }
    }

    void ANavigationBar::setItemEnabled(int index, bool isEnabled)
    {
        QAbstractButton* btn = mNavGroup->button(index);
        if (nullptr != btn)
        {
            btn->setEnabled(isEnabled);
        }
    }

    void ANavigationBar::removeItem(int index)
    {
        auto& item = mNavGroup->buttons().at(index);
        if (nullptr != item)
        {
            mNavGroup->buttons().removeAt(index);
            item->deleteLater();

            _updateItemId();
        }
    }

    QPushButton* ANavigationBar::getItem(int index)
    {
        return qobject_cast<QPushButton*>(mNavGroup->button(index));
    }

    QPushButton* ANavigationBar::getCurrentItem() const
    {
        return qobject_cast<QPushButton*>(mNavGroup->checkedButton());
    }

    int ANavigationBar::getCurrentIndex(void) const
    {
        const auto& buttons = mNavGroup->buttons();
        for (int i = 0; i < buttons.length(); ++i)
        {
            if (mNavGroup->button(i)->isChecked())
                return i;
        }

        return -1;
    }

    Qt::Orientation ANavigationBar::getOrientation() const
    {
        return (mLayout->direction() == QBoxLayout::TopToBottom ||
                mLayout->direction() == QBoxLayout::BottomToTop)
            ? Qt::Vertical
            : Qt::Horizontal;
    }

    void ANavigationBar::setSpacing(int spacing)
    {
        mLayout->setSpacing(spacing);
    }

    int ANavigationBar::getSpacing() const
    {
        return mLayout->spacing();
    }

    void ANavigationBar::setMargins(int margins)
    {
        mLayout->setMargin(margins);
    }

    void ANavigationBar::setMargins(int left, int top, int right, int bottom)
    {
        mLayout->setContentsMargins(left, top, right, bottom);
    }

    QMargins ANavigationBar::getMargins() const
    {
        return mLayout->contentsMargins();
    }

    void ANavigationBar::navigate(int index)
    {
        const int len = mNavGroup->buttons().length();
        if (index >= 0 && index < len && index != mPrevIndex)
        {
            mNavGroup->button(index)->toggle();
            updateTrackBarStyle(false);
            mPrevIndex = index;
            emit navigated(index);
        }
    }

    bool ANavigationBar::event(QEvent* e)
    {
        if (e->type() == QEvent::StyleChange || e->type() == QEvent::LayoutRequest)
        {
            _updateScrollBars();
        }

        return QAbstractScrollArea::event(e);
    }

    bool ANavigationBar::eventFilter(QObject* o, QEvent* e)
    {
        if (o == mContent)
        {
            switch (e->type())
            {
            case QEvent::Resize:
                _updateScrollBars();
                break;
            case QEvent::Wheel:
            {
                const auto& wheelEvt = static_cast<QWheelEvent*>(e);
                const auto scrollValue = wheelEvt->angleDelta() / 6;
                if (getOrientation() == Qt::Vertical)
                {
                    auto vbar = verticalScrollBar();
                    vbar->setValue(vbar->value() - scrollValue.y());
                }
                else
                {
                    auto hbar = horizontalScrollBar();
                    hbar->setValue(hbar->value() - scrollValue.y());
                }
            }
            break;
            default:
                break;
            }
        }

        return QAbstractScrollArea::eventFilter(o, e);
    }

    void ANavigationBar::resizeEvent(QResizeEvent* event)
    {
        _updateScrollBars();

        return QAbstractScrollArea::resizeEvent(event);
    }

    void ANavigationBar::scrollContentsBy(int, int)
    {
        _updateContentPosition();
    }

    void ANavigationBar::showEvent(QShowEvent* e)
    {
        int index = mNavGroup->checkedId();
        index = index == -1 ? 0 : index;

        const int len = mNavGroup->buttons().length();
        if (index >= 0 && index < len && index != mPrevIndex)
        {
            mNavGroup->button(index)->toggle();
            updateTrackBarStyle();
            mPrevIndex = index;
            emit navigated(index);
        }

        mTrackBar->raise();

        return QAbstractScrollArea::showEvent(e);
    }

    int ANavigationBar::getItemDistance(int index, bool isCenter)
    {
        if (index < 0)
        {
            index = mNavGroup->checkedId();
            if (index < 0)
            {
                return -1;
            }
        }

        int dist = index * mLayout->spacing();

        if (getOrientation() == Qt::Vertical)
        {
            dist += mLayout->contentsMargins().top();

            for (int i = 0; i < index; ++i)
            {
                dist += mNavGroup->button(i)->height();
            }

            if (isCenter)
            {
                dist += mNavGroup->button(index)->height() / 2;
            }
        }
        else
        {
            dist += mLayout->contentsMargins().left();

            for (int i = 0; i < index; ++i)
            {
                dist += mNavGroup->button(i)->width();
            }

            if (isCenter)
            {
                dist += mNavGroup->button(index)->width() / 2;
            }
        }

        return dist;
    }

    int ANavigationBar::getItemOffset(int index, bool isCenter)
    {
        if (getOrientation() == Qt::Vertical)
        {
            return getItemDistance(index, isCenter) + mContent->y();
        }
        else
        {
            return getItemDistance(index, isCenter) + mContent->x();
        }
    }

    void ANavigationBar::setBoundaryOffset(int value)
    {
        if (!mNavGroup->buttons().isEmpty())
        {
            const int count = mNavGroup->buttons().size();
            int minValue = 0;
            if (getOrientation() == Qt::Vertical)
            {
                minValue = qMin(mNavGroup->button(0)->height(), mNavGroup->button(count - 1)->height());
            }
            else
            {
                minValue = qMin(mNavGroup->button(0)->width(), mNavGroup->button(count - 1)->width());
            }
            mBoundaryOffset = qMin(minValue, value);
        }
    }

    void ANavigationBar::scroll(int h, int v)
    {
        QPropertyAnimation* hAni = nullptr;
        if (h != 0)
        {
            const int hCurValue = horizontalScrollBar()->value();
            hAni = new QPropertyAnimation(this, QByteArrayLiteral("hScrollValue"));
            hAni->setEasingCurve(QEasingCurve::Type(mTrackBarStyle.animationType));
            hAni->setDuration(mTrackBarStyle.duration);
            hAni->setStartValue(hCurValue);
            hAni->setEndValue(qMax(0, qMin(hCurValue - h, horizontalScrollBar()->maximum())));
        }

        QPropertyAnimation* vAni = nullptr;
        if (v != 0)
        {
            const int vCurValue = verticalScrollBar()->value();
            vAni = new QPropertyAnimation(this, QByteArrayLiteral("vScrollValue"));
            vAni->setEasingCurve(QEasingCurve::Type(mTrackBarStyle.animationType));
            vAni->setDuration(mTrackBarStyle.duration);
            vAni->setStartValue(vCurValue);
            vAni->setEndValue(qMax(0, qMin(vCurValue - v, verticalScrollBar()->maximum())));
        }

        if (nullptr != vAni && nullptr != hAni)
        {
            QParallelAnimationGroup* group = new QParallelAnimationGroup;
            group->addAnimation(hAni);
            group->addAnimation(vAni);
            group->start(QAbstractAnimation::DeleteWhenStopped);
        }
        else if (nullptr != vAni)
        {
            vAni->start(QAbstractAnimation::DeleteWhenStopped);
        }
        else if (nullptr != hAni)
        {
            hAni->start(QAbstractAnimation::DeleteWhenStopped);
        }
    }

    void ANavigationBar::_init(Qt::Orientation ori)
    {
        viewport()->setBackgroundRole(QPalette::NoRole);
        horizontalScrollBar()->setSingleStep(15);
        verticalScrollBar()->setSingleStep(15);
        horizontalScrollBar()->setValue(0);
        verticalScrollBar()->setValue(0);
        setObjectName(AOBJNAME_NAVBAR);

        mBoundaryOffset = 20;
        mPrevIndex = -1;
        mContent = new QWidget(this);
        mNavGroup = new QButtonGroup(this);
        mTrackBar = new QWidget(mContent);
        mTrackBar->resize(4, 4);
        mTrackBar->setVisible(false);
        mTrackBar->setObjectName(AOBJNAME_NAVBAR_TRACKBAR);

        if (ori == Qt::Vertical)
        {
            setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
            mLayout = new QBoxLayout(QBoxLayout::TopToBottom, mContent);
        }
        else
        {
            setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
            mLayout = new QBoxLayout(QBoxLayout::LeftToRight, mContent);
        }

        mLayout->setSpacing(0);
        mLayout->setMargin(0);
        mLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);

        mContent->installEventFilter(this);

#if (QT_VERSION >= QT_VERSION_CHECK(5, 15, 0))
        connect(mNavGroup, &QButtonGroup::idClicked, this, &ANavigationBar::navigate);
#else
        connect(mNavGroup, static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked),
                this, &ANavigationBar::navigate);
#endif

        mContent->raise();
        mTrackBar->raise();
    }

    void ANavigationBar::_updateItemId()
    {
        const int len = mNavGroup->buttons().length();
        for (int i = 0; i < len; ++i)
        {
            mNavGroup->setId(mNavGroup->buttons().at(i), i);
        }
        updateTrackBarStyle();
    }

    void ANavigationBar::updateTrackBarStyle(bool stopAnimation)
    {
        mTrackBar->setVisible(mTrackBarStyle.isEnabled);

        if (!mTrackBarStyle.isEnabled)
        {
            return;
        }

        if (mNavGroup->checkedId() < 0)
        {
            mTrackBar->setVisible(false);
        }
        else
        {
            const int dist = _updateItemPosition(mNavGroup->checkedId());
            QRect geo = mTrackBar->geometry();
            int x = 0, y = 0;

            if (getOrientation() == Qt::Vertical)
            {
                if (mTrackBarStyle.isFitItem)
                {
                    geo.setHeight(getCurrentItem()->height());
                }
                x = ((mTrackBarStyle.offset >= 0) ? 0 : (mContent->width() - mTrackBar->width() + 1))
                    + mTrackBarStyle.offset;
                y = dist - mContent->y() - geo.height() / 2;

            }
            else
            {
                if (mTrackBarStyle.isFitItem)
                {
                    geo.setWidth(getCurrentItem()->width());
                }
                x = dist - mContent->x() - geo.width() / 2;
                y = ((mTrackBarStyle.offset < 0) ? 1 : (mContent->height() - mTrackBar->height()))
                    + mTrackBarStyle.offset;
            }
            geo.moveTopLeft(QPoint(x, y));

            if (!stopAnimation && mTrackBarStyle.isAnimation)
            {
                if (geo == mTrackBar->geometry())
                {
                    return;
                }

                QPropertyAnimation* ani = new QPropertyAnimation(mTrackBar, QByteArrayLiteral("geometry"));
                ani->setEasingCurve(QEasingCurve::Type(mTrackBarStyle.animationType));
                ani->setDuration(mTrackBarStyle.duration);
                ani->setStartValue(mTrackBar->geometry());
                ani->setEndValue(geo);
                ani->start(QAbstractAnimation::DeleteWhenStopped);
            }
            else
            {
                mTrackBar->setGeometry(geo);
            }
        }
    }

    void ANavigationBar::_updateScrollBars(void)
    {
        QSize p = viewport()->size();
        //const auto& m = maximumViewportSize();
        const auto& min = mContent->minimumSize();
        const auto& max = mContent->maximumSize();

        /*if (mLayout->hasHeightForWidth())
        {
            const QSize p_hfw = p.expandedTo(min).boundedTo(max);
            const int h = mContent->heightForWidth(p_hfw.width());
            min = QSize(p_hfw.width(), qMax(p_hfw.height(), h));
        }
        if (m.expandedTo(min) == m && m.boundedTo(max) == m)
        {
            p = m;    // no scroll bars needed
        }*/

        const auto& v = p.expandedTo(min).boundedTo(max);
        if (getOrientation() == Qt::Vertical)
        {
            mContent->resize(viewport()->width(), mContent->height());
        }
        else
        {
            mContent->resize(mContent->width(), viewport()->height());
        }

        horizontalScrollBar()->setRange(0, v.width() - p.width());
        horizontalScrollBar()->setPageStep(p.width());
        verticalScrollBar()->setRange(0, v.height() - p.height());
        verticalScrollBar()->setPageStep(p.height());

        _updateContentPosition();
        updateTrackBarStyle();
    }

    void ANavigationBar::_updateContentPosition()
    {
        const auto& hbar = horizontalScrollBar();
        const auto& vbar = verticalScrollBar();

        const Qt::LayoutDirection dir = layoutDirection();
        const QRect scrolled = QStyle::visualRect(dir, viewport()->rect(),
                                                    QRect(QPoint(-hbar->value(), -vbar->value()), mContent->size()));
        const QRect aligned = QStyle::alignedRect(dir, Qt::AlignLeft | Qt::AlignTop,
                                                    mContent->size(), viewport()->rect());
        mContent->move(mContent->width() < viewport()->width() ? aligned.x() : scrolled.x(),
                        mContent->height() < viewport()->height() ? aligned.y() : scrolled.y());
    }

    int ANavigationBar::_updateItemPosition(int index)
    {
        const auto& btn = mNavGroup->button(index);
        Q_ASSERT(nullptr != btn);

        const int offset = getItemOffset(index, false);
        if (mBoundaryOffset < 0)
        {
            return offset;
        }

        int boundary = mBoundaryOffset;
        int scrollValue = 0;

        if (getOrientation() == Qt::Vertical)
        {
            if (index == 0)
            {
                boundary = mLayout->contentsMargins().top();
            }
            else if (index == getItemCount() - 1)
            {
                boundary = mLayout->contentsMargins().bottom();
            }

            if (offset < boundary)
            {
                scrollValue = boundary - offset;
            }
            else if (offset + boundary + btn->height() > height())
            {
                scrollValue = height() - offset - btn->height() - boundary;
            }

            scroll(0, scrollValue);
            return offset + btn->height() / 2;
        }
        else
        {
            if (index == 0)
            {
                boundary = mLayout->contentsMargins().left();
            }
            else if (index == getItemCount() - 1)
            {
                boundary = mLayout->contentsMargins().right();
            }

            if (offset < boundary)
            {
                scrollValue = boundary - offset;
            }
            else if (offset + boundary + btn->width() > width())
            {
                scrollValue = width() - offset - btn->width() - boundary;
            }

            scroll(scrollValue, 0);
            return offset + btn->width() / 2;
        }
    }
}
