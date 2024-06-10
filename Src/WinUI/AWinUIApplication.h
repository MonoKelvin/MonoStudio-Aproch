/****************************************************************************
 * @file    AWinUIApplication.h
 * @date    2024-06-09 
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
#include "../AprochGlobal.h"
#include "../AprochExportDefine.h"

#include <QCoreApplication>
#include <QQmlApplicationEngine>
#include <QtQml/qqml.h>

APROCH_NAMESPACE_BEGIN

class AWinUIApplicationPrivate;

/**
 * @brief winui应用程序
 */
class APROCH_API AWinUIApplication : public QObject
{
    Q_OBJECT;

    A_DEFINE_MEMBER(bool, isUseSystemAppBar);
    A_DEFINE_MEMBER(QString, windowIcon);
    A_DEFINE_MEMBER(QLocale, locale);
    A_DEFINE_MEMBER_PTR(QObject*, context);
    QML_NAMED_ELEMENT(AWinUIApplication);
    QML_SINGLETON

public:
    AWinUIApplication(int& argc, char** argv);
    ~AWinUIApplication();

    /** @brief 获取应用实例 */
    static AWinUIApplication* getInstance();

    /** @brief 创建qml实例 */
    static AWinUIApplication* create(QQmlEngine*, QJSEngine*);
    
    /** @brief 初始化应用实例（qml中调用） */
    Q_INVOKABLE void init(QObject* qmlContext, const QLocale& locale = QLocale::system());

    /** @brief 加载qml首页并运行 */
    int exec(const QUrl& url);

private:
    Q_DISABLE_COPY_MOVE(AWinUIApplication);
    QScopedPointer<AWinUIApplicationPrivate> d;
};

APROCH_NAMESPACE_END