/****************************************************************************
 * @file    ARibbonButton_p.h
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
#include <QStyle>
#include <QToolButton>
#include <QStringList>

#include "ARibbonButton.h"

 // 
 // The most of the following code is copied from Qtitan.
 // 
 // Qtitan Library by Developer Machines(Microsoft - Ribbon implementation for Qt.C++)
 // Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
 // 

APROCH_NAMESPACE_BEGIN

class ARibbonButtonPrivate : public QObject
{
public:
    A_DECLARE_PUBLIC(ARibbonButton)
public:
    ARibbonButtonPrivate();
protected:
    void resetWordWrap();
    QSize sizeWordWrap();
    bool m_wordWrap;
    bool m_largeIcon;
    bool m_simplifiedMode;
    int m_wrapIndex;
    QSize m_sizeHint;
    QSize m_sizeWordWrap;
    Qt::ToolButtonStyle m_saveButtonStyle;
    Qt::ToolButtonStyle m_simplifiedButtonStyle;
    Qt::ArrowType m_saveArrowType;
    QVector<int> m_wordIndexes;
};

APROCH_NAMESPACE_END