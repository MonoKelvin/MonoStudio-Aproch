/****************************************************************************
 * @file    AAbstractEditorFactory.h
 * @date    2022-7-16
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

APROCH_NAMESPACE_BEGIN

class ADataWidgetBinding;
class AAbstractEditorFactoryPrivate;

/**
 * @brief 用于创建数据编辑控件的抽象工厂基类
 */
class APROCH_API AAbstractEditorFactory : public QObject
{
    Q_OBJECT
public:
    explicit AAbstractEditorFactory(ADataWidgetBinding* dwb, QObject* parent = nullptr);
    ~AAbstractEditorFactory();

    /** @brief 创建绑定给定数据的控件 */
    QWidget* createEditor(AData* data, QWidget* parent, const QString& bindPropName = QString(),
                          EDataBindType type = EDataBindType::TwoWay);

protected:
    virtual QWidget* createEditorImpl(AData* data, QWidget* parent, const QString& bindPropName, EDataBindType type) = 0;

private:
    Q_DISABLE_COPY_MOVE(AAbstractEditorFactory);
    Q_DECLARE_PRIVATE(AAbstractEditorFactory);
};

APROCH_NAMESPACE_END
