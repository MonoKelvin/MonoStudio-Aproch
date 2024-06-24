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

#ifdef Q_OS_WIN
#include "AWinUIStyleHelper.h"
#endif // Q_OS_WIN


APROCH_NAMESPACE_BEGIN

class AThemePrivate
{
public:
    inline static ATheme* theAppTheme = nullptr;
    EThemeType themeType = EThemeType::System;

public:
    static ATheme* themeInst()
    {
        if (!theAppTheme)
            theAppTheme = new ATheme();
        return theAppTheme;
    }
};

ATheme::ATheme(QObject* parent)
    : QObject(parent)
    , d_ptr(new AThemePrivate())
{
}

ATheme::~ATheme()
{
}

void ATheme::setTheme(EThemeType type)
{
    auto theAppTheme = AThemePrivate::themeInst();
    if (theAppTheme->d_ptr->themeType == type)
        return;
    theAppTheme->d_ptr->themeType = type;

    // TODO

    emit theAppTheme->themeChanged();
}

EThemeType ATheme::getTheme()
{
    auto theAppTheme = AThemePrivate::themeInst();
    return theAppTheme->d_ptr->themeType;
}

EThemeType ATheme::getSystemTheme()
{
#ifdef Q_OS_WIN
    return AWinUIStyleHelper::getThemeConfig().systemUsesLightTheme ? EThemeType::Light : EThemeType::Dark;
#endif // Q_OS_WIN

    return EThemeType::System;
}

APROCH_NAMESPACE_END
