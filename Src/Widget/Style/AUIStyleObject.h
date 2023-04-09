/****************************************************************************
 * @file    AUIStyleObject.h
 * @date    2023-04-08 
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
#include <QStyleOption>

APROCH_NAMESPACE_BEGIN

class AUIStyleObjectPrivate;
class APROCH_API AUIStyleObject
{
public:
    AUIStyleObject();
    virtual ~AUIStyleObject();

    // frame
    static const char BorderRadius[];
    static const char BorderThickness[];
    static const char BorderColor[];
    static const char Margins[];
    static const char Paddings[];
    static const char Spacing[];

    // font           
    static const char TextSize[];
    static const char TextSpcing[];
    static const char TextLineSpcing[];

    // effect
    static const char Shadow[];

    // animation
    static const char Transition[];

    // theme
    static const char Theme[];

public:
    QVariant getStyleValue(const char* name) const;
    QVariant getStyleValue(const char* name, const QVariant& defaultValue) const;

    template<typename T>
    T getStyleValue(const char* name) const
    {
        return getStyleValue(name).value<T>();
    }

    template<typename T>
    T getStyleValue(const char* name, const T& defaultValue) const
    {
        const QVariant val = getStyleValue(name);
        if (!val.isValid())
            return defaultValue;
        return val.value<T>();
    }

    const QList<QByteArray>& getStyleNameList() const;

protected:
    void bindStyleObject(QWidget* widget);
    void addStyle(const char* name, const QVariant& val);

    template<typename T>
    void addStyle(const char* name, const T& val)
    {
        addStyle(name, QVariant::fromValue(val));
    }

    virtual void initProperties(QWidget* widget);

private:
    Q_DISABLE_COPY_MOVE(AUIStyleObject);
    QScopedPointer<AUIStyleObjectPrivate> _uso_d_ptr;
};

inline AUIStyleObject* aStyleObject(QWidget* widget) noexcept
{
    return (AUIStyleObject*)(widget);
}

template<class T>
inline T* aDynamicStyleObject(T* obj) noexcept
{
    return dynamic_cast<T*>(obj);
}

inline QVariant aStyleValue(QWidget* widget, const char* name) noexcept
{
    return ((AUIStyleObject*)(widget)) ? ((AUIStyleObject*)(widget))->getStyleValue(name) : QVariant();
}

/* ----------------------------------------------------------------------------------------------------
    default style defines
---------------------------------------------------------------------------------------------------- */

extern APROCH_API SCornerF AprochDefaultBorderRadius;
extern APROCH_API SBorderThicknessF AprochDefaultBorderThickness;
extern APROCH_API QColor AprochDefaultBorderColor;
extern APROCH_API QMargins AprochDefaultMargins;
extern APROCH_API Paddings AprochDefaultPaddings;
extern APROCH_API Spacing AprochDefaultSpacing;
//extern APROCH_API QColor AprochDefaultBackground;
extern APROCH_API QColor AprochDefaultBackgroundColor;
extern APROCH_API QString AprochDefaultFontFamily;
extern APROCH_API QColor AprochDefaultTextColor;
extern APROCH_API int AprochDefaultTextSize;
extern APROCH_API Spacing AprochDefaultTextSpcing;
extern APROCH_API Spacing AprochDefaultTextLineSpcing;
extern APROCH_API SShadowParam AprochDefaultShadow;
//extern APROCH_API SAnimationParam AprochDefaultTransition;

APROCH_NAMESPACE_END