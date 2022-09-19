/****************************************************************************
 * @file    AAppConfigService.h
 * @date    2021-10-20
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

#include <QSettings>

namespace aproch
{
    /**
     * @brief 应用程序配置服务
     * @note
     *
     * 键值格式应该为：[x/]x[/x]，如："AppName", "Windows/Size"，"Windows/Theme/Color"等；如果需要设置默认键值，
     * 可以让起始分类命名为<Default>，如"Default/Size"，"Default/App/Theme"等；或者通过 @see setDefault 设置
     *
     * 所有键值、分组都不区分大小写。
     *
     */
    class APROCH_API AAppConfigService : public IAppConfigService
    {
    public:
        AAppConfigService();

        /**
         * @brief 设置文件配置路径
         * @param filename 路径
         */
        virtual void setFileName(const QString &filename) override;

        /**
         * @brief 设置键值，如果键不存在，则新建键值并同时设置默认值为<value>
         * @param key 键，如果以"Default/"（或多个重复的）形式的默认标识开头，则设置的值为该键的默认值
         * @see setDefault
         * @param value 值
         */
        virtual void setValue(const QString &key, const QVariant &value) override;

        /**
         * @brief 设置默认值
         * @param key 键。如果该键以"Default/"（或多个重复的）形式的默认标识开头，则会自动将所有的默认标识清除
         * @param value 默认值
         */
        virtual void setDefault(const QString &key, const QVariant &value) override;

        /**
         * @brief 获取键对应的值
         * @param key 键。如果以"Default/"开头，则获取的是默认值
         * @return 值。如果没有将返回无效的空值
         */
        virtual QVariant getValue(const QString &key) const override;

        /**
         * @brief 获取键对应的默认值
         * @param key 键。如果以"Default/"（或多个重复的）形式的默认标识开头，则自动清除所有默认标识后再获取其默认值
         * @return 默认值。如果没有将返回无效的空值
         */
        virtual QVariant getDefault(const QString &key) const override;

        virtual void reset() override;
        virtual void reset(const QString &key) override;
        virtual bool has(const QString &key) const override;

    protected:
        /**
         * @brief 获取见正规后的键值
         * @param key 键值
         * @param keyWithDef 带有默认标识的键值
         * @param keyWithoutDef 不带有默认标识的键值
         */
        void getNormalKeys(const QString &key, QString &keyWithDef, QString &keyWithoutDef) const;

        /**
         * @brief 返回配置文件路劲
         * @return 文件路径全称
         */
        virtual const QString configFilePath() const;

    private:
        /** @brief 配置 */
        QSettings mSettings;
    };
}
