/****************************************************************************
 * @file    AWindowsStyleManager.cpp
 * @date    2022-05-29
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
#include "ATheme.h"

#include <QApplication>

#ifdef Q_OS_WIN
#include "AWinUIStyleHelper.h"
#endif // Q_OS_WIN


APROCH_NAMESPACE_BEGIN

ATheme* theAppTheme = nullptr;

class AThemePrivate
{
public:
    EThemeType themeType = EThemeType::System;
    bool systemThemeWatchEnabled = false;
};

bool ATheme::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::ThemeChange)
    {
        if (d_ptr->systemThemeWatchEnabled)
        {
            // TODO
            /*auto sysTheme = getSystemTheme();

            QString qss = sysTheme == EThemeType::Dark ? "dark.qss" : "light.qss";

            QFile style(QApplication::applicationDirPath() + AStr("/../../../../Src/Resource/theme/%1").arg(qss));
            if (style.open(QFile::ReadOnly | QFile::Text))
                qApp->setStyleSheet(style.readAll());
            style.close();*/

            emit themeChanged();
        }
    }

    return false;
}

ATheme::ATheme(QObject* parent)
    : QObject(parent)
    , d_ptr(new AThemePrivate())
{
}

ATheme::~ATheme()
{
}

ATheme* ATheme::instance()
{
    if (!theAppTheme)
    {
        theAppTheme = new ATheme();
        qApp->installEventFilter(theAppTheme);
    }
    return theAppTheme;
}

void ATheme::setTheme(EThemeType type)
{
    auto theAppTheme = instance();
    if (instance()->d_ptr->themeType == type)
        return;
    theAppTheme->d_ptr->themeType = type;

    // TODO

    emit theAppTheme->themeChanged();
}

EThemeType ATheme::getTheme()
{
    return instance()->d_ptr->themeType;
}

EThemeType ATheme::getSystemTheme()
{
#ifdef Q_OS_WIN
    return AWinUIStyleHelper::getThemeConfig().systemUsesLightTheme ? EThemeType::Light : EThemeType::Dark;
#endif // Q_OS_WIN

    return EThemeType::System;
}

void ATheme::setSystemThemeWatchEnabled(bool enabled)
{
    instance()->d_ptr->systemThemeWatchEnabled = enabled;
}

APROCH_NAMESPACE_END
