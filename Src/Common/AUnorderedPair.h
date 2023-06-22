/****************************************************************************
 * @file    AUnorderedPair.h
 * @date    2023-06-22
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

#ifndef AUNORDEREDPAIR_H
#define AUNORDEREDPAIR_H

#include "AprochGlobal.h"

#include <qpair.h>

APROCH_NAMESPACE_BEGIN

/**
 * @brief 无序数据对
 * @see QPair
 */
template <class T>
struct AUnorderedPair
{
    typedef T value_type;

    Q_DECL_CONSTEXPR AUnorderedPair()
        noexcept((std::is_nothrow_default_constructible<T>::value))
        : first(), second()
    {
    }

    Q_DECL_CONSTEXPR AUnorderedPair(const T& t1, const T& t2)
        noexcept((std::is_nothrow_copy_constructible<T>::value))
        : first(t1), second(t2)
    {
    }

    // compiler-generated copy/move ctor/assignment operators are fine!

    template <typename TT>
    Q_DECL_CONSTEXPR AUnorderedPair(const AUnorderedPair<TT>& p)
        noexcept((std::is_nothrow_constructible<T>::value))
        : first(p.first), second(p.second)
    {
    }

    template <typename TT>
    Q_DECL_RELAXED_CONSTEXPR AUnorderedPair& operator=(const AUnorderedPair<TT>& p)
        noexcept((std::is_nothrow_assignable<T, TT&>::value))
    {
        first = p.first; second = p.second; return *this;
    }

    template <typename TT>
    Q_DECL_CONSTEXPR AUnorderedPair(AUnorderedPair<TT>&& p)
        noexcept((std::is_nothrow_constructible<T, TT>::value))
        // can't use std::move here as it's not constexpr in C++11:
        : first(static_cast<TT&&>(p.first)), second(static_cast<TT&&>(p.second))
    {
    }

    template <typename TT>
    Q_DECL_RELAXED_CONSTEXPR AUnorderedPair& operator=(AUnorderedPair<TT>&& p)
        noexcept((std::is_nothrow_assignable<T, TT>::value))
    {
        first = std::move(p.first); second = std::move(p.second); return *this;
    }

    Q_DECL_RELAXED_CONSTEXPR void swap(AUnorderedPair& other)
        noexcept(noexcept(qSwap(other.first, other.first)) && noexcept(qSwap(other.second, other.second)))
    {
        // use qSwap() to pick up ADL swaps automatically:
        qSwap(first, other.first);
        qSwap(second, other.second);
    }

    Q_DECL_RELAXED_CONSTEXPR QPair<T, T> toQPair()
        noexcept(noexcept(QPair<T>(first, second)))
    {
        return QPair<T, T>(first, second);
    }

    static Q_DECL_RELAXED_CONSTEXPR AUnorderedPair<T> fromQPair(const QPair<T, T>& p)
        noexcept(noexcept(AUnorderedPair<T>(first, second)))
    {
        return AUnorderedPair<T>(p.first, p.second);
    }

    T first;
    T second;
};

#if defined(__cpp_deduction_guides) && __cpp_deduction_guides >= 201606
template<class T>
AUnorderedPair(T) -> AUnorderedPair<T>;
#endif

template <typename T>
void swap(AUnorderedPair<T>& lhs, AUnorderedPair<T>& rhs) noexcept(noexcept(lhs.swap(rhs)))
{
    lhs.swap(rhs);
}

// mark AUnorderedPair<T> as complex/movable/primitive depending on the
// typeinfos of the constituents:
template<class T>
class QTypeInfo<AUnorderedPair<T> > : public QTypeInfoMerger<AUnorderedPair<T>, T>
{
}; // Q_DECLARE_TYPEINFO

template <class T>
Q_DECL_CONSTEXPR Q_INLINE_TEMPLATE bool operator==(const AUnorderedPair<T>& p1, const AUnorderedPair<T>& p2)
noexcept(noexcept(p1.first == p2.first && p1.second == p2.second))
{
    return (p1.first == p2.first && p1.second == p2.second) || 
        (p1.first == p2.second && p1.second == p2.first);
}

template <class T>
Q_DECL_CONSTEXPR Q_INLINE_TEMPLATE bool operator!=(const AUnorderedPair<T>& p1, const AUnorderedPair<T>& p2)
noexcept(noexcept(!(p1 == p2)))
{
    return !(p1 == p2);
}

template <class T>
Q_DECL_CONSTEXPR Q_OUTOFLINE_TEMPLATE AUnorderedPair<T> qMakePair(const T& x, const T& y)
noexcept(noexcept(AUnorderedPair<T>(x, y)))
{
    return AUnorderedPair<T>(x, y);
}

APROCH_NAMESPACE_END

#endif // AUNORDEREDPAIR_H
