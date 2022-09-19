/****************************************************************************
 * @file    APromptWidget.cpp
 * @date    2022-06-03
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
#include "stdafx.h"
#include "APromptWidget.h"

#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>

namespace aproch
{
    APromptWidget::APromptWidget(const QString& content, QWidget* parent, APromptWidget::EPromptType type, int duration)
        : QWidget(parent)
        , mDuration(duration)
    {
        Q_ASSERT(parent);

        buildUI(content);
        setProperty("promptType", type);
        show();
    }

    void APromptWidget::paintEvent(QPaintEvent*)
    {
        APROCH_USE_STYLE_SHEET();
    }

    void APromptWidget::showEvent(QShowEvent*)
    {
        QString text = mContentLabel->text();
        const QSize textSize = adjustText(parentWidget()->width() * 0.8f, mContentLabel->font(), text);
        const int fontHeight = GetFontPixelHeight(mContentLabel->font()) / 2;

        mContentLabel->setText(text);
        resize(textSize.width() + 2 * fontHeight, textSize.height() + 2 * fontHeight);
        layout()->setContentsMargins(fontHeight, fontHeight, fontHeight, fontHeight);

        const QSize pwSize = parentWidget()->size();
        const int xOffset = (pwSize.width() - this->width()) / 2;
        QRect oriRect = QRect(xOffset, pwSize.height() + 10, this->width(), this->height());

        QPropertyAnimation* showAni = new QPropertyAnimation(this, QByteArrayLiteral("geometry"), this);
        showAni->setDuration(300);
        showAni->setStartValue(oriRect);

        const int yOffset = this->height() + 40;
        oriRect.setTop(oriRect.top() - yOffset);
        oriRect.setBottom(oriRect.bottom() - yOffset);
        showAni->setEndValue(oriRect);
        showAni->setEasingCurve(QEasingCurve::OutBack);

        QPropertyAnimation* quitAni = new QPropertyAnimation(this, QByteArrayLiteral("geometry"), this);
        quitAni->setDuration(300);
        quitAni->setStartValue(showAni->endValue());
        quitAni->setEndValue(showAni->startValue());
        quitAni->setEasingCurve(QEasingCurve::InBack);

        QSequentialAnimationGroup* seqAnimation = new QSequentialAnimationGroup(this);

        seqAnimation->addAnimation(showAni);
        seqAnimation->addPause(mDuration);
        seqAnimation->addAnimation(quitAni);
        seqAnimation->setLoopCount(1);

        seqAnimation->start();

        connect(seqAnimation, &QSequentialAnimationGroup::finished, this, &APromptWidget::deleteLater);
    }

    QSize APromptWidget::adjustText(int maxWidth, const QFont& font, QString& text)
    {
        QString strText = text;

        QFontMetrics fm(font);
        int width = fm.width(strText);
        int height = fm.height();

        int leadingHeight = fm.leading();
        int lineSpacingHeight = fm.lineSpacing();

        QString strFeed = "";
        int newLine = 0;

        int newLineWidthCount = 0;
        if (!strText.isEmpty())
        {
            QString curChar;
            for (int i = 0; i < strText.length(); i++)
            {
                curChar = strText[i];
                if (curChar == "\n")
                {
                    newLine++;
                }

                newLineWidthCount = newLineWidthCount + fm.width(curChar);
                if (newLineWidthCount > maxWidth)
                {
                    newLineWidthCount = 0;
                    strFeed = strFeed + "\n";
                    newLine++;
                }
                strFeed = strFeed + curChar;
            }
        }
        text = strFeed;

        return QSize(qMin(width, maxWidth), (height + leadingHeight) * (newLine + 1));
    }

    void APromptWidget::buildUI(const QString& prompt)
    {
        mContentLabel = new QLabel(prompt, this);
        QHBoxLayout* hbLayout = new QHBoxLayout(this);
        hbLayout->setSpacing(0);
        hbLayout->setMargin(0);

        mContentLabel->setAlignment(Qt::AlignVCenter);
        mContentLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        hbLayout->addWidget(mContentLabel, Qt::AlignCenter);
    }
}
