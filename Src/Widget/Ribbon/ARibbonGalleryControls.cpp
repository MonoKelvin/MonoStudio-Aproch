/****************************************************************************
 * @file    ARibbonGalleryControls.cpp
 * @date    2023-07-02 
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
#include "ARibbonGalleryControls.h"
#include "ARibbonButton_p.h"
#include "ARibbonGroup.h"

 // 
 // The most of the following code is copied from Qtitan.
 // 
 // Qtitan Library by Developer Machines(Microsoft - ARibbon implementation for Qt.C++)
 // Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
 // 

APROCH_NAMESPACE_BEGIN

ARibbonGalleryControl::ARibbonGalleryControl(ARibbonGroup* parentGroup)
    : ARibbonWidgetControl(parentGroup, true/*ignoreActionSettings*/)
    , m_marginTop(0) 
    , m_marginBottom(0)

{
    setContentsMargins(3, 3);
    // 6 3-5 2 6
    sizeDefinition(ARibbonControlSizeDefinition::GroupLarge)->setMinimumItemCount(6);
    sizeDefinition(ARibbonControlSizeDefinition::GroupLarge)->setMaximumItemCount(6);

    sizeDefinition(ARibbonControlSizeDefinition::GroupMedium)->setMinimumItemCount(3);
    sizeDefinition(ARibbonControlSizeDefinition::GroupMedium)->setMaximumItemCount(5);

    sizeDefinition(ARibbonControlSizeDefinition::GroupSmall)->setMinimumItemCount(2);
    sizeDefinition(ARibbonControlSizeDefinition::GroupSmall)->setMaximumItemCount(2);

    sizeDefinition(ARibbonControlSizeDefinition::GroupPopup)->setMinimumItemCount(6);
    sizeDefinition(ARibbonControlSizeDefinition::GroupPopup)->setMaximumItemCount(6);

    sizeDefinition(ARibbonControlSizeDefinition::GroupSimplifiedOverflow)->setVisible(false);

    ARibbonGallery* gallery = new ARibbonGallery(this);
    gallery->setGeometry(0, 0, 0, 0);
    setContentWidget(gallery);
    widget()->updatelayout();
}

ARibbonGalleryControl::~ARibbonGalleryControl()
{
}

ARibbonGallery* ARibbonGalleryControl::widget() const
{
    return qobject_cast<ARibbonGallery*>(contentWidget());
}

void ARibbonGalleryControl::setContentsMargins(int top, int bottom)
{
    m_marginTop = top;
    m_marginBottom = bottom;
}

QSize ARibbonGalleryControl::sizeHint() const
{
    QSize sz = ARibbonWidgetControl::sizeHint();
    if (widget() && parentGroup())
    {
        if (ARibbonBar* ribbonBar = parentGroup()->ribbonBar())
        {
            if (widget()->rowCount() == -1)
                sz.setHeight(ribbonBar->rowItemHeight() * ribbonBar->rowItemCount() - (m_marginTop + m_marginBottom));
        }
        
        int count = widget()->columnCount();
        if (count > 0 && widget()->itemCount() > 0)
        {
            ARibbonGalleryItem* item = widget()->item(0);
            QSize szItem = item->sizeHint();
            Q_ASSERT(!szItem.isNull());
            sz.setWidth(count * szItem.width() + widget()->borders().width() + 2);
        }
        else
        {
            sz.rwidth() = qMin(maximumWidth(), sz.width());
            sz.rheight() = qMin(maximumHeight(), sz.height());
            sz.rwidth() = qMax(minimumWidth(), sz.width());
            sz.rheight() = qMax(minimumHeight(), sz.height());
        }
    }
    return sz;
}

bool ARibbonGalleryControl::adjustCurrentSize(bool expand)
{
    ARibbonControlSizeDefinition::GroupSize sizeDef = currentSize();
    if (sizeDefinition(sizeDef)->isVisible() &&
        sizeDef == ARibbonControlSizeDefinition::GroupSimplifiedOverflow)
    {
        int min = sizeDefinition(sizeDef)->minimumItemCount();
        if (min != -1)
        {
            widget()->setColumnCount(min);
            return true;
        }
    }

    int min = sizeDefinition(sizeDef)->minimumItemCount();
    int max = sizeDefinition(sizeDef)->maximumItemCount();
    int current = widget()->columnCount();
    const int old = current;
    if (expand)
        current++;
    else
        current--;
    current = qMin(max, current);
    current = qMax(min, current);
    widget()->setColumnCount(current);

    if (old != widget()->columnCount())
        return true;
    return ARibbonWidgetControl::adjustCurrentSize(expand);
 }

void ARibbonGalleryControl::sizeChanged(ARibbonControlSizeDefinition::GroupSize size)
{
    int minCount = sizeDefinition(ARibbonControlSizeDefinition::GroupSimplifiedOverflow)->minimumItemCount();
    if ((minCount != -1 || sizeDefinition(size)->isVisible()) && size == ARibbonControlSizeDefinition::GroupSimplifiedOverflow)
    {
        size = ARibbonControlSizeDefinition::GroupSimplified;
    }

    int min = sizeDefinition(size)->minimumItemCount();
    int max = sizeDefinition(size)->maximumItemCount();

    if (size > currentSize())
        widget()->setColumnCount(max);
    else
        widget()->setColumnCount(min);

    ARibbonWidgetControl::sizeChanged(size);
}

void ARibbonGalleryControl::resizeEvent(QResizeEvent* event)
{
    ARibbonWidgetControl::resizeEvent(event);
    widget()->updatelayout();
}

APROCH_NAMESPACE_END