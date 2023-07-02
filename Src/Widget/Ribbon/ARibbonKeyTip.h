/****************************************************************************
 * @file    ARibbonKeyTip_p.h
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
#pragma once
#include <QFrame>
#include <QTimer>
#include <QIcon>
#include <QEvent>

 // 
 // The most of the following code is copied from Qtitan.
 // 
 // Qtitan Library by Developer Machines(Microsoft - ARibbon implementation for Qt.C++)
 // Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
 // 

APROCH_NAMESPACE_BEGIN

class ARibbonKeyTipPrivate;
/* KeyTip */
class APROCH_API ARibbonKeyTip : public QFrame
{
    Q_OBJECT
public:
    enum AlignTip
    {
        AlignTipTop = 0x00000000,
        AlignTipLeft = 0x00000000,
        AlignTipCenter = 0x00000001,
        AlignTipRight = 0x00000002,
        AlignTipVCenter = 0x00000004,
        AlignTipBottom = 0x00000008,
    };
public:
    ARibbonKeyTip(QWidget* bars, QWidget* owner, const QString& caption,
                  const QPoint& pt, uint align, bool enabled, QAction* action = nullptr);
    virtual ~ARibbonKeyTip();
public:
    QPoint posTip() const;
    void setVisibleTip(bool visible);
    bool isVisibleTip() const;
    bool isEnabledTip() const;

    QWidget* getBars() const;
    uint getAlign() const;

    void setExplicit(bool exp);
    bool isExplicit() const;

    QString getStringTip() const;
    void setStringTip(const QString& str);

    QString getCaption() const;
    void setCaption(const QString& str);

    QString getPrefix() const;
    void setPrefix(const QString& pref);

    QWidget* getOwner() const;
    QAction* getAction() const;
    void hideAndDestroy();
    void setVisible(bool visible) override;
protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
private:
    A_DECLARE_PRIVATE(ARibbonKeyTip);
    Q_DISABLE_COPY(ARibbonKeyTip);
};

APROCH_NAMESPACE_END