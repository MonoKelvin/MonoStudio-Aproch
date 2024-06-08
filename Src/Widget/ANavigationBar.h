/****************************************************************************
 * @file    ACaptionBar.h
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
#include <QAbstractScrollArea>

#define AOBJNAME_NAVBAR AStr("aproch_navbar")
#define AOBJNAME_NAVBAR_ITEM AStr("aproch_navbar_item")
#define AOBJNAME_NAVBAR_TRACKBAR AStr("aproch_navbar_trackbar")
#define AOBJNAME_NAVBAR_GROUP AStr("aproch_navbar_group")

class QButtonGroup;

APROCH_NAMESPACE_BEGIN

/**
 * @brief 导航栏的追踪条样式选项
 * @note 更多和显示相关的样式通过样式表来完成，如可以通过样式表设置背景色和圆角等，否则即使
 * 开启追踪条，导航栏默认也不会显示出来，因为没有背景色
 */
struct SNavTrackBarStyle
{
    /**
     * @brief 偏移位置
     *
     * 如果开启（isEnabled = true）追踪条，则：
     * - 水平导航栏中其位置在导航按钮下方，即 TrackBar 底部和导航栏底部平齐，此时
     *   offset == 0，如果 offset == -1，则TrackBar顶部和导航栏顶部平齐；否则随着
     *   offset增加其位置从底部开始上移，随着offset减少其位置从底部开始下移
     * - 垂直导航栏中其位置在导航按钮左方，即 TrackBar 左部和导航栏左部平齐，此时
     *   offset == 0，如果 offset == -1，则TrackBar右部和导航栏右部平齐；否则随着
     *   offset增加其位置从左部开始右移，随着offset减少其位置从右部开始左移
     *
     * @note 该偏移值不受导航栏的 spacing 和 margin 影响
     */
    int offset;

    /**
     * @brief 动画类型，isAnimation为true时该值有效。同时该值也控制导航动画的类型
     * @see QEasingCurve::Type
     */
    int animationType;

    /**
     * @brief 动画时长，单位为毫秒(ms)，isAnimation为true时该值有效。同时该值也控制导航
     * 栏动画持续时间
     */
    int duration;

    /** @brief 是否长度/宽度适应导航按钮的长度/宽度 */
    bool isFitItem;

    /**
     * @brief 是否使用动画。
     */
    bool isAnimation;

    /**
     * @brief 是否使用拉伸动画，isAnimation为true时该值有效。
     * @todo 下一个版本中完成
     */
    bool isStretchingAnimation;

    /** @brief 是否使用 TrackBar */
    bool isEnabled;

    SNavTrackBarStyle(void)
        : offset(0), animationType(1) // InQuad
          ,
          duration(250), isFitItem(true), isAnimation(true), isStretchingAnimation(false), isEnabled(true)
    {
    }
};

/**
 * @brief 导航栏
 * @note
 * 导航栏中每一个可导航的按钮称为导航项(item)，每个item都有一个索引表示它的位置，对于垂直导航
 * 栏其索引（如果存在至少一个item）顺序为从上到下，从0开始；水平导航栏的索引顺寻为从左到右，从
 * 0开始。
 *
 * 导航栏中附带一个“追踪条”控件，可以通过 SNavTrackBarStyle 参数进行风格化设置，其作用时为
 * 了跟踪当前选中的item，一般常见的是在垂直导航栏中item位置的左侧，当选中某个item时，其位置就
 * 会竖直移动到该item位置；类似的，在水平导航栏中常见的是在item位置底部，当选中某个item时，其
 * 位置就会水平移动到该item位置。
 *
 * @note 当前导航项再次点击进行导航时不会触发导航事件
 * @note 当导航栏首次显示（showEvent）时，会自动触发一次导航，默认第一个导航项被选中
 */
class APROCH_API ANavigationBar : public QAbstractScrollArea
{
    Q_OBJECT
    Q_PROPERTY(int hScrollValue READ getHScrollValue WRITE setHScrollValue)
    Q_PROPERTY(int vScrollValue READ getVScrollValue WRITE setVScrollValue)
public:
    explicit ANavigationBar(QWidget *parent = nullptr);
    explicit ANavigationBar(Qt::Orientation ori, QWidget *parent = nullptr);
    ~ANavigationBar();

    /**
     * @brief 获取当前水平滚动条滚动的值，相当于 horizontalScrollBar()->value();
     * @return 当前水平滚动条滚动的值
     * @see horizontalScrollBar()
     */
    int getHScrollValue(void) const noexcept;

    /**
     * @brief 设置当前水平滚动条滚动的值，不带动画
     * @param value 滚动值
     */
    void setHScrollValue(int value);

    /**
     * @brief 获取当前垂直滚动条滚动的值，相当于 verticalScrollBar()->value();
     * @return 当前垂直滚动条滚动的值
     * @see verticalScrollBar()
     */
    int getVScrollValue(void) const noexcept;

    /**
     * @brief 设置当前垂直滚动条滚动的值，不带动画
     * @param value 滚动值
     */
    void setVScrollValue(int value);

    /**
     * @brief 添加一个导航项
     * @param icon 导航项显示的图标
     * @param text 导航项显示的文字
     * @param index 要添加到导航栏的位置。0是开始位置，小于零则添加到最后
     */
    void addItem(const QString &text, const QIcon &icon = QIcon(), int index = -1);

    /**
     * @brief 获得导航项数目
     * @return 导航项数目
     */
    int getItemCount(void) const noexcept;

    /**
     * @brief 设置当前导航项大小。垂直导航栏会自动忽略w值，水平导航栏会自动忽略h值
     * @param w 宽度。
     * @param h 高度。
     * @param index 要设置的导航项的索引，如果小于0就全部同时设置
     * @note 设置导航项大小是针对当前导航栏中已有的导航项的，如果之后再添加导航项则
     * 不会进行大小的设置
     */
    void setItemSize(int w, int h, int index = -1);

    /**
     * @brief 设置导航项是否激活
     * @param index 导航项索引。如果索引不存在则什么也不做
     * @param isEnabled 是否激活导航项
     * - true: 激活导航项，则该导航项可以选中并触发导航信号等
     * - false: 取消激活导航项，则该导航项不可被选中，不在接收点击事件等，即使该导航项已经被选中了
     */
    void setItemEnabled(int index, bool isEnabled);

    /**
     * @brief 设置导航项大小。垂直导航栏会自动忽略宽度，水平导航栏会自动忽略高度
     * @param size 导航项大小
     * @param index 要设置的导航项的索引，如果小于0就全部同时设置
     */
    inline void setItemSize(const QSize &size, int index = -1)
    {
        setItemSize(size.width(), size.height(), index);
    }

    /**
     * @brief 移除指定位置的导航项
     * @param index 导航项的索引，如果索引不存在则什么也不做
     */
    void removeItem(int index);

    /**
     * @brief 根据索引获取导航项按钮
     * @param index 导航项索引
     * @return 返回指定索引的导航项，如果索引不存在则返回空
     */
    QPushButton *getItem(int index);

    /**
     * @brief 获得当前选中的导航项按钮
     * @return 返回当前选中的导航项按钮，如果未选中则返回nullptr
     * @see getPrevIndex()
     */
    QPushButton *getCurrentItem(void) const;

    /**
     * @brief 获得当前选中的导航索引
     * @return 返回当前选中的导航索引，如果未选中则返回-1
     * @see getPrevIndex()
     */
    int getCurrentIndex(void) const;

    /**
     * @brief 获得上一次导航的导航索引值
     * @return 上一次导航的导航索引值
     * @see getCurrentIndex(), getCurrentItem()
     */
    inline int getPrevIndex(void) const noexcept
    {
        return mPrevIndex;
    }

    /**
     * @brief 添加分组名称
     * @param text 分组文本内容
     * @param icon 分组文本图标
     * @param index 要添加到导航栏的位置。0是开始位置，小于零则添加到最后。其中索引参照其他分组项和导航项的个数
     */
    void addGroup(const QString &text, const QIcon &icon = QIcon(), int index = -1);

    /**
     * @brief 根据索引获取分组项标签
     * @param index 分组项索引
     * @return 返回指定索引的分组项，如果索引不存在则返回空
     */
    QLabel *getGroup(int index);

    /**
     * @brief 获取当前导航栏的方向
     * @return 返回导航栏的方向
     *  - Vertical 垂直方向
     *  - Horizontal 水平方向
     * @see Qt::Orientation
     */
    Qt::Orientation getOrientation(void) const;

    /**
     * @brief 获取导航栏中包含导航项的内容控件
     * @note 尽量不要使用该函数去修改内容控件，否则会造成难以预料的结果，请使用const版本
     * @return 内容控件
     */
    inline QWidget *getContent(void) noexcept
    {
        return mContent;
    }

    /**
     * @brief 获取导航栏中包含导航项的内容控件
     * @return 内容控件
     */
    inline const QWidget *getContent(void) const noexcept
    {
        return mContent;
    }

    /**
     * @brief 获取追踪条控件
     * @return 追踪条控件
     * @see SNavTrackBarStyle
     */
    inline QWidget *getTrackBar(void) const noexcept
    {
        return mTrackBar;
    }

    /**
     * @brief 获取追踪条控件样式常量引用
     * @return 追踪条控件样式的常量引用，不可修改
     * @see SNavTrackBarStyle
     */
    inline const SNavTrackBarStyle &getTrackBarStyle(void) const noexcept
    {
        return mTrackBarStyle;
    }

    /**
     * @brief 获取追踪条控件样式的引用。如果修改了样式，一定要使用 updateTrackBarStyle 方法
     * 更新样式，否则某些样式无法应用上，甚至导致不可预料的后果。
     * @return 追踪条控件样式引用，可以修改
     * @see updateTrackBarStyle()
     */
    inline SNavTrackBarStyle &getTrackBarStyle(void) noexcept
    {
        return mTrackBarStyle;
    }

    /**
     * @brief 设置追踪条样式。该方法会自动调用 updateTrackBarStyle
     * @param trackBarStyle 追踪条样式
     * @see updateTrackBarStyle()
     */
    inline void setTrackBarStyle(const SNavTrackBarStyle &trackBarStyle)
    {
        mTrackBarStyle = trackBarStyle;
        updateTrackBarStyle();
    }

    /**
     * @brief 更新追踪条样式，建议使用 getTrackBarStyle() 并修改了样式后调用该函数保持追踪条
     * 的更新，防止某些样式无法应用
     * @param stopAnimation 是否更新时强制停止使用追踪条的动画，默认为 true
     */
    void updateTrackBarStyle(bool stopAnimation = true);

    /**
     * @brief 设置边界偏移。
     * @param value 边界偏移
     */
    void setBoundaryOffset(int value);

    /**
     * @brief 获取边界偏移
     * @return 边界偏移
     */
    inline int getBoundaryOffset(void) const noexcept
    {
        return mBoundaryOffset;
    }

    /**
     * @brief 设置每个导航项的间距
     * @param spacing 间距
     */
    void setSpacing(int spacing);

    /**
     * @brief 获得导航项间距
     * @return 导航项间距
     */
    int getSpacing(void) const;

    /**
     * @brief 设置导航栏的四个方向的内边距
     * @param margins 导航栏内边距
     */
    void setMargins(int margins);

    /**
     * @brief 设置导航栏内边距
     * @param left 左边距
     * @param top 上边距
     * @param right 右边距
     * @param bottom 下边距
     */
    void setMargins(int left, int top, int right, int bottom);

    /**
     * @brief 获得导航栏内边距
     * @return 导航栏内边距
     * @see QMargins
     */
    QMargins getMargins(void) const;

    /**
     * @brief 获得导航项到`滚动区域内容的最左/上`部分的距离，不是到导航栏最左/上侧
     * @param index 导航项索引，小于0表示当前选中的导航项，默认-1
     * @param isCenter 是否计算的是导航项中心到滚动区域内容的最左/上部分的距离
     * @return 距离，返回-1说明获取导航项失败
     * @see getItemOffset()
     */
    int getItemDistance(int index = -1, bool isCenter = true);

    /**
     * @brief 获得导航项到`导航栏的最左/上`部分的偏移距离，不是到滚动区域内容的最左/上侧
     * @param index 导航项索引，小于0表示当前选中的导航项，默认-1
     * @param isCenter 是否计算的是导航项中心到导航栏的最左/上部分的偏移距离
     * @return 偏移距离
     * @note 该函数不会对空的导航项进行计算，所以当索引未找到导航项时，返回0
     * @see getItemDistance()
     */
    int getItemOffset(int index = -1, bool isCenter = true);

public Q_SLOTS:
    /**
     * @brief 槽：导航。调用该函数后相对应的导航项会被选中，即使该导航项处于非激活状态
     * @param index 要导航到的索引。
     */
    void navigate(int index);

    /**
     * @brief 带动画的相对于视图的滚动，当两个参数都不为0时进行的是并行动画
     *
     * 当想要视图（不是滚动条）向上滚动时，应设置x的值小于零，这时滚动条则是相反下方
     * 滚动
     *
     * @note 超过可以滚动的最大值（滚动条最大值和当前值之差）时将会忽略多余的滚动内容
     *
     * @param x 水平方向需要滚动的像素大小
     * @param y 垂直方向需要滚动的像素大小
     */
    void scroll(int h, int v);

Q_SIGNALS:
    /**
     * @brief 信号：当导航时触发
     * @param item 当前导航的导航项索引
     */
    void navigated(int index);

protected:
    bool event(QEvent *e) override;
    bool eventFilter(QObject *o, QEvent *e) override;
    void resizeEvent(QResizeEvent *event) override;
    void scrollContentsBy(int, int) override;
    void showEvent(QShowEvent *e) override;

private:
    void _init(Qt::Orientation ori);
    void _updateItemId(void);
    void _updateScrollBars(void);
    void _updateContentPosition(void);
    int _updateItemPosition(int index);

    /** @brief 找到导航项所在布局中的索引 */
    int _findNavItemIndex(int index) const;

    /** @brief 找到分组项所在布局中的索引 */
    int _findNavGrouppIndex(int index) const;

    /** @brief 找到控件所在布局中的索引 */
    template <typename Type>
    int _findWidgetIndexHelper(int index, const QList<Type> &container) const
    {
        QList<QLayoutItem *> layoutItems;
        for (int i = 0; i < mLayout->count(); ++i)
            layoutItems.push_back(mLayout->itemAt(i));

        if (layoutItems.empty())
            return 0;

        if (container.empty() || index < 0)
            return mLayout->indexOf(layoutItems.back()) + 1;

        if (index > layoutItems.size())
            return mLayout->indexOf(container.back()) + 1;

        int realIndex = 0;
        while (index >= 0 && realIndex < layoutItems.size())
        {
            Type navBtn = qobject_cast<Type>(layoutItems[realIndex++]->widget());
            if (!navBtn)
                continue;
            if (container.contains(navBtn))
                index--;
        }

        return realIndex;
    }

    /** @brief 找到布局中指定控件的索引 */
    int _findIndexOfWidgetInLayout(QWidget *widget) const;

private:
    /** @brief 导航追踪条样式 */
    SNavTrackBarStyle mTrackBarStyle;

    /** @brief 导航按钮组 */
    QButtonGroup *mNavGroup;

    /** @brief 分组标签页集合 */
    QList<QLabel *> mNavGroupVector;

    /** @brief 内容容器 */
    QWidget *mContent;

    /** @brief 内容布局 */
    QBoxLayout *mLayout;

    /** @brief 追踪条控件 */
    QWidget *mTrackBar;

    /**
     * @brief 边界偏移。用于逆向追踪时确保追踪条和导航项始终处于可视位置。该值最大为垂直/水平
     * 导航栏两边按钮的最大高度/宽度，如果超过则多余的值将被忽略。
     *
     * - >=0: 有效的边界偏移量，即和导航栏一边始终保持该值的间距
     * - <0: 不使用边界逆向追踪
     *
     * @note 逆向追踪时动画是否开启、动画持续时间以及动画类型都是通过 mTrackBarStyle 参数同时
     * 控制的
     *
     * @see setBoundaryOffset
     */
    int mBoundaryOffset;

    /** @brief 之前导航的索引，如果没有则为-1 */
    int mPrevIndex;

private:
    Q_DISABLE_COPY_MOVE(ANavigationBar)
};

APROCH_NAMESPACE_END
