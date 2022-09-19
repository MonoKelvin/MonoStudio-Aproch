/****************************************************************************
 * @file    ABoxLayoutCreator.cpp
 * @date    2022-07-07
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
#include "ABoxLayoutCreator.h"

namespace aproch
{
    const QString aStr_VLayout = "vlayout";
    const QString aStr_HLayout = "hlayout";
    const QString aStr_BoxLayout = "boxlayout";
    const QString aStr_Layout = "layout";
    const QString aStr_Direction = "direction";

    ABoxLayoutCreator::ABoxLayoutCreator()
    {
        addName(aStr_VLayout);
        addName(aStr_HLayout);
        addName(aStr_BoxLayout);
        addName(aStr_Layout);
        addName(aStr_Direction);
    }

    QObject *ABoxLayoutCreator::createObject(const QString &name, aproch::IDPOData &data, QObject *parent)
    {
        const QString objName = name.trimmed().toLower();
        ADPWData &dpwData = (ADPWData &)data;

        QBoxLayout *layout = new QBoxLayout(QBoxLayout::LeftToRight, qobject_cast<QWidget *>(parent));

        // direction
        if (objName == aStr_VLayout)
            layout->setDirection(QBoxLayout::TopToBottom);
        else if (objName == aStr_BoxLayout || objName == aStr_Layout)
        {
            const QString direction = dpwData.value(aStr_Direction).toLower();
            if (direction.isEmpty() || direction == "lefttoright" || direction == "l2r")
                layout->setDirection(QBoxLayout::LeftToRight);
            else if (direction == "righttoleft" || direction == "r2l")
                layout->setDirection(QBoxLayout::RightToLeft);
            else if (direction == "toptobottom" || direction == "t2b")
                layout->setDirection(QBoxLayout::TopToBottom);
            else if (direction == "bottomtotop" || direction == "b2t")
                layout->setDirection(QBoxLayout::BottomToTop);
            else
            {
                // todo: log
                qWarning(QString("Unrecognized attribute value \'%1\' for \'%2\'")
                                .arg(direction)
                                .arg(objName)
                                .toLatin1());
            }
        }

        return layout;
    }
}
