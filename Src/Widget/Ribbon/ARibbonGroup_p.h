/****************************************************************************
 * @file    ARibbonGroup_p.h
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
#pragma once

#include <QIcon>
#include "ARibbonGroup.h"
#include "ARibbonControls.h"
#include "Widget/Style/AStyleOption.h"
#include "Widget/Style/AStyleHelper.h"

class QAction;

// 
// The most of the following code is copied from Qtitan.
// 
// Qtitan Library by Developer Machines(Microsoft - Ribbon implementation for Qt.C++)
// Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
// 

APROCH_NAMESPACE_BEGIN

class ARibbonGroupOption;
class ARibbonGroupScrollButton;
class ARibbonControl;
class ARibbonGroupPopup;

class ARibbonGroupPrivate : public QObject
{
    Q_OBJECT
        A_DECLARE_PUBLIC(ARibbonGroup)
public:
    explicit ARibbonGroupPrivate();
    virtual ~ARibbonGroupPrivate();
    inline static ARibbonGroupPrivate* _get(ARibbonGroup* group);
    ARibbonPage* parentPage() const;
public:
    void init();
    void clear(bool deleteControls);
    void enableGroupScroll(bool scrollLeft, bool scrollRight);
    void showGroupScroll();

    void initStyleOption(ARibbonGroupStyleOption& opt) const;
    int calcMinimumWidth(ARibbonControlSizeDefinition::GroupSize size) const;
    int arrangeRowContent(QList<ARibbonControl*>& row, int leftOffset, int topOffset, int rowItemHeight, int rowItemCount, QWidget* parent) const;
    void reposition(const QRect& rect);
    void updateOptionButtonLayout(QWidget* parent);
    QSize updateControlsLayout(QWidget* parent, int leftOffset = 0, int topOffset = 0);
    bool adjustCurrentSize(bool expand);
    void updateLayoutParent();
    QSize sizeHint() const;
    QSize contentSizeHint() const;
    void setReductionSize(ARibbonControlSizeDefinition::GroupSize size);
    void reduce();
    void expand();
    ARibbonGroupPopup* groupPopup();
    QWidget* parentForControls();
    QList<ARibbonControl*> controlsOrdered() const;
    bool hasOverflowContent() const;
    static bool visualIndexLessThan(ARibbonControl* first, ARibbonControl* second);
    static int calcGroupTitleHeight(const QFont& titleFont);
    void showPopupMenu();
public Q_SLOTS:
    void pressLeftScrollButton();
    void pressRightScrollButton();
public:
    ARibbonGroupOption* m_optionButton;
    ARibbonGroupScrollButton* m_buttonScrollLeft;    // Button to draw left scroll
    ARibbonGroupScrollButton* m_buttonScrollRight;   // Button to draw right scroll
    ARibbonGroupPopup* m_groupPopup;
    ARibbonToolBarControl* m_toolBar;

    QList<ARibbonControl*> m_controls;
    ARibbonControlSizeDefinition::GroupSize m_currentSize;
    Qt::Alignment m_contentAlignment;
    Qt::Alignment m_controlsAlignment;
    int m_spacing;
    bool m_butOptionVisible;
    bool m_hasOverflowContent;
    QSize m_contentSizeHint;
    QAction* m_actOption;
    QString m_title;
    QFont m_titleFont;
    QColor m_titleColor;
    QIcon m_icon;
    int m_pageScrollPos;
    int m_groupScrollPos;  // Scroll position
    Qt::TextElideMode m_titleElideMode;
private:
    Q_DISABLE_COPY(ARibbonGroupPrivate)
};

inline ARibbonGroupPrivate* ARibbonGroupPrivate::_get(ARibbonGroup* group)
{
    return &group->aproch_d();
}
inline ARibbonControlSizeDefinition::GroupSize aproch_get_groupsize(ARibbonControlSizeDefinition::GroupSize& size, const ARibbonGroup* group)
{
    const bool simplified = group != nullptr && group->ribbonBar() != nullptr && group->ribbonBar()->simplifiedMode();
    if (simplified && size < ARibbonControlSizeDefinition::GroupSimplified)
        size = ARibbonControlSizeDefinition::GroupSimplified;
    if (!simplified && size > ARibbonControlSizeDefinition::GroupPopup)
        size = ARibbonControlSizeDefinition::GroupLarge;
    return size;
}

APROCH_NAMESPACE_END