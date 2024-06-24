/****************************************************************************
 * @file    AWidgetStyleDecoration.cpp
 * @date    2024-06-23 
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
#include "AWidgetStyleDecoration.h"

#include <QWKWidgets/widgetwindowagent.h>

APROCH_NAMESPACE_BEGIN

AWidgetStyleDecoration::AWidgetStyleDecoration()
{
}

AWidgetStyleDecoration::~AWidgetStyleDecoration()
{
}

#ifdef Q_OS_WIN

static QMap<EWinUIMaterial, QString> s_bkMaterial2Name = {
    QPair<EWinUIMaterial, QString>(EWinUIMaterial::DWMBlur, AStr("dwm-blur")),
    QPair<EWinUIMaterial, QString>(EWinUIMaterial::Acrylic, AStr("acrylic-material")),
    QPair<EWinUIMaterial, QString>(EWinUIMaterial::Mica, AStr("mica")),
    QPair<EWinUIMaterial, QString>(EWinUIMaterial::MicaAlt, AStr("mica-alt")),
};

bool AWidgetStyleDecoration::setWinUIMaterial(const SWinUIMaterialOption& option)
{
    if (!_host)
        return false;

    Q_ASSERT(mWinAgent);

    bool hasMaterial = false;
    QString name = s_bkMaterial2Name.value(option.material);
    if (name.isEmpty() || !option.enabled)
    {
        for (auto value : s_bkMaterial2Name)
            mWinAgent->setWindowAttribute(value, false);
    }
    else
    {
        hasMaterial = mWinAgent->setWindowAttribute(name, true);
        if (!hasMaterial)
            return false;
    }

    _materialOption = option;

    // apply system theme
    auto appTheme = _materialOption.theme;
    if (appTheme == EThemeType::System)
        appTheme = ATheme::getSystemTheme();

    if (_materialOption.enabled)
    {
        switch (appTheme)
        {
        case EThemeType::Dark:
            mWinAgent->setWindowAttribute("dark-mode", true);
            break;
        case EThemeType::Light:
            mWinAgent->setWindowAttribute("dark-mode", false);
            break;
        case EThemeType::Custom:
            // TODO
            break;
        default:
            break;
        }
    }

    _host->setAttribute(Qt::WA_TranslucentBackground, hasMaterial);
    _host->setProperty("has-material", hasMaterial);
    _host->style()->polish(_host);
    return true;
}

AWidgetStyleDecoration::SWinUIMaterialOption AWidgetStyleDecoration::getWinUIMaterial()
{
    Q_ASSERT(mWinAgent);

    _materialOption.material = EWinUIMaterial::NoWinUIMaterial;
    _materialOption.enabled = false;

    for (auto iter = s_bkMaterial2Name.cbegin(); iter != s_bkMaterial2Name.cend(); ++iter)
    {
        if (mWinAgent->windowAttribute(iter.value()).toBool())
        {
            _materialOption.material = iter.key();
            _materialOption.enabled = true;
            break;
        }
    }

    if (mWinAgent->windowAttribute("dark-mode").toBool())
        _materialOption.theme = EThemeType::Dark;

    return _materialOption;
}

void AWidgetStyleDecoration::setExtraMargins(const QMargins& margins)
{
    Q_ASSERT(mWinAgent);

    mWinAgent->setWindowAttribute("extra-margins", QVariant::fromValue(margins));
}

QMargins AWidgetStyleDecoration::getExtraMargins() const
{
    Q_ASSERT(mWinAgent);
    return mWinAgent->windowAttribute("extra-margins").value<QMargins>();
}

#endif

void AWidgetStyleDecoration::initStyle(QWidget* self)
{
    Q_ASSERT(self);

    _host = self;
    mWinAgent = new QWK::WidgetWindowAgent(_host);
    mWinAgent->setup(_host);
}

APROCH_NAMESPACE_END
