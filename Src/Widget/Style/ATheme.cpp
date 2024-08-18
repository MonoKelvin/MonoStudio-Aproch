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
    QByteArray customThemeData;
    QString customThemeFile;
    EThemeType themeType = EThemeType::System;
    bool systemThemeWatchEnabled = false;
};

bool ATheme::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::ThemeChange)
    {
        if (d_ptr->systemThemeWatchEnabled && d_ptr->themeType == EThemeType::System)
        {
            reload();
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

ATheme* ATheme::getInstance()
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
    auto theAppTheme = getInstance();
    if (theAppTheme->d_ptr->themeType == type)
        return;

    theAppTheme->d_ptr->themeType = type;

    // reload theme
    theAppTheme->reload();

    emit theAppTheme->themeChanged();
}

bool ATheme::setCustomThemeFile(const QString& strFileName)
{
    QFile file(strFileName);
    if (!file.open(QFile::ReadOnly))
        return false;

    auto theAppTheme = getInstance();
    theAppTheme->d_ptr->customThemeData = file.readAll();
    theAppTheme->d_ptr->customThemeFile = strFileName;
    if (theAppTheme->d_ptr->customThemeData.isEmpty())
        return false;

    return true;
}

EThemeType ATheme::getTheme()
{
    return getInstance()->d_ptr->themeType;
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
    getInstance()->d_ptr->systemThemeWatchEnabled = enabled;
}

void ATheme::reload()
{
    EThemeType themeType = ATheme::getTheme();

    QByteArray themeData;
    if (themeType == EThemeType::Custom)
    {
        QFile customThemeFile(d_ptr->customThemeFile);
        if (!customThemeFile.open(QFile::ReadOnly | QFile::Text))
        {
            qWarning() << "custom theme file \"" << d_ptr->customThemeFile << "\" is missing,"
                " reading cached data";
            themeData = d_ptr->customThemeData;
        }
        else
            themeData = customThemeFile.readAll();
        customThemeFile.close();
    }
    else
    {
        QString themeResFile = "Dark";
        switch (themeType)
        {
        case EThemeType::Light:
        {
            themeResFile = "Light";
            break;
        }
        case EThemeType::System:
        {
            if (getSystemTheme() == EThemeType::Light)
                themeResFile = "Light";
            break;
        }
        default:
            break;
        }

        QByteArray readThemeData;
        QFile qssFile(AStr(":/theme/%1.qss").arg(themeResFile));
        if (qssFile.open(QFile::ReadOnly | QFile::Text))
            readThemeData = qssFile.readAll();
        qssFile.close();

        QFile appQssFile(QApplication::applicationDirPath() + AStr("/Style/Theme/%1.qss").arg(themeResFile));
        if (appQssFile.open(QFile::ReadOnly | QFile::Text))
            readThemeData += appQssFile.readAll();
        appQssFile.close();

        themeData = readThemeData;
    }

    qApp->setStyleSheet(themeData);
}

APROCH_NAMESPACE_END
