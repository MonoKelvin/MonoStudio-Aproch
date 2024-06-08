/****************************************************************************
 * @file    AColorSliderControl.cpp
 * @date    2021-11-08
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
#include "AColorSliderControl.h"

APROCH_NAMESPACE_BEGIN

AColorSliderControl::AColorSliderControl(EColorChannel channel, EFormat format, QWidget *parent)
    : QWidget(parent), mLayout(nullptr), mSlider(nullptr), mLabel(nullptr), mInput(nullptr)
{
    init(QColor(), channel, 0.0, format);
}

AColorSliderControl::AColorSliderControl(EColorChannel channel, float val, const QColor &baseColor, EFormat format, QWidget *parent)
    : QWidget(parent), mLayout(nullptr), mSlider(nullptr), mLabel(nullptr), mInput(nullptr)
{
    init(baseColor, channel, val, format);
}

void AColorSliderControl::setBaseColor(const QColor &color)
{
    float val = 0.0;
    switch (mChannel)
    {
    case Red:
        val = mChannel == Float ? color.redF() : color.red();
        break;
    case Green:
        val = mChannel == Float ? color.greenF() : color.green();
        break;
    case Blue:
        val = mChannel == Float ? color.blueF() : color.blue();
        break;
    case Alpha:
        val = mChannel == Float ? color.alphaF() : color.alpha();
        break;
    case Hue:
        val = mChannel == Float ? qBound(0.0f, float(color.hueF()) / 360.0f, 1.0f) : color.hue();
        break;
    case Saturation:
        val = mChannel == Float ? color.saturationF() : color.saturation();
        break;
    case Brightness:
        val = mChannel == Float ? color.valueF() : color.value();
        break;
    case Lightness:
        val = mChannel == Float ? color.lightnessF() : color.lightness();
        break;
    default:
        break;
    }

    setValue(val);
}

void AColorSliderControl::setIsFillColor(bool isFillColor)
{
    if (isFillColor)
    {
        mSlider->setStyleSheet(AStr("background-color:%1;").arg(getColor().name()));
    }
    else
    {
        mSlider->setStyleSheet(ANull_String);
    }
}

void AColorSliderControl::setLable(const QString &content)
{
    QString realContent = content.trimmed();
    if (realContent.isEmpty())
        realContent = getNameByChannel(mChannel);

    mLabel->setText(realContent);
    mLabel->updateGeometry();
}

QString AColorSliderControl::getNameByChannel(EColorChannel channel) const
{
    switch (channel)
    {
    case Red:
        return AStr("R");
    case Green:
        return AStr("G");
    case Blue:
    case Brightness:
        return AStr("B");
    case Alpha:
        return AStr("A");
    case Hue:
        return AStr("H");
    case Saturation:
        return AStr("S");
    case Lightness:
        return AStr("L");
    default:
        break;
    }

    return ANull_String;
}

float AColorSliderControl::getMaxRangeByFormat(EColorChannel channel, EFormat format) const
{
    if (format == Float)
        return 1.0f;

    switch (channel)
    {
    case Red:
    case Green:
    case Blue:
    case Alpha:
        return 255;
    case Hue:
        return 360;
    case Saturation:
    case Brightness:
    case Lightness:
        return 360;
    default:
        break;
    }

    return 0.0f;
}

void AColorSliderControl::setValue(float val)
{
    if (qFuzzyCompare(mValue, val))
        return;

    mValue = _bound(val);
    mSlider->setValue(_ivalue(mValue));
    mInput->setText(_svalue(mValue));

    Q_EMIT valueChanged(mValue);
}

void AColorSliderControl::_updateSliderValue(const QString &content)
{
    bool ok = false;
    float val = ((mFormat == Hex) ? float(content.toInt(&ok, 16)) : content.toFloat(&ok));
    val = ok ? val : 0.0f;
    if (!qFuzzyCompare(mValue, val))
    {
        mValue = _bound(val);
        mSlider->setValue(_ivalue(mValue));
        Q_EMIT valueChanged(mValue);
    }
}

void AColorSliderControl::_updateInputValue(int val)
{
    const float realVal = _fvalue(val);
    mSlider->setToolTip(_svalue(realVal));
    if (!qFuzzyCompare(mValue, realVal))
    {
        mValue = _bound(realVal);
        mInput->setText(_svalue(mValue));
        Q_EMIT valueChanged(mValue);
    }
}

void AColorSliderControl::init(const QColor &baseColor, EColorChannel channel, float val, EFormat format)
{
    mBaseColor = baseColor;
    mChannel = channel;
    mValue = val;
    mFormat = format;

    mLayout = new QBoxLayout(QBoxLayout::LeftToRight, this);
    mLabel = new QLabel(getNameByChannel(channel), this);
    mSlider = new QSlider(Qt::Horizontal, this);
    mInput = new QLineEdit(this);

    // Layout
    mLayout->setContentsMargins(0, 0, 0, 0);
    mLayout->setSpacing(10);
    mLayout->addWidget(mLabel);
    mLayout->addWidget(mSlider);
    mLayout->addWidget(mInput);

    connect(mInput, &QLineEdit::textChanged, this, &AColorSliderControl::_updateSliderValue);
    connect(mSlider, &QSlider::valueChanged, this, &AColorSliderControl::_updateInputValue);
    connect(mInput, &QLineEdit::editingFinished, this, [&]
            {
            QString content = mInput->text();
            switch (mFormat)
            {
            case AColorSliderControl::Float:
                mInput->setText(_fstring(content.toFloat()));
                break;
            case AColorSliderControl::Int:
                mInput->setText(_istring(content.toFloat()));
                break;
            case AColorSliderControl::Hex:
                if (content.size() == 1)
                    mInput->setText(content.insert(0, '0'));
                break;
            default:
                break;
            } });

    updateWidgetsData();
}

void AColorSliderControl::updateWidgetsData()
{
    // Slider
    const int maxRange = (mFormat == Float) ? 1000 : getMaxRangeByFormat(mChannel, mFormat);
    mSlider->setRange(0, maxRange);
    mSlider->setValue(_ivalue(mValue));
    mSlider->setPageStep(1);

    // Input
    mValue = _fvalue(mSlider->value());
    if (mChannel <= Alpha && mFormat == Hex)
    {
        QRegularExpressionValidator*validator = new QRegularExpressionValidator(mInput);
        validator->setRegularExpression(QRegularExpression(AStr("[\\dA-F]+"), QRegularExpression::CaseInsensitiveOption));
        mInput->setValidator(validator);
        mInput->setMaxLength(2);
        mInput->setText(_hstring(mValue));
    }
    else if (mFormat == Int)
    {
        QIntValidator *validator = new QIntValidator(0, maxRange, mInput);
        mInput->setValidator(validator);
        mInput->setText(_istring(mValue));
    }
    else
    {
        ADoubleValidator *validator = new ADoubleValidator(0, 1.0, 3, mInput);
        mInput->setValidator(validator);
        mInput->setText(_fstring(mValue));
    }
}

void AColorSliderControl::setOrientation(Qt::Orientation ori, const char *orderFlag)
{
    int lIndex = QString(orderFlag).indexOf('L', 0, Qt::CaseInsensitive);
    int sIndex = QString(orderFlag).indexOf('S', 0, Qt::CaseInsensitive);
    int eIndex = QString(orderFlag).indexOf('E', 0, Qt::CaseInsensitive);

    // 全部调整为 >=0 的不相等索引
    if (lIndex == -1)
        lIndex = qMax(sIndex, eIndex) + 1;
    if (sIndex == -1)
        sIndex = qMax(lIndex, eIndex) + 1;
    if (eIndex == -1)
        eIndex = qMax(lIndex, sIndex) + 1;

    QVector<QPair<int, QWidget *>> widgetsList;
    widgetsList.push_back(QPair<int, QWidget *>(lIndex, mLabel));
    widgetsList.push_back(QPair<int, QWidget *>(sIndex, mSlider));
    widgetsList.push_back(QPair<int, QWidget *>(eIndex, mInput));

    std::sort(widgetsList.begin(), widgetsList.end());

    mLayout->removeWidget(mLabel);
    mLayout->removeWidget(mSlider);
    mLayout->removeWidget(mInput);

    auto iter = widgetsList.cbegin();
    mLayout->addWidget(iter->second);
    ++iter;
    mLayout->addWidget(iter->second);
    ++iter;
    mLayout->addWidget(iter->second);

    mSlider->setOrientation(ori);
    mLayout->setDirection(ori == Qt::Horizontal ? QBoxLayout::LeftToRight : QBoxLayout::TopToBottom);
}

void AColorSliderControl::setChannel(EColorChannel channel)
{
    mChannel = channel;
    updateWidgetsData();
}

void AColorSliderControl::setFormat(EFormat format)
{
    mFormat = format;
    updateWidgetsData();
}

QColor AColorSliderControl::getColor() const
{
    QColor color = mBaseColor;
    switch (mChannel)
    {
    case Red:
        mChannel == Float ? color.setRedF(mValue) : color.setRed(mValue);
        break;
    case Green:
        mChannel == Float ? color.setGreenF(mValue) : color.setGreen(mValue);
        break;
    case Blue:
        mChannel == Float ? color.setBlueF(mValue) : color.setBlue(mValue);
        break;
    case Alpha:
        mChannel == Float ? color.setAlphaF(mValue) : color.setAlpha(mValue);
        break;
    case Hue:
        mChannel == Float ? color.setHsvF(mValue * 360.0, color.saturationF(), color.valueF(), color.alphaF()) : color.setHsv(mValue, color.saturation(), color.value(), color.alpha());
        break;
    case Saturation:
        mChannel == Float ? color.setHsvF(color.hueF(), mValue, color.valueF(), color.alphaF()) : color.setHsv(color.hue(), mValue, color.value(), color.alpha());
        break;
    case Brightness:
        mChannel == Float ? color.setHsvF(color.hueF(), color.saturationF(), mValue, color.alphaF()) : color.setHsv(color.hue(), color.saturation(), mValue, color.alpha());
        break;
    case Lightness:
        mChannel == Float ? color.setHslF(color.hueF(), color.saturationF(), mValue, color.alphaF()) : color.setHsl(color.hue(), color.saturation(), mValue, color.alpha());
        break;
    default:
        break;
    }

    return color;
}

APROCH_NAMESPACE_END
