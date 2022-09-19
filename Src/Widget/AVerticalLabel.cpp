/****************************************************************************
 * @file    AVerticalLabel.cpp
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
#include "stdafx.h"
#include "AVerticalLabel.h"

namespace aproch
{
    AVerticalLabel::AVerticalLabel(QWidget* parent, Qt::WindowFlags flags)
        : QWidget(parent, flags)
        , mAlignment(Qt::AlignHCenter)
    {
        init();
    }

    AVerticalLabel::AVerticalLabel(const QString& text, const EOption option, QWidget* parent, Qt::WindowFlags flags)
        : QWidget(parent, flags)
        , mAlignment(Qt::AlignHCenter)
    {
        init(text, option);
    }

    void AVerticalLabel::init(const QString& text, const EOption option)
    {
        setAttribute(Qt::WA_StyledBackground, true);

        mHLayout = new QBoxLayout(QBoxLayout::RightToLeft, this);
        mHLayout->setMargin(0);
        mHLayout->setSpacing(0);
        mSpacerLeft = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum);
        mSpacerRight = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum);

        mHLayout->addSpacerItem(mSpacerLeft);
        addLabel(text, Qt::AlignTop | Qt::AlignHCenter, option);
        mHLayout->addSpacerItem(mSpacerRight);
    }

    void AVerticalLabel::removeLabel(int index)
    {
        if (index < 0 || index > mLabels.size())
            return;

        QLabel* theLabel = mLabels.at(index);
        const auto& childList = mHLayout->children();
        for (const auto& childWidget : childList)
        {
            if (nullptr != childWidget && childWidget == theLabel)
            {
                mHLayout->removeWidget(theLabel);
                mLabels.removeAt(index);
                theLabel->deleteLater();
                break;
            }
        }
    }

    QList<QLabel*> AVerticalLabel::addLabel(const QString& label,
                                            Qt::Alignment align,
                                            const EOption option)
    {
        return insetLabel(mLabels.size(), label, align, option);
    }

    QList<QLabel*> AVerticalLabel::insetLabel(int index,
                                                const QString& label,
                                                Qt::Alignment align,
                                                const EOption option)
    {
        QList<QLabel*> labels;
        if (index < 0 || index > mLabels.size() || label.isEmpty())
            return labels;

        // 避免布局中含有其他控件，导致获取的索引位置错误，需要重新获取布局中控件所在的索引
        const int layoutCount = mHLayout->count();
        if (layoutCount < mLabels.size() + 2)   // 2表示左右两个弹簧空间
            return labels;

        int layoutIndex = -1;
        if(mLabels.size() > 0 && index > 0)
        {
            const QLabel* theLabel = mLabels.at(index - 1);
            for (int i = 0; i < layoutCount; ++i)
            {
                QWidget* lbWidget = mHLayout->itemAt(i)->widget();
                if (lbWidget != theLabel)
                    continue;
                layoutIndex = i + 1;
                break;
            }
            if (layoutIndex == -1)
                return labels;
        }
        else
        {
            layoutIndex = layoutCount - 1;
        }

        // 创建控件
        labels = createLabel(label, align, option);

        // 加到界面中
        for (const auto& labelWidget : labels)
        {
            mLabels.insert(index++, labelWidget);
            mHLayout->insertWidget(layoutIndex++, labelWidget);
        }

        return labels;
    }

    QString AVerticalLabel::getText() const
    {
        QString text;
        for (const auto& label : mLabels)
            text += label->text();
        return text;
    }

    QString AVerticalLabel::getLabel(int index) const
    {
        if (index < 0 || index > mLabels.size())
            return ANull_String;

        const QLabel* label = mLabels.at(index);
        Q_ASSERT(nullptr != label);

        return label->text();
    }

    QLabel* AVerticalLabel::getLabelWidget(int index) const
    {
        return nullptr;
    }

    void AVerticalLabel::setAlignment(const Qt::Alignment& align)
    {
        if (align & Qt::AlignLeft)  // 左对齐
        {
            mSpacerLeft->changeSize(1, 1, QSizePolicy::Ignored, QSizePolicy::Minimum);
            mSpacerRight->changeSize(1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum);
        }
        else if (align & Qt::AlignRight)   // 右对齐
        {
            mSpacerLeft->changeSize(1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum);
            mSpacerRight->changeSize(1, 1, QSizePolicy::Ignored, QSizePolicy::Minimum);
        }
        else // 其他默认居中对齐
        {
            mSpacerLeft->changeSize(1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum);
            mSpacerRight->changeSize(1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum);
        }

        mHLayout->update();
    }

    void AVerticalLabel::paintEvent(QPaintEvent* event)
    {
        APROCH_USE_STYLE_SHEET();

        QWidget::paintEvent(event);
    }

    QList<QLabel*> AVerticalLabel::createLabel(const QString& label, Qt::Alignment align, const EOption option)
    {
        using StringToolki = aproch::AStringToolkit;

        QString newLabel = label;
        if (option & TrimmedFirst)
            newLabel = StringToolki::TrimmLeft(label);
        if (option & TrimmedLast)
            newLabel = StringToolki::TrimmRight(newLabel);
        const auto& strList = label.split(AKey_NewLine, option & MergeLineBreaks ?
                                            QString::SkipEmptyParts :
                                            QString::KeepEmptyParts);

        QList<QLabel*> labels;
        for (const auto& str : strList)
        {
            QStringList textList = str.split(ANull_String);
            if (textList.first().isEmpty())
                textList.pop_front();
            if (textList.back().isEmpty())
                textList.pop_back();

            // 制表符号换成四个空格
            for (auto&& text : textList)
                text.replace(AKey_Table, AStr("    "));

            QLabel* label = new QLabel(textList.join(AKey_NewLine), this);
            label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
            label->setWordWrap(true);
            label->setAlignment(align);

            labels.push_back(label);
        }

        return labels;
    }
}
