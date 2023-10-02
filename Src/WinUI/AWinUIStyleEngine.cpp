/****************************************************************************
 * @file    AWinUIStyleEngine.cpp
 * @date    2023-04-07 
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
#include "AWinUIStyleEngine.h"
#include "Private/AWinUIStyle.h"

#include <QApplication>

#include <private/qobject_p.h>

APROCH_NAMESPACE_BEGIN

class AWinUIStyleEnginePrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(AWinUIStyleEngine);
public:
    QMap<QByteArray, QPalette> paletteMap;
    QPalette appPalette;
    QString themeFile;
    AWinUIStyleEngine::ThemeType themeType = AWinUIStyleEngine::BySystem;
};

AWinUIStyleEngine::AWinUIStyleEngine(QObject* parent)
    : QObject(*(new AWinUIStyleEnginePrivate), parent)
{
    //d_ptr.reset(new AWinUIStyleEnginePrivate);
}

void AWinUIStyleEngine::initStyleTheme(const QString& styleFile)
{
    Q_D(AWinUIStyleEngine);
    if (setTheme(styleFile))
        return;

    // Application
    d->appPalette.setColor(QPalette::Base, QColor(50, 50, 50));
    d->appPalette.setColor(QPalette::Background, QColor(32, 32, 32));
    d->appPalette.setColor(QPalette::Text, Qt::white);
    d->appPalette.setColor(QPalette::WindowText, Qt::white);
    d->appPalette.setColor(QPalette::ButtonText, Qt::white);
    d->appPalette.setColor(QPalette::PlaceholderText, QColor(143, 143, 143));

    QApplication::setPalette(d->appPalette);
    
    // ATextBox

    QApplication::setStyle(create(QString::fromLatin1("winui")));
    QApplication::setFont(QFont(QString::fromLatin1("Microsoft Yahei"), 10));
}

AWinUIStyleEngine::ThemeType AWinUIStyleEngine::getTheme() const
{
    Q_D(const AWinUIStyleEngine);
    return d->themeType;
}

QString AWinUIStyleEngine::getThemeFile() const
{
    Q_D(const AWinUIStyleEngine);
    return d->themeFile;
}

void AWinUIStyleEngine::setTheme(ThemeType type)
{
    Q_D(AWinUIStyleEngine);
    if (d->themeType == type)
        return;

    d->themeType = type;
}

bool AWinUIStyleEngine::setTheme(const QString& themeFile)
{
    Q_D(AWinUIStyleEngine);
    QFile file(themeFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    d->themeFile = file.fileName();
    d->themeType = Custom;

    // TODO: read file
    return true;
}

QPalette& AWinUIStyleEngine::getAppPalette()
{
    Q_D(AWinUIStyleEngine);
    return d->appPalette;
}

QPalette& AWinUIStyleEngine::getPalette(const char* name)
{
    Q_D(AWinUIStyleEngine);

    QByteArray className(name);
    auto iter = d->paletteMap.find(className);
    if (iter != d->paletteMap.end())
        return iter.value();

    d->paletteMap.insert(className, QPalette());
    return d->paletteMap[className];
}

QPalette& AWinUIStyleEngine::getPalette(QWidget* widget)
{
    return getPalette(widget->metaObject()->className());
}

QStyle* AWinUIStyleEngine::create(const QString& key)
{
    if (key.compare(QLatin1String("winui"), Qt::CaseInsensitive) == 0)
        return new AWinUIStyle();

    return nullptr;
}

AWinUIStyleEngine* AWinUIStyleEngine::instance()
{
    static AWinUIStyleEngine inst;
    return &inst;
}

APROCH_NAMESPACE_END
