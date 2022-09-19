/****************************************************************************
 * @file    AIconFont.h
 * @date    2021-10-27
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
    using TIcon = QChar;
    using TIconName = QString;

    /**
     * @brief 图标字体
     * @note 默认使用了 Font Awesome 系列的图标字体。
     */
    class APROCH_API AIconFont
    {
    public:
        explicit AIconFont(const QString &iconFontPath, const QString &iconNameFile = QString());
        virtual ~AIconFont();

        /**
         * @brief 获取图标对应的字符
         * @param iconName 图标名
         * @return 图标字符
         */
        TIcon getIcon(const TIconName &iconName);

        /**
         * @brief 获取图标字符对应的名称
         * @param iconChar 图标字符
         * @return 图标名称。如果没有对应的名称则返回空字符
         */
        TIconName getIconName(const TIcon &iconChar);

        /**
         * @brief 获取字体名称
         * @return 字体名称
         */
        QString getFontFamily() const;

        /**
         * @brief 获取字体ID
         * @return 字体ID
         */
        inline int getFontID() const noexcept
        {
            return mID;
        }

        /**
         * @brief 添加图标字体。如果存在则覆盖
         * @param iconFont 图标字体
         * @return 字体ID
         */
        static int AddIconFont(const AIconFont &iconFont);

        /**
         * @brief 删除图标字体
         * @param iconFontName 图标字体名称
         */
        static void RemoveIconFont(const QString &iconFontName);

        /**
         * @brief 获取图标字符
         * @param iconName 图标名称
         * @param iconFontName 图标字体名称
         * @return 图标名称
         */
        static TIcon Icon(const TIconName &iconName, const QString &iconFontName);

        /**
         * @brief 获取图标字体
         * @param iconFontName 图标字体名称
         * @return 图标字体。如果字体不存在，则返回默认字体
         */
        static QFont IconFont(const QString &iconFontName);

        /**
         * @brief 获取图标字体
         * @param fontId 图标字体ID
         * @return 图标字体。如果字体不存在，则返回默认字体
         */
        static QFont IconFont(int fontId);

        /**
         * @brief 是否和另一个图标字体相等
         * @param rhs 图标字体
         * @return 是否相等
         */
        inline bool operator==(const AIconFont &rhs) const
        {
            return mFontFamily.toUpper() == rhs.mFontFamily.toUpper();
        }

    protected:
        /**
         * @brief 解析字符名称
         * @param iconNameFile 字符名称文件
         * @param charMap 字符表
         * @return 是否解析成功
         */
        virtual bool parseIconFont(const QString &iconNameFile, QMap<TIconName, TIcon> &charMap) const;

    private:
        /** @brief 字体表。<字体名称, 字体> */
        static QList<AIconFont> sFontList;

        /** @brief 字符表 */
        QMap<TIconName, TIcon> mCharMap;

        /** @brief 字体名称 */
        QString mFontFamily;

        /** @brief 字体ID */
        int mID;
    };
}
