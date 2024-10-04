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

class AWidgetStyleDecorationPrivate
{
public:
    QPointer<QWidget> _host;
    EThemeType _theme = EThemeType::System;
    QMetaObject::Connection _themeChangedConnection;

    inline void setTheme(QWK::WidgetWindowAgent* winAgent, EThemeType theme)
    {
        switch (theme)
        {
        case EThemeType::Dark:
            winAgent->setWindowAttribute("dark-mode", true);
            break;
        case EThemeType::Light:
            winAgent->setWindowAttribute("dark-mode", false);
            break;
        case EThemeType::Custom:
            // TODO
            break;
        default:
            break;
        }
    }
};

AWidgetStyleDecoration::AWidgetStyleDecoration()
    : d_ptr(new AWidgetStyleDecorationPrivate)
{
}

AWidgetStyleDecoration::~AWidgetStyleDecoration()
{
}

#ifdef Q_OS_WIN

void AWidgetStyleDecoration::setWinUIEnabled(bool enabled)
{
    setWinUIMaterial(EWinUIMaterial::NoWinUIMaterial);
}

bool AWidgetStyleDecoration::isWinUIEnabled() const
{
    return getWinUIMaterial() != EWinUIMaterial::NoWinUIMaterial;
}

static QMap<EWinUIMaterial, QString> s_bkMaterial2Name = {
    { EWinUIMaterial::DWMBlur, AStr("dwm-blur") },
    { EWinUIMaterial::Acrylic, AStr("acrylic-material") },
    { EWinUIMaterial::Mica, AStr("mica") },
    { EWinUIMaterial::MicaAlt, AStr("mica-alt") },
};

bool AWidgetStyleDecoration::setWinUIMaterial(EWinUIMaterial material)
{
    Q_ASSERT(mWinAgent);

    bool hasMaterial = false;
    QString name = s_bkMaterial2Name.value(material);

    // 先清空已有样式
    for (auto value : s_bkMaterial2Name)
        mWinAgent->setWindowAttribute(value, false);

    if (!name.isEmpty())
    {
        hasMaterial = mWinAgent->setWindowAttribute(name, true);
        if (!hasMaterial)
            return false;
    }

    if (d_ptr->_host)
    {
        d_ptr->_host->setAttribute(Qt::WA_TranslucentBackground, hasMaterial);
        d_ptr->_host->setProperty("has-material", hasMaterial);
        d_ptr->_host->style()->polish(d_ptr->_host);
    }

    return true;
}

EWinUIMaterial AWidgetStyleDecoration::getWinUIMaterial() const
{
    Q_ASSERT(mWinAgent);

    for (auto iter = s_bkMaterial2Name.cbegin(); iter != s_bkMaterial2Name.cend(); ++iter)
    {
        if (mWinAgent->windowAttribute(iter.value()).toBool())
            return iter.key();
    }

    return EWinUIMaterial::NoWinUIMaterial;
}

bool AWidgetStyleDecoration::setWinUITheme(EThemeType theme)
{
    Q_ASSERT(mWinAgent);

    /*if (d_ptr->_theme == theme)
        return false;*/

    // apply system theme
    auto appTheme = theme;
    if (appTheme == EThemeType::System)
    {
        appTheme = ATheme::getSystemTheme();
        if (d_ptr->_host)
        {
            d_ptr->_themeChangedConnection = QObject::connect(ATheme::getInstance(), &ATheme::themeChanged, [this]() {
                if (!d_ptr->_host)
                    return;
                d_ptr->setTheme(mWinAgent, ATheme::getSystemTheme());
            });
        }
    }
    else
    {
        QObject::disconnect(d_ptr->_themeChangedConnection);
    }

    d_ptr->_theme = theme;
    d_ptr->setTheme(mWinAgent, appTheme);

    return true;
}

EThemeType AWidgetStyleDecoration::getWinUITheme() const
{
    return d_ptr->_theme;
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

    d_ptr->_host = self;
    mWinAgent = new QWK::WidgetWindowAgent(d_ptr->_host);
    mWinAgent->setup(d_ptr->_host);

    setWinUITheme(EThemeType::System);
}

APROCH_NAMESPACE_END
