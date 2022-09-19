/****************************************************************************
 * @file    APluginManager.h
 * @date    2021-1-9
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

namespace aproch
{
    /**
     * 插件管理器
     */
    class APROCH_API APluginManager
    {
        APROCH_SINGLETON(APluginManager);

    public:
        ~APluginManager();

        /**
         * @brief 加载插件
         * @param dllFileName 插件文件名称，相对于软件目录的相对路径或者绝对路径
         * @return 是否加载成功
         */
        bool loadPlugin(const QString &dllFileName);

        /**
         * @brief 默认加载所有的插件
         */
        void loadPlugins();

        /**
         * @brief 获取所有的插件及信息
         * @return 插件表
         */
        inline const QMap<IPlugin *, SPluginInfo> &getPlugins() const noexcept
        {
            return mPlugins;
        }

    private:
        APluginManager();

    private:
        /** 加载成功的插件列表即插件信息 */
        QMap<IPlugin *, SPluginInfo> mPlugins;
    };
}
