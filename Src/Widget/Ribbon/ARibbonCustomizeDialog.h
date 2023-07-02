/****************************************************************************
 * @file    ARibbonCustomizeDialog.h
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
#include <QDialog>

// 
// The most of the following code is copied from Qtitan.
// 
// Qtitan Library by Developer Machines(Microsoft - ARibbon implementation for Qt.C++)
// Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
// 

APROCH_NAMESPACE_BEGIN

class ARibbonCustomizeDialogPrivate;

/* ARibbonCustomizeDialog */
class APROCH_API ARibbonCustomizeDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ARibbonCustomizeDialog(QWidget* parent = nullptr);
    virtual ~ARibbonCustomizeDialog();
public:
    void addPage(QWidget* page);
    void insertPage(int index, QWidget* page);
    int indexOf(QWidget* page) const;

    QWidget* currentPage() const;
    void setCurrentPage(QWidget* page);

    int currentPageIndex() const;
    void setCurrentPageIndex(int index);

    int pageCount() const;
    QWidget* pageByIndex(int index) const;
public:
    virtual void accept();
    virtual void reject();
protected:
    virtual void showEvent(QShowEvent* event);
    virtual void hideEvent(QHideEvent* event);
private:
    A_DECLARE_PRIVATE(ARibbonCustomizeDialog)
    Q_DISABLE_COPY(ARibbonCustomizeDialog)
};

APROCH_NAMESPACE_END