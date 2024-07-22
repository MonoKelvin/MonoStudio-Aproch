/****************************************************************************
 * @file    AVerticalLabel.h
 * @date    2022-5-08
 * @author  MonoKelvin
 * @email   15007083506@qq.com
 * @github  https://github.com/MonoKelvin
 * @brief
 *
 * This source file is part of MonoDream.
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

class QBoxLayout;
class QSpacerItem;
class QLabel;

APROCH_NAMESPACE_BEGIN

/**
 * @brief 竖向标签。竖向标签是由一系列竖向文本<QLabel>控件按照水平布局，从右向左排列的组合控件，
 * 每个空间内的标签文本都不包含换行符'\n'，如果使用 addLabel 或者 insetLabel 方法添加标签列，
 * 则会自动识别换行，并在其后添加新的标签列。
 * @note 设置选项
 * MergeLineBreaks：开启时，添加标签页会对连续换行符合并，不把换行符当作新的一列
 * TrimmedFirst：开启时，将文本内容开头的所有空格以及换行符删除
 * TrimmedLast：开启时，将文本内容结尾的所有空格以及换行符删除
 * TrimmedAll：开启时，将文本内容开头和结尾的所有空格以及换行符删除
 */
class APROCH_API AVerticalLabel : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief 选项
     */
    enum EOption
    {
        NoOption = 0x000,
        MergeLineBreaks = 0x0001,
        TrimmedFirst = 0x0002,
        TrimmedLast = 0x0004,
        TrimmedAll = TrimmedFirst | TrimmedLast,
    };

    explicit AVerticalLabel(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    explicit AVerticalLabel(const QString &text, const EOption option = NoOption,
                            QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());

    /**
     * @brief 移除标签列
     * @param index 索引位置
     * @see addLabel, insetLabel
     */
    void removeLabel(int index);

    /**
     * @brief 在末尾添加标签列。自动识别换行符号，并添加新标签列
     * @param label 标签文字
     * @param align 文本对齐方式
     * @param opotion 选项
     * @return 返回添加后的标签控件，如果添加失败则返回空
     * @see AVerticalLabel::EOption, insetLabel, removeLabel
     */
    QList<QLabel *> addLabel(const QString &label,
                             Qt::Alignment align = Qt::AlignTop | Qt::AlignHCenter,
                             const EOption option = NoOption);

    /**
     * @brief 在指定位置插入标签列。自动识别换行符号，并在其后添加新标签列
     * @param index 索引位置
     * @param label 标签文字
     * @param align 文本对齐方式
     * @param opotion 选项
     * @return 返回添加后的标签控件，如果添加失败则返回空
     * @see AVerticalLabel::EOption, addLabel, removeLabel
     */
    QList<QLabel *> insetLabel(int index, const QString &label,
                               Qt::Alignment align = Qt::AlignTop | Qt::AlignHCenter,
                               const EOption option = NoOption);

    /**
     * 获取文本内容
     * @return 文本内容
     */
    QString getText() const;

    /**
     * @brief 获取指定位置的标签列文本内容
     * @param index 标签位置
     * @return 标签文本内容
     * @see getLabelWidget
     */
    QString getLabel(int index) const;

    /**
     * @brief 获取指定位置的标签列控件
     * @param index 标签位置
     * @return 标签控件
     * @see getLabel
     */
    QLabel *getLabelWidget(int index) const;

    /**
     * @brief 获取所有的标签列表
     * @return
     */
    inline const QList<QLabel *> &getLabelWidgets() const noexcept
    {
        return mLabels;
    }

    /**
     * @brief 设置控件内容对齐方式
     * @param align 对齐方式，仅支持Qt::AlignLeft, Qt::AlignCenter/Qt::AlignHCenter和Qt::AlignRight三种对齐
     */
    void setAlignment(const Qt::Alignment &align);

protected:
    /**
     * @brief 从给定标签文本字符创建标签列表
     * @param label 标签文本
     * @param align 文本对齐方式
     * @param option 选项
     * @return 标签列表
     */
    QList<QLabel *> createLabel(const QString &label, Qt::Alignment align, const EOption option);

private:
    /**
     * @brief 初始化控件
     * @param text 内容
     * @param opotion 选项
     */
    void init(const QString &text = QString(), const EOption option = NoOption);

protected:
    /** @brief 水平布局 */
    QBoxLayout *mHLayout;

    /** @brief 左侧弹簧控件 */
    QSpacerItem *mSpacerLeft;

    /** @brief 右侧弹簧控件 */
    QSpacerItem *mSpacerRight;

    /** @brief 标签列表 */
    QList<QLabel *> mLabels;

    /** @brief 对齐方式 */
    Qt::Alignment mAlignment;

private:
    Q_DISABLE_COPY_MOVE(AVerticalLabel)
};

APROCH_NAMESPACE_END
