/****************************************************************************
 * @file    APrivateAppConfigService.h
 * @date    2022-06-12
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

#define AServiceName_PrivateAppConfig "aproch.PrivateAppConfigService"

APROCH_NAMESPACE_BEGIN

/**
 * @brief 应用程序私有配置服务
 */
class APROCH_API APrivateAppConfigService : public IAppConfigService
{
public:
    APrivateAppConfigService();

    /**
     * @brief 设置文件配置路径
     * @param filename 路径
     */
    virtual void setFileName(const QString &filename) override;

    /**
     * @brief 设置键值
     * @param key 键
     * @see setDefault
     * @param value 值
     */
    virtual void setValue(const QString &key, const QVariant &value) override;

    /**
     * @brief 设置默认值
     * @param key 键
     * @param value 默认值
     */
    virtual void setDefault(const QString &, const QVariant &) override {}

    /**
     * @brief 获取键对应的值
     * @param key 键
     * @return 值。如果没有将返回无效的空值
     */
    virtual QVariant getValue(const QString &key) const override;

    /**
     * @brief 获取键对应的默认值
     * @param key 键
     * @return 默认值。如果没有将返回无效的空值
     */
    virtual QVariant getDefault(const QString &key) const override;

    virtual void reset() override {}
    virtual void reset(const QString &) override {}
    virtual bool has(const QString &key) const override;

private:
    /** @brief 配置表 */
    QVariantMap mConfigMap;
};

APROCH_NAMESPACE_END
