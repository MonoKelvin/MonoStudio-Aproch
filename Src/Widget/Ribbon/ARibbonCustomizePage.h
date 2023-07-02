/****************************************************************************
 * @file    ARibbonCustomizePage.h
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
#include "ARibbonCustomizeDialog.h"

 // 
 // The most of the following code is copied from Qtitan.
 // 
 // Qtitan Library by Developer Machines(Microsoft - Ribbon implementation for Qt.C++)
 // Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
 // 

APROCH_NAMESPACE_BEGIN

class ARibbonBar;
class ARibbonQuickAccessBarCustomizePagePrivate;
/* ARibbonQuickAccessBarCustomizePage */
class APROCH_API ARibbonQuickAccessBarCustomizePage : public QWidget
{
    Q_OBJECT
public:
    explicit ARibbonQuickAccessBarCustomizePage(ARibbonBar* ribbonBar);
    virtual ~ARibbonQuickAccessBarCustomizePage();
public:
    ARibbonBar* ribbonBar() const; 
    void addCustomCategory(const QString& strCategory);
    void addSeparatorCategory(const QString& strCategory = QString());
public Q_SLOTS:
    void accepted();
protected:
    virtual void showEvent(QShowEvent* event);
private:
    A_DECLARE_PRIVATE(ARibbonQuickAccessBarCustomizePage)
    Q_DISABLE_COPY(ARibbonQuickAccessBarCustomizePage)
};

class ARibbonBarCustomizePagePrivate;
/* ARibbonBarCustomizePage */ 
class APROCH_API ARibbonBarCustomizePage : public QWidget
{
    Q_OBJECT
public:
    explicit ARibbonBarCustomizePage(ARibbonBar* ribbonBar);
    virtual ~ARibbonBarCustomizePage();
public:
    ARibbonBar* ribbonBar() const; 
    void addCustomCategory(const QString& strCategory);
    void addSeparatorCategory(const QString& strCategory = QString());

    /*
    * @brief 是否允许修改页面
    */
    void enableModifyPage(bool bEnable = true);

public Q_SLOTS:
    void accepted();
    void rejected();
protected:
    virtual void showEvent(QShowEvent* event);
    virtual void hideEvent(QHideEvent* event);
private:
    A_DECLARE_PRIVATE(ARibbonBarCustomizePage)
    Q_DISABLE_COPY(ARibbonBarCustomizePage)
};

APROCH_NAMESPACE_END