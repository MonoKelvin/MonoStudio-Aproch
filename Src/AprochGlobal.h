/****************************************************************************
 * @file    AprochGlobal.h
 * @date    2021-1-10
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

#include <mutex>

#include <QtCore/qglobal.h>
#include <QVariant>
#include <QPointer>
#include <QStyleOption>
#include <QStylePainter>

#include "AprochExportDefine.h"

#define A_QUOTE_X(x) #x
#define A_QUOTE(x) A_QUOTE_X(x)

#if (QT_VERSION >= QT_VERSION_CHECK(5, 8, 0))
#define A_FALLTHROUGH() Q_FALLTHROUGH()
#else
#define A_FALLTHROUGH()
#endif

#if defined(APROCH_USE_NAMESPACE)
#define APROCH_NAMESPACE aproch
#endif

#if !defined(APROCH_NAMESPACE)
#define APROCH_NAMESPACE_BEGIN
#define APROCH_NAMESPACE_END
#define APROCH_CLASSNAME(classname) classname
#define APROCH_PRE_NAMESPACE(classname) classname
#define APROCH_META_CLASSNAME(classname) classname
#else
#define APROCH_NAMESPACE_BEGIN namespace APROCH_NAMESPACE {
#define APROCH_NAMESPACE_END }
#define APROCH_CLASSNAME(classname) APROCH_NAMESPACE::classname
#define APROCH_PRE_NAMESPACE(classname) APROCH_NAMESPACE::classname
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#define APROCH_META_CLASSNAME(classname) A_QUOTE(APROCH_NAMESPACE::)classname
#else
#define APROCH_META_CLASSNAME(classname) classname
#endif
#endif

/** 定义实现接口宏 */
#ifdef implement
#undef implement
#endif
#define implement public

/** 定义抽象类类型关键字 */
#ifdef abstract
#undef abstract
#endif
#define abstract

#ifndef M_2PI
#define M_2PI 6.28318530717958647692
#endif

/**
 * @brief 常量字符串转换为编译器构造的QString，用以降低运行时构造QString的代价
 * @param _ConstString_ 常量字符串，如: "String"
 * @note 等价于使用 QStringLiteral(_ConstString_)
 * @see QStringLiteral
 */
#define AStr(_ConstString_) QStringLiteral(_ConstString_)

/**
 * @brief 常量字符串转换为单字节字符串QLatin1String，用在所有可以支持QLatin1String参数的地方
 * @param _ConstString_ 常量字符串，如: "String"
 * @note 等价于使用 QLatin1String(_ConstString_)
 * @see QLatin1String
 */
#define ALatin1(_ConstString_) QLatin1String(_ConstString_)

/**
 * @brief 常量字符串转换为字节数组QByteArray，用以降低运行时构造QByteArray的代价
 * @param _ConstString_ 常量字符串，如: "Byte Array"
 * @note 等价于使用 QByteArrayLiteral(_ConstString_)
 * @see QByteArrayLiteral
 */
#define AByteArr(_ConstString_) QByteArrayLiteral(_ConstString_)

/** @brief 获取三个数中最大的数 */
#define AMax3(a, b, c) ((a) > (b) ? ((a) > (c) ? (a) : (c)) : ((b) > (c) ? (b) : (c)))

/** @brief 获取三个数中最小的数 */
#define AMin3(a, b, c) ((a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b) : (c)))

/** 使用该宏来声明单例类，注意声明的单例类的析构函数必须修饰为public */
#define APROCH_SINGLETON(_ClassName_)                              \
public:                                                            \
    static QSharedPointer<_ClassName_> getInstance(void) noexcept; \
                                                                   \
private:                                                           \
    static QSharedPointer<_ClassName_> mInstance;                  \
    static std::mutex mMutex

/** 使用该宏来初始化单例和互斥锁 */
#define APROCH_INIT_SINGLETON(_ClassName_)                                  \
    QSharedPointer<_ClassName_> _ClassName_::mInstance(nullptr);            \
    std::mutex _ClassName_::mMutex;                                         \
    QSharedPointer<_ClassName_> _ClassName_::getInstance(void) noexcept     \
    {                                                                       \
        if (nullptr == mInstance)                                           \
        {                                                                   \
            std::lock_guard<std::mutex> guard(mMutex);                      \
            if (nullptr == mInstance)                                       \
            {                                                               \
                mInstance = QSharedPointer<_ClassName_>(new _ClassName_()); \
            }                                                               \
        }                                                                   \
        return mInstance;                                                   \
    }

/** 使用该宏来声明单例类，注意声明的单例类的析构函数必须修饰为public */
#define APROCH_OBJECT_SINGLETON(_ClassName_)                 \
public:                                                      \
    static QPointer<_ClassName_> getInstance(void) noexcept; \
                                                             \
private:                                                     \
    static QPointer<_ClassName_> mInstance;                  \
    static std::mutex mMutex

/** 使用该宏来初始化单例和互斥锁 */
#define APROCH_OBJECT_INIT_SINGLETON(_ClassName_)                     \
    QPointer<_ClassName_> _ClassName_::mInstance(nullptr);            \
    std::mutex _ClassName_::mMutex;                                   \
    QPointer<_ClassName_> _ClassName_::getInstance(void) noexcept     \
    {                                                                 \
        if (nullptr == mInstance)                                     \
        {                                                             \
            std::lock_guard<std::mutex> guard(mMutex);                \
            if (nullptr == mInstance)                                 \
            {                                                         \
                mInstance = QPointer<_ClassName_>(new _ClassName_()); \
            }                                                         \
        }                                                             \
        return mInstance;                                             \
    }

extern APROCH_API const QVariant ANull_Variant;    // 空变量
extern APROCH_API const QString ANull_String;      // 空字符串
extern APROCH_API const QString AKey_True;         // true
extern APROCH_API const QString AKey_False;        // false
extern APROCH_API const QString AKey_0;            // 0
extern APROCH_API const QString AKey_1;            // 1
extern APROCH_API const QString AKey_LeftBracket;  // (
extern APROCH_API const QString AKey_RightBracket; // )
extern APROCH_API const QString AKey_Id;           // id
extern APROCH_API const QString AKey_Default;      // default
extern APROCH_API const QString AKey_Value;        // value
extern APROCH_API const QString AKey_Type;         // type
extern APROCH_API const QString AKey_Name;         // name
extern APROCH_API const QString AKey_Slash;        // 正斜杠"/"
extern APROCH_API const QString AKey_Backslash;    // 反斜杠"\"
extern APROCH_API const QString AKey_Comma;        // 逗号","
extern APROCH_API const QString AKey_NewLine;      // 换行符"\n"
extern APROCH_API const QString AKey_Table;        // 指标符"\t"
extern APROCH_API const QString AKey_RGB;          // "RGB"
extern APROCH_API const QString AKey_ARGB;         // "ARGB"

extern APROCH_API const QString AKey_Const; // 常数"const"
extern APROCH_API const QString AKey_Ptr;   // 指针"pointer"
extern APROCH_API const QString AKey_Ref;   // 引用"reference"
extern APROCH_API const QString AKey_Zero;  // 零"0"
extern APROCH_API const QString AKey_One;   // 壹"1"
extern APROCH_API const QString AKey_NAN;   // 非数字"nan"
extern APROCH_API const QString AKey_INF;   // 无限大"inf"

extern APROCH_API const QString AKey_Bool;
extern APROCH_API const QString AKey_Boolean;
extern APROCH_API const QString AKey_String;
extern APROCH_API const QString AKey_UChar;
extern APROCH_API const QString AKey_Char;
extern APROCH_API const QString AKey_UShort;
extern APROCH_API const QString AKey_Short;
extern APROCH_API const QString AKey_Int;
extern APROCH_API const QString AKey_UInt;
extern APROCH_API const QString AKey_Float;
extern APROCH_API const QString AKey_Double;
extern APROCH_API const QString AKey_ULong;
extern APROCH_API const QString AKey_Long;
extern APROCH_API const QString AKey_LongLong;
extern APROCH_API const QString AKey_ULongLong;
extern APROCH_API const QString AKey_BigNumber; // 大整数(-inf, +inf)
extern APROCH_API const QString AKey_Real;      // 实数"real"
extern APROCH_API const QString AKey_StdString; // 标准库字符串"std::string"
extern APROCH_API const QString AKey_Byte;      // 字节"byte"

extern APROCH_API const QStringList ASL_BaseDataType;

extern APROCH_API const QString AKey_Vector;
extern APROCH_API const QString AKey_List;
extern APROCH_API const QString AKey_Map;
extern APROCH_API const QString AKey_UnorderedMap;
extern APROCH_API const QString AKey_Hash;
extern APROCH_API const QString AKey_Array;
extern APROCH_API const QString AKey_Set;

/** Properties Name */

extern APROCH_API const char* APropName_BtnType;    // 按钮类型
extern APROCH_API const char* APropValue_TextBtn;   // 文字按钮
extern APROCH_API const char* APropName_WidgetType; // 控件类型
extern APROCH_API const char* APropValue_Panel;     // 面板
extern APROCH_API const char* APropValue_Single;    // 简单风格模式

/** @brief 判断两个类是否有继承关系的静态断言 */
#define APROCH_ASSERT_IS_DERIVED(_BaseName_, _DerivedName_) \
    static_assert(std::is_base_of<_BaseName_, _DerivedName_>::value, "Derive Error")

/** @brief 获取服务 */
#define APROCH_SERVICE(_ServiceInterface_, _ServiceName_) \
    qSharedPointerCast<_ServiceInterface_, APROCH_CLASSNAME(IService)>(APROCH_CLASSNAME(AAppContext)::getInstance()->getService(_ServiceName_))

/** @brief 判断两个类是否有继承关系，并且继承自<_BaseName_>的类需要Q_GADGET或者Q_OBJECT的元对象声明宏 */
#define APROCH_ASSERT_IS_DERIVED_BY_METAOBJECT(_BaseName_, _DerivedName_) \
    APROCH_ASSERT_IS_DERIVED(_BaseName_, _DerivedName_);                  \
    Q_ASSERT_X(_DerivedName_::staticMetaObject.className() != AStr(#_BaseName_), __FILE__, "Missing declaration of Q_GADGET or Q_OBJECT")

/** @brief 成员变量 */
#define A_M_VAR(_Var_) m_##_Var_

/** @brief 变量的修改信号函数 */
#define A_CHANGED_SIGNAL_FUNC(_Var_) _Var_##Changed

/** @brief Qt属性的声明和定义，带有修改信号 */
#define A_DEFINE_PROPERTY(_Type_, _Var_, _FuncName_)                                                        \
public:                                                                                                     \
    inline _Type_ get##_FuncName_(void) const noexcept { return A_M_VAR(_Var_); }                           \
    inline void set##_FuncName_(const _Type_ &_arg)                                                         \
    {                                                                                                       \
        if (A_M_VAR(_Var_) != _arg)                                                                         \
        {                                                                                                   \
            A_M_VAR(_Var_) = _arg;                                                                          \
            Q_EMIT A_CHANGED_SIGNAL_FUNC(_Var_)(A_M_VAR(_Var_));                                            \
        }                                                                                                   \
    }                                                                                                       \
Q_SIGNALS:                                                                                                  \
    void A_CHANGED_SIGNAL_FUNC(_Var_)(const _Type_ &);                                                      \
private:                                                                                                    \
    Q_PROPERTY(_Type_ _Var_ READ get##_FuncName_ WRITE set##_FuncName_ NOTIFY A_CHANGED_SIGNAL_FUNC(_Var_)) \
    _Type_ A_M_VAR(_Var_)

/** @brief Qt指针类型属性的声明和定义，带有修改信号 */
#define A_DEFINE_PROPERTY_PTR(_Type_, _Var_, _FuncName_)                                                    \
public:                                                                                                     \
    inline _Type_ get##_FuncName_(void) const noexcept { return A_M_VAR(_Var_); }                           \
    inline void set##_FuncName_(_Type_ _arg)                                                                \
    {                                                                                                       \
        if (A_M_VAR(_Var_) != _arg)                                                                         \
        {                                                                                                   \
            A_M_VAR(_Var_) = _arg;                                                                          \
            Q_EMIT A_CHANGED_SIGNAL_FUNC(_Var_)(A_M_VAR(_Var_));                                            \
        }                                                                                                   \
    }                                                                                                       \
Q_SIGNALS:                                                                                                  \
    void A_CHANGED_SIGNAL_FUNC(_Var_)(_Type_);                                                              \
private:                                                                                                    \
    Q_PROPERTY(_Type_ _Var_ READ get##_FuncName_ WRITE set##_FuncName_ NOTIFY A_CHANGED_SIGNAL_FUNC(_Var_)) \
    _Type_ A_M_VAR(_Var_)

/** @brief Qt成员属性，主要针对QML中的自定义属性 */
#define A_DEFINE_MEMBER(_Type_, _Var_)                                              \
Q_PROPERTY(_Type_ _Var_ MEMBER A_M_VAR(_Var_) NOTIFY A_CHANGED_SIGNAL_FUNC(_Var_))  \
public:                                                                             \
Q_SIGNAL void A_CHANGED_SIGNAL_FUNC(_Var_)();                                       \
void _Var_(const _Type_& in_##_Var_)                                                \
{                                                                                   \
    if (A_M_VAR(_Var_) != in_##_Var_)                                               \
    {                                                                               \
        A_M_VAR(_Var_) = in_##_Var_;                                                \
        Q_EMIT A_CHANGED_SIGNAL_FUNC(_Var_)();                                      \
    }                                                                               \
}                                                                                   \
_Type_ _Var_() { return A_M_VAR(_Var_); }                                           \
private:                                                                            \
    _Type_ A_M_VAR(_Var_)

/** @brief Qt成员指针属性，主要针对QML中的自定义属性 */
#define A_DEFINE_MEMBER_PTR(_Type_, _Var_)                                          \
Q_PROPERTY(_Type_ _Var_ MEMBER A_M_VAR(_Var_) NOTIFY A_CHANGED_SIGNAL_FUNC(_Var_))  \
public:                                                                             \
Q_SIGNAL void A_CHANGED_SIGNAL_FUNC(_Var_)();                                       \
void _Var_(_Type_ in_##_Var_)                                                       \
{                                                                                   \
    if (A_M_VAR(_Var_) != in_##_Var_)                                               \
    {                                                                               \
        A_M_VAR(_Var_) = in_##_Var_;                                                \
        Q_EMIT A_CHANGED_SIGNAL_FUNC(_Var_)();                                      \
    }                                                                               \
}                                                                                   \
_Type_ _Var_() { return A_M_VAR(_Var_); }                                           \
private:                                                                            \
    _Type_ A_M_VAR(_Var_)

/** @brief Qt属性的声明和定义，不带修改信号  */
#define A_DEFINE_PROPERTY_NOSIGNAL(_Type_, _Var_, _FuncName_)                     \
public:                                                                           \
    inline _Type_ get##_FuncName_(void) const noexcept { return A_M_VAR(_Var_); } \
    inline void set##_FuncName_(const _Type_ &_arg) { A_M_VAR(_Var_) = _arg; }    \
                                                                                  \
private:                                                                          \
    Q_PROPERTY(_Type_ _Var_ READ get##_FuncName_ WRITE set##_FuncName_)           \
    _Type_ A_M_VAR(_Var_)

#define A_DECLARE_PRIVATE(Class)                         \
    friend class Class##Private;                         \
    Class##Private *aproch_d_ptr;                        \
    Class##Private &aproch_d() { return *aproch_d_ptr; } \
    const Class##Private &aproch_d() const { return *aproch_d_ptr; }

#define A_DECLARE_PUBLIC(Class)                               \
    friend class Class;                                       \
    Class *aproch_p_ptr;                                      \
    inline void setPublic(Class *ptr) { aproch_p_ptr = ptr; } \
    Class &aproch_p() { return *aproch_p_ptr; }               \
    const Class &aproch_p() const { return *aproch_p_ptr; }

#define A_DECLARE_EX_PRIVATE(Class) friend class Class##Private;                                            \
Class##Private& aproch_d() { return reinterpret_cast<Class##Private &>(*aproch_d_ptr); }                    \
const Class##Private& aproch_d() const { return reinterpret_cast<const Class##Private &>(*aproch_d_ptr); }

#define A_DECLARE_EX_PUBLIC(Class)                                              \
friend class Class;                                                             \
Class& aproch_p() { return static_cast<Class &>(*aproch_p_ptr); }               \
const Class& aproch_p() const { return static_cast<Class &>(*aproch_p_ptr); }

#define A_INIT_PRIVATE(Class)            \
    aproch_d_ptr = new Class##Private(); \
    aproch_d_ptr->setPublic(this)

#define A_D(Class) Class##Private &d = aproch_d()
#define A_P(Class) Class &p = aproch_p()
#define A_DELETE_PRIVATE() \
    delete aproch_d_ptr;   \
    aproch_d_ptr = nullptr

#define A_DELETE_AND_NULL(p) { delete p; p = nullptr; }
#define A_SAFE_DELETE(p) if (nullptr != p) { delete p; p = nullptr; }

#if !defined(QStringLiteral)
#define QStringLiteral(str) QString(QLatin1String(str))
#endif

#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)) && !defined(Q_QDOC)
class QStringSubset : public QStringView
{
public:
    QStringSubset(const QString* string, int position, int length)
        : QStringView(&string->constData()[position], length)
    {
    }
    QStringSubset(const QString* string)
        : QStringView(*string)
    {
    }
    QStringSubset()
        : QStringView()
    {
    }
    QStringSubset(const QStringView& s)
        : QStringView(s)
    {
    }
};
#else
#define QStringSubset QStringRef
#endif

/**
 * @brief 继承自QWidget的类如果不实现自己的paintEvent方法，那么该类就无法使用样式表
 * 改变样式。通过该宏可以在样式表中改变当前控件的样式
 * @note 该宏必须放在`paintEvent`函数中。
 */
#define APROCH_USE_STYLE_SHEET()                           \
    do                                                     \
    {                                                      \
        QStylePainter _Painter_(this);                     \
        QStyleOption _Opt_;                                \
        _Opt_.initFrom(this);                              \
        _Opt_.rect = rect();                               \
        _Painter_.drawPrimitive(QStyle::PE_Widget, _Opt_); \
    } while (0)

// 窗口缩放阈值
#define APROCH_RESIZE_BOUNDARY 8

/** * meta type defines */
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
Q_DECLARE_METATYPE(QMargins)
Q_DECLARE_METATYPE(QMarginsF)
#endif

template <class SizeValue>
static SizeValue qBoundSize(const SizeValue& minVal, const SizeValue& val, const SizeValue& maxVal)
{
    SizeValue croppedVal = val;
    if (minVal.width() > val.width())
        croppedVal.setWidth(minVal.width());
    else if (maxVal.width() < val.width())
        croppedVal.setWidth(maxVal.width());

    if (minVal.height() > val.height())
        croppedVal.setHeight(minVal.height());
    else if (maxVal.height() < val.height())
        croppedVal.setHeight(maxVal.height());

    return croppedVal;
}

template <class Value>
static Value qBoundV2D(const Value& minVal, const Value& val, const Value& maxVal)
{
    Value croppedVal = val;
    if (minVal.x() > val.x())
        croppedVal.setX(minVal.x());
    else if (maxVal.x() < val.x())
        croppedVal.setX(maxVal.x());

    if (minVal.y() > val.y())
        croppedVal.setY(minVal.y());
    else if (maxVal.y() < val.y())
        croppedVal.setY(maxVal.y());

    return croppedVal;
}

// Match the exact signature of qBound for VS 6.
APROCH_API QSize qBound(QSize minVal, QSize val, QSize maxVal);
APROCH_API QSizeF qBound(QSizeF minVal, QSizeF val, QSizeF maxVal);
APROCH_API QPoint qBound(QPoint minVal, QPoint val, QPoint maxVal);
APROCH_API QPointF qBound(QPointF minVal, QPointF val, QPointF maxVal);

// ----------------------------------------------------------------------------------------------------
// 以下定义全局数据结构和方法
// ----------------------------------------------------------------------------------------------------

APROCH_NAMESPACE_BEGIN

/** @brief 数据绑定类型 */
enum class EDataBindType
{
    /** @brief 不进行绑定 */
    None,

    /** @brief 单向绑定 */
    OneWay,

    /** @brief 单向绑定的反向情况 */
    OneWayRevise,

    /** @brief 双向绑定 */
    TwoWay,

    /** @brief 仅在初始时绑定一次。其他有效的绑定类型后续无法重新设置为该值，但该值的绑定类型可重新设置为其他值 */
    FirstTime,

    /** @brief 仅在初始时绑定一次，并且是反向绑定。其他有效的绑定类型后续无法重新设置为该值，但该值的绑定类型可重新设置为其他值 */
    FirstTimeRevise,
};

/**
 * @brief 根据类型名称该类型的默认值。数字类型返回都是0，布尔类型返回false，字符（串）类型返回为空字符（串），容器类型返回返回空容器
 * @param typeName 名称，不区分大小写
 * @return 给定类型的QVariant。无法识别的类型则返回空变量
 */
extern APROCH_API QVariant GetDefaultValueByType(const QString& typeName);

APROCH_NAMESPACE_END
