/****************************************************************************
 * @file    ANumberValidator.h
 * @date    2021-8-28
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

#include <cmath>
#include <QValidator>
#include <QLocale>

APROCH_NAMESPACE_BEGIN

/**
 * @brief 数字校验器
 * @tparam T 数据类型
 */
template <class T>
class ANumberValidator : public QValidator
{
    Q_DISABLE_COPY(ANumberValidator)
    Q_PROPERTY(int bottom READ getBottom WRITE setBottom)
    Q_PROPERTY(int top READ getTop WRITE setTop)
    Q_PROPERTY(int decimals READ getDecimals WRITE setDecimals)
public:
    explicit ANumberValidator(QObject *parent = nullptr)
        : QValidator(parent), mBottom(Minimum()), mTop(Maximun()), mDecimals(3)
    {
    }

    using value_type = T;

    /**
     * @brief 设置最小值
     * @param bottom 最小值
     */
    void setBottom(T bottom)
    {
        setRange(bottom, getTop());
    }

    /**
     * @brief 设置最大值
     * @param top 最大值
     */
    void setTop(T top)
    {
        setRange(getBottom(), top);
    }

    /**
     * @brief 设置范围
     * @param bottom 最大值
     * @param top 最小值
     */
    void setRange(T bottom, T top)
    {
        bool rangeChanged = false;
        if (bottom > top)
        {
            T temp = bottom;
            bottom = top;
            top = temp;
        }

        bottom = qMax(Minimum(), bottom);
        if (mBottom != bottom)
        {
            mBottom = bottom;
            rangeChanged = true;
        }

        top = qMin(Maximun(), top);
        if (mTop != top)
        {
            mTop = top;
            rangeChanged = true;
        }
    }

    /**
     * @brief 获取小数位数
     * @return 小数位数
     */
    int getDecimals() const noexcept
    {
        return mDecimals;
    }

    /**
     * @brief 设置小数位数，只有当类型<T>为<float>或者<double>时，设置才起作用
     * @param decimals 位数。要求大于等于零
     */
    inline void setDecimals(int decimals)
    {
        mDecimals = qMax(0, decimals);
    }

    /**
     * @brief 数据类型<T>的最小可表示的值
     * @return 最小值
     */
    static T Minimum()
    {
        return (std::numeric_limits<T>::min)();
    }

    /**
     * @brief 数据类型<T>的最大可表示的值
     * @return 最大值
     */
    static T Maximun()
    {
        return (std::numeric_limits<T>::max)();
    }

    /**
     * @brief 获取最小值
     * @return 最小值
     */
    inline T getBottom() const noexcept
    {
        return mBottom;
    }

    /**
     * @brief 获取最大值
     * @return 最大值
     */
    inline T getTop() const noexcept
    {
        return mTop;
    }

    /**
     * @brief 是否是整数类型，模板参数<T>是<float>或者<double>就是小数类型，否则就是整数类型
     * @return 是否是整数类型
     */
    bool isInteger() const noexcept
    {
        return (typeid(T) != typeid(float)) && (typeid(T) != typeid(double));
    }

    virtual State validate(QString &input, int &) const override
    {
        if (input.isEmpty())
            return Intermediate;

        const bool startsWithMinus(input[0] == '-');
        if (mBottom >= 0 && startsWithMinus)
            return Invalid;

        const bool startsWithPlus(input[0] == '+');
        if (mTop < 0 && startsWithPlus)
            return Invalid;

        if (input.size() == 1 && (startsWithPlus || startsWithMinus))
            return Intermediate;

        if (isInteger())
            return validateInteger(input, startsWithPlus);
        else
            return validateDouble(input);
    }

    virtual void fixup(QString &input) const override
    {
        if (isInteger())
        {
            bool ok;
            qlonglong entered = input.toLongLong(&ok);
            if (ok)
                input = locale().toString(entered);
        }
    }

protected:
    /**
     * @brief 校验整数类型数据
     * @param input 输入字符串
     * @param startsWithPlus 开始标志是否是'+'符号
     * @return 状态
     */
    State validateInteger(QString &input, bool startsWithPlus) const
    {
        bool ok = false;
        qlonglong entered = input.toLongLong(&ok);
        if (!ok)
            return Invalid;

        if (entered >= mBottom && entered <= mTop)
        {
            locale().toLongLong(input, &ok);
            return ok ? Acceptable : Intermediate;
        }

        if (entered >= 0)
        {
            int buffLength = input.size();
            if (startsWithPlus)
                buffLength--;
            const int tLength = mTop != 0 ? static_cast<int>(std::log10(qAbs(mTop))) + 1 : 1;
            return (entered > mTop && -entered < mBottom && buffLength > tLength) ? Invalid : Intermediate;
        }

        return (entered < mBottom) ? Invalid : Intermediate;
    }

    /**
     * @brief 校验浮点类型
     * @param input 输入字符串
     * @return 状态
     */
    State validateDouble(QString &input) const
    {
        if (input.isEmpty())
            return QValidator::Intermediate;

        bool ok = false;
        double i = locale().toDouble(input, &ok); // returns 0.0 if !ok
        if (i == std::numeric_limits<double>::quiet_NaN())
            return QValidator::Invalid;
        if (!ok)
            return QValidator::Intermediate;

        if (i >= mBottom && i <= mTop)
            return QValidator::Acceptable;

        double max = qMax(qAbs(mBottom), qAbs(mTop));
        if (max < LLONG_MAX)
        {
            qlonglong n = AMathToolkit::Pow10(AMathToolkit::NumDigits(qlonglong(max)));
            if (qAbs(i) > (n - std::pow(10, -mDecimals)))
                return QValidator::Invalid;
        }

        return QValidator::Intermediate;
    }

private:
    /** @brief 最小值 */
    T mBottom;

    /** @brief 最大值 */
    T mTop;

    /** @brief 小数位数 */
    int mDecimals;
};

APROCH_NAMESPACE_END
