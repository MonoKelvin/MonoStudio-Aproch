/****************************************************************************
 * @file    ACaptionBar_p.h
 * @date    2024-06-21 
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

#include <QBoxLayout>

#include "../ACaptionBar.h"

APROCH_NAMESPACE_BEGIN

class ACaptionBarPrivate
{
public:
    ACaptionBarPrivate()
    {
    }
    ~ACaptionBarPrivate()
    {
    }

public:
    ACaptionBar* q_ptr = nullptr;

    QHBoxLayout* mainLayout = nullptr;
    QPointer<QWidget> hostWidget;
    bool isAutoTitle = true;
    bool isAutoIcon = true;

public:
    static int constexpr _widget2Index(EWindowCaptionWidget widgetType)
    {
        return int(widgetType) / 2;
    }

    static EWindowCaptionWidget constexpr _index2Widget(int index)
    {
        return index == 0 ? EWindowCaptionWidget::WindowIcon : EWindowCaptionWidget(index * 2);
    }

public:
    void init(const FWindowCaptionWidgets& widgets);

    Q_ALWAYS_INLINE QWidget* widgetAt(EWindowCaptionWidget type) const
    {
        return mainLayout->itemAt(_widget2Index(type))->widget();
    }
    void setWidgetAt(EWindowCaptionWidget type, QWidget* widget);
    QWidget* takeWidgetAt(EWindowCaptionWidget type);

    Q_ALWAYS_INLINE QLayout* appendixLayout() const
    {
        return mainLayout->itemAt(_widget2Index(EWindowCaptionWidget::WindowAppendixLayout))->layout();
    }
    void setAppendixLayout(QLayout* layout);
    QLayout* takeAppendixLayout();

    Q_ALWAYS_INLINE void insertDefaultSpace(int index)
    {
        mainLayout->insertSpacerItem(index, new QSpacerItem(0, 0));
    }

private:
    Q_DISABLE_COPY(ACaptionBarPrivate)
};

APROCH_NAMESPACE_END
