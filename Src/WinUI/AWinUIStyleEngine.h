/****************************************************************************
 * @file    AWinUIStyleEngine.h
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
#ifndef AWINUISTYLEENGINE_H
#define AWINUISTYLEENGINE_H
#include "WinUIGloabl.h"
#include <QtWidgets/QStylePlugin>

APROCH_NAMESPACE_BEGIN

class AWinUIStyleEnginePrivate;
class APROCH_API AWinUIStyleEngine : public QStylePlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QStyleFactoryInterface" FILE "AWinUIStyle.json")
public:
    explicit AWinUIStyleEngine(QObject* parent = nullptr);

    enum ThemeType
    {
        Dark,
        Light,
        BySystem,
        Custom,
    };

    QStyle* create(const QString& key) override;
    static AWinUIStyleEngine* instance();

    /** @brief 初始化WinUI样式主题 */
    void initStyleTheme(const QString& styleFile = QString());

    ThemeType getTheme() const;
    QString getThemeFile() const;
    void setTheme(ThemeType type);
    bool setTheme(const QString& themeFile);

    /** @brief 获取软件的调色板 */
    QPalette& getAppPalette();

    /** @brief 获取指定类型控件的调色板 */
    QPalette& getPalette(const char* name);
    QPalette& getPalette(QWidget* widget);

    template<class ClassType>
    QPalette& getPalette()
    {
        return getPalette(ClassType::staticMetaObject.className());
    }

private:
    Q_DISABLE_COPY_MOVE(AWinUIStyleEngine);
    Q_DECLARE_PRIVATE(AWinUIStyleEngine);
};

APROCH_NAMESPACE_END

#endif // !AWINUISTYLEENGINE_H
