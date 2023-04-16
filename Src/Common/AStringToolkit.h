/****************************************************************************
 * @file    AStringToolkit.h
 * @date    2021-10-08
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

APROCH_NAMESPACE_BEGIN

/**
 * @brief 字符串工具
 */
class APROCH_API AStringToolkit
{
public:
    /**
     * @brief 在给定的字符串(source)中，子字符串(symbols)周围添加字符(addition)
     * @param source 原始字符串
     * @param substrs 子字符串列表
     * @param addition 用于包裹子字符传的字符
     * @return 新的字符串
     */
    static QString WrapSymbolsWithChars(const QString &source, const QStringList &substrs, const char *addition = " ");

    /**
     * @brief 正规化分类目录名
     * @note 正规化操作是将所有"\\"符号转成"/"符号，并在最前面添加"/"（如果没有）符号，若<withName>为false，则在最后添加"/"符号
     * @param category 目录名
     * @param withName 是否目录带上了名称（类似操作系统中的目录带上文件名）
     * @return 正规化后的目录名
     */
    static QString NormalizeCategory(const QString &category, bool withName = false);

    /**
     * @brief 获取目录的每一级
     * @note 在两个"/"符号之间的字符为一级别，如："/abc/defg/h/i" 目录级别为："abc","defg","h"
     * @param category
     * @return
     */
    static QList<QString> GetCategoryLevels(const QString &category);

    /**
     * @brief 替换所有单个或连续的 "\\" 和 "/" 符号为 "/"
     * @param source 原始字符串
     * @return 替换后新的字符串
     */
    static QString ReplacToSlash(const QString &source);

    /**
     * @brief 颜色转16进制字符串表示，如：Color2HexStr(Qt::red) 得到 "#FF0000";
     * @param color 颜色
     * @param isAlpha 是否将透明通道输出，如果是则格式为 "#ARGB" 形式
     * @return 16进制字符表示
     */
    static QString Color2HexStr(const QColor &color, bool isAlpha = false);

    /**
     * @brief 颜色转RGB字符串表示，如：Color2HexStr(Qt::red) 得到 "RGB(255， 0， 0)"
     * @param color 颜色
     * @param isAlpha 是否将透明通道输出，如果是则格式为 "#ARGB(A, R, G, B)"
     * @return
     */
    static QString Color2RgbStr(const QColor &color, bool isAlpha = false);

    /**
     * @brief 删除字符左边所有的空格和换行符号
     * @param str 原始字符
     * @return 返回修改后的新字符串
     */
    static QString TrimLeft(const QString &str);

    /**
     * @brief 删除字符右边所有的空格和换行符号
     * @param str 原始字符
     * @return 返回修改后的新字符串
     */
    static QString TrimRight(const QString &str);

    /**
     * @brief 删除前后空白字符后进行比较是否相等
     * @param str1 字符串1
     * @param str2 字符串2
     * @param cs 大小写敏感
     * @return 是否相等
     */
    static bool TrimCompare(const QString &str1, const QString &str2, Qt::CaseSensitivity cs = Qt::CaseInsensitive);

    /**
     * @brief 将所有连续相同（至少连续出现两次）的字符替换为指定字符
     * @note 区分大小写
     * @param str 原始字符
     * @param rpStr 用于替换的字符
     * @return 返回修改后的新字符串
     */
    static QString ReplaceContinuousString(const QString &str, const QString &rpStr);

    /**
     * @brief 将指定连续相同的字符（至少连续出现两次）替换为指定字符
     * @note 区分大小写
     * @param str 原始字符
     * @param target 进行替换的字符
     * @param rpStr 用于替换的字符
     * @return 返回修改后的新字符串
     */
    static QString ReplaceContinuousString(const QString &str, const QString &target, const QString &rpStr);

    /**
     * @brief 查找值
     * @param varMap 变量表
     * @param key 键值
     * @param sensitivity 是否大小写敏感。如果大小写不敏感，则查找到第一个值就返回，不继续查找
     * @param defaultVal 未查找到，则返回的默认值
     * @return QVariant 查找到的结果
     */
    static QVariant FindValue(const QVariantMap &varMap, const QString &key,
                              Qt::CaseSensitivity sensitivity = Qt::CaseInsensitive,
                              const QVariant &defaultVal = QVariant());

    /**
     * @brief 查找值
     * @param varMap 变量表
     * @param key 键值
     * @param sensitivity 是否大小写敏感。如果大小写不敏感，则查找到第一个值就返回，不继续查找
     * @param defaultVal 未查找到，则返回的默认值
     * @return T 查找到的结果
     */
    template <typename T>
    static T FindValue(const QMap<QString, T> &varMap, const QString &key,
                       Qt::CaseSensitivity sensitivity = Qt::CaseInsensitive,
                       const T &defaultVal = T())
    {
        if (sensitivity == Qt::CaseSensitive)
            return varMap.value(key, defaultVal);

        for (auto iter = varMap.begin(); iter != varMap.end(); ++iter)
            if (iter.key().compare(key, sensitivity) == 0)
                return iter.value();

        return defaultVal;
    }

    /**
     * @brief 使用MD5算法生成文本
     * @param source 源字符串
     * @param isHalf 是否只得到16位的字符串，否则为32位
     * @return 返回加密好的字符串
     */
    static const QString EncryptByMD5(const QString &source, bool isHalf = false);
};

APROCH_NAMESPACE_END
