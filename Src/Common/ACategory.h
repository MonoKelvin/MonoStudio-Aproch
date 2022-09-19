/****************************************************************************
 * @file    ACategory.h
 * @date    2022-08-13
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
     * @brief 分类。分类是由部分类名称加上分隔字符组成的字符串标识，形式如：A/B/C，但在ACategory类中每级分类都是单独记录的，称为分级<Level>
     * 且分割字符<Splitter>可自定义其他字符，如定义成'|'，对应的分类名称为：A|B|C
     */
    class APROCH_API ACategory
    {
    public:
        /** @brief 拆分分类行为选项 */
        enum ESplitBehavior
        {
            /** @brief 没有行为 */
            NoBehavior = 0x000,

            /** @brief 跳过空的分级 */
            SkipEmptyLevel = 0x001,

            /** @brief 分割字符不区分大小写 */
            CaseInsensitive = 0x002,

            /** @brief 将分级的前后空格去除 */
            TrimmedLevel = 0x004,
        };

        ACategory();
        explicit ACategory(const QString& category, ESplitBehavior behavior = SkipEmptyLevel);
        explicit ACategory(const QStringList& stringList, ESplitBehavior behavior = SkipEmptyLevel);

        /** @brief 从字符串解析分类名称 */
        void fromString(const QString& category, ESplitBehavior behavior = SkipEmptyLevel);

        /** @brief 转为字符形式，如：A/B/C. */
        QString toString() const;

        /** @brief 获取所有的分级（按顺序） */
        const QStringList& getLevels() const;

        /** @brief 获取指定位置分级的内容 */
        QString getLevel(int level) const;

        /** @brief 设置分级位置的内容 */
        void setLevel(int level, const QString& str);

        /** @brief 向后追加分级 */
        ACategory& pushLevel(const QString& level, ESplitBehavior behavior = SkipEmptyLevel);

        /** @brief 在指定位置添加分级 */
        ACategory& insertLevel(int position, const QString& level, ESplitBehavior behavior = SkipEmptyLevel);

        /** @brief 移出指定位置的分级 */
        ACategory& removeLevel(int position);

        /** @brief 移除最后位置的分级 */
        ACategory& popLevel();

        /** @brief 设置分割字符 */
        void setSplitter(const QString& splitter);

        /** @brief 获取分割字符 */
        QString getSplitter() const;

        /** @brief 分类是否为空 */
        bool isVoid() const;

        /** @brief 分类中是否有分级 */
        bool hasLevel(const QString& level, Qt::CaseSensitivity cs = Qt::CaseInsensitive) const;

        /** @brief 上一级分类 */
        ACategory previous() const;

        /** @brief 转为字符串，等同于 toString */
        operator QString() const;

        /** @brief 和另一个分类是否相等 */
        bool operator==(const ACategory& rhs) const;

        /** @brief 和另一个分类是否不等 */
        bool operator!=(const ACategory& rhs) const;

        /** @brief 向后追加分级，等同于 pushLevel(level, ESplitBehavior::SkipEmptyLevel) */
        ACategory& operator<<(const QString& level);

    protected:
        /** @brief 定义调试输出 */
        friend APROCH_API QDebug operator<<(QDebug ds, const ACategory& category);

    private:
        /** @brief 分类 */
        QStringList m_category;

        /** @brief 分割字符 */
        QString m_splitter;
    };
}