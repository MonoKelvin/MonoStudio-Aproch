/****************************************************************************
 * @file    AColorSliderControl.h
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
#pragma once
class QLabel;

APROCH_NAMESPACE_BEGIN

/**
 * @brief 颜色通道滑动条
 * @note 默认创建的布局为从左到右依次是：标签、滑动条、输入控件
 * 各种通道格式为：
 *
 * 红通道(Red)：[0, 1] / [0, 255] / [0x00, 0xFF]
 * 绿通道(Green)：[0, 1] / [0, 255] / [0x00, 0xFF]
 * 蓝通道(Blue)：[0, 1] / [0, 255] / [0x00, 0xFF]
 * 透明通道(Alpha)：[0, 1] / [0, 255] / [0x00, 0xFF]
 * 色调(Hue)：[0, 1] / [0, 360]
 * 饱和度(Saturation)：[0, 1] / [0, 100]
 * 明度(Brightness)：[0, 1] / [0, 100]
 * 亮度(Lightness)：[0, 1] / [0, 100]
 *
 */
class APROCH_API AColorSliderControl : public QWidget
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(AColorSliderControl)
public:
    /**
     * @brief 颜色通道枚举
     */
    enum EColorChannel
    {
        Red,        // 红通道
        Green,      // 绿通道
        Blue,       // 蓝通道
        Alpha,      // 透明通道
        Hue,        // 色调
        Saturation, // 饱和度
        Brightness, // 明度(Value)
        Lightness,  // 亮度
    };
    Q_ENUM(EColorChannel)

    /**
     * @brief 数据格式
     */
    enum EFormat
    {
        Float, // 浮点数。所有通道通用
        Int,   // 整数。所有通道通用
        Hex,   // 十六进制，只用于R、G、B、A通道
    };
    Q_ENUM(EFormat)

    explicit AColorSliderControl(EColorChannel channel, EFormat format = EFormat::Int, QWidget *parent = nullptr);
    AColorSliderControl(EColorChannel channel, float val, const QColor &baseColor, EFormat format = EFormat::Int, QWidget *parent = nullptr);

    /**
     * @brief 获取基本颜色
     * @return 基本色
     */
    inline QColor getBaseColor() const noexcept
    {
        return mBaseColor;
    }

    /**
     * @brief 设置基本颜色
     * @param color 基本颜色
     */
    void setBaseColor(const QColor &color);

    /**
     * @brief 设置是否给滑动条背景色填充当前通道对应的颜色
     * @param isFillColor 是否填充背景颜色
     */
    void setIsFillColor(bool isFillColor);

    /**
     * @brief 设置布局方向
     * @param ori 方向
     * @param orderFlag 顺序标志
     * @note <orderFlag> 标识控件展示的顺序，L(Label)表示标签控件，S(Slider)表示滑动条控件，E(Edit)表示编辑框；如果<ori>为@see Qt::Horizontal
     * 则默认顺序为从左到右以此为：LSE，若<ori>为@see Qt::Vertical，则从上到下顺序默认为：LSE
     */
    void setOrientation(Qt::Orientation ori, const char *orderFlag = "LSE");

    /**
     * 设置通道
     * @param comp 通道类型
     */
    void setChannel(EColorChannel channel);

    /**
     * @brief 获取颜色通道类型
     * @return 颜色通道类型
     */
    inline EColorChannel getChannel() const noexcept
    {
        return mChannel;
    }

    /**
     * 设置格式
     * @param comp 格式类型
     */
    void setFormat(EFormat format);

    /**
     * @brief 获取颜色格式类型
     * @return 颜色格式类型
     */
    inline EFormat getFormat() const noexcept
    {
        return mFormat;
    }

    /**
     * @brief 获取对基本颜色@see getBaseColor 提取单通道后的颜色
     * @return 颜色
     */
    QColor getColor() const;

    /**
     * 获取通道值
     * @return 颜色通道值
     */
    inline float getValue() const noexcept
    {
        return mValue;
    }

    /**
     * @brief 获取滑动条控件
     * @return 滑动条控件
     */
    inline QSlider *getSlider() const noexcept
    {
        return mSlider;
    }

    /**
     * @brief 设置标签对应展示的内容。如果内容为空，则还原为默认的通道名称
     * @param content 内容
     */
    void setLable(const QString &content);

protected:
    /**
     * @brief 根据通道类型获取名称
     * @param channel 通道类型
     * @return 名称
     */
    QString getNameByChannel(EColorChannel channel) const;

    /**
     * 根据数据格式获取数据最大参数
     * @param channel 通道类型
     * @param format 格式
     * @return 数据的最大值
     */
    float getMaxRangeByFormat(EColorChannel channel, EFormat format) const;

Q_SIGNALS:
    /**
     * @brief 信号：对应通道类型值改变时触发
     */
    void valueChanged(float val);

public Q_SLOTS:
    /**
     * @brief 设置通道。如果越界则<val>默认为0
     */
    void setValue(float val);

private Q_SLOTS:
    /**
     * @brief 由输入框的数据更新滑动条值
     * @param content 输入框的内容
     */
    void _updateSliderValue(const QString &content);

    /**
     * @brief 由滑动条的值更新输入框内容
     * @param val 滑动条的值
     */
    void _updateInputValue(int val);

private:
    /**
     * @brief 初始化
     * @param baseColor 基本颜色
     * @param channel 通道类型
     * @param val 当前值
     * @param format 数据格式
     */
    void init(const QColor &baseColor, EColorChannel channel, float val, EFormat format);

    /**
     * @brief 更新控件的数据
     */
    void updateWidgetsData();

    /** @brief 一些辅助方法 */
private:
    /**
     * @brief 浮点转字符串
     * @param val 浮点值
     * @return 字符串
     */
    inline const QString _fstring(const float &val) const noexcept
    {
        return QString::number(qBound(0.0f, val, 1.0f), 'g', 3);
    }

    /**
     * @brief 浮点数转整数字符串
     * @param val 浮点数
     * @return 整数字符串
     */
    inline const QString _istring(const float &val) const noexcept
    {
        return QString::number(int(val));
    }

    /**
     * @brief 浮点数（先转整数）转16进制字符串
     * @param val 浮点数
     * @return 16进制字符串
     */
    inline const QString _hstring(const float &val) const noexcept
    {
        return QString::number(int(val), 16).toUpper();
    }

    /**
     * @brief 获取范围内的值
     * @param val 值
     * @return 范围内的值
     */
    inline const float _bound(const float &val) const noexcept
    {
        return qBound(0.0f, val, getMaxRangeByFormat(mChannel, mFormat));
    }

    /**
     * @brief 获取正确的浮点值
     * @param val 整数值
     * @return 浮点值
     */
    inline const float _fvalue(const int &val) const noexcept
    {
        return (mFormat == Float) ? float(val) / 1000.0f : float(val);
    }

    /**
     * @brief 获取正确的整数值
     * @param val 浮点值
     * @return 整数值
     */
    inline const int _ivalue(const float &val) const noexcept
    {
        return (mFormat == Float) ? int(qBound(0.0f, val, 1.0f) * 1000) : int(val);
    }

    /**
     * @brief 获取正确的整数值
     * @param val 浮点值
     * @return 整数值
     */
    inline const QString _svalue(const float &val) const noexcept
    {
        return (mFormat == Float) ? _fstring(val) : (mFormat == Hex ? _hstring(val) : _istring(val));
    }

protected:
    /** @brief 布局 */
    QBoxLayout *mLayout;

    /** @brief 标签 */
    QLabel *mLabel;

    /** @brief 滑动条 */
    QSlider *mSlider;

    /** @brief 输入框 */
    QLineEdit *mInput;

    /** @brief 基本颜色 */
    QColor mBaseColor;

    /** @brief 当前值 */
    float mValue;

    /** @brief 通道类型 */
    EColorChannel mChannel;

    /** @brief 通道记录格式类型 */
    EFormat mFormat;
};

APROCH_NAMESPACE_END
