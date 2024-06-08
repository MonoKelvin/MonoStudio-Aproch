/****************************************************************************
 * @file    APrototype.h
 * @date    2021-8-12
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
#include <QMap>
#include <QString>
#include "AIdentity.h"

/** @brief 默认拷贝选项 */
#ifdef A_DEFAULT_COPY_WITHOUT_ID
#define A_DEFAULT_COPY_OPTION (APROCH_PRE_NAMESPACE(ECopyOption::DeepCopy))
#else
#define A_DEFAULT_COPY_OPTION (APROCH_PRE_NAMESPACE(ECopyOption::DeepCopyWithID))
#endif

/** @brief 继承自原型对象<APrototype>的子类需要添加该宏以创建克隆原型，不可克隆的原型不需要添加 */
#define APROCH_PROTOTYPE(_ClassName_)                                                                        \
public:                                                                                                      \
    virtual APrototype *clone(const FCopyOptions &op = A_DEFAULT_COPY_OPTION) const override                 \
    {                                                                                                        \
        return new _ClassName_(*this, op);                                                                   \
    }                                                                                                        \
private:                                                                                                     \
    Q_DISABLE_MOVE(_ClassName_)                                                                              \
    friend APrototype

/** @brief 继承自原型对象<APrototype>的子类需要添加该宏以创建私有克隆原型，不可克隆的原型不需要添加 */
#define APROCH_PROTOTYPE_PRIVATE(_ClassName_)                                                                \
private:                                                                                                     \
    virtual APrototype *clone(const FCopyOptions &op = A_DEFAULT_COPY_OPTION) const override                 \
    {                                                                                                        \
        return new _ClassName_(*this, op);                                                                   \
    }                                                                                                        \
    Q_DISABLE_MOVE(_ClassName_)                                                                              \
    friend APrototype

APROCH_NAMESPACE_BEGIN

/**
 * @brief 拷贝选项
 */
enum ECopyOption
{
    /** @brief 仅拷贝ID */
    IDCopy = 0x0001,

    /** @brief 浅拷贝，但不拷贝ID */
    ShallowCopy = 0x0002,

    /** @brief 浅拷贝，同时拷贝ID */
    ShallowCopyWithID = 0x0004 ^ IDCopy,

    /** @brief 保留父级关系 */
    KeepParents = 0x0008,

    /** @brief 同时拷贝所有子集节点 */
    CopyWithChildren = 0x0010,

    /** @brief 深度拷贝，但不拷贝ID */
    DeepCopy = 0xFFFF ^ IDCopy,

    /** @brief 深度拷贝，同时拷贝ID */
    DeepCopyWithID = 0xFFFF,
};
Q_DECLARE_FLAGS(FCopyOptions, ECopyOption)
Q_DECLARE_OPERATORS_FOR_FLAGS(FCopyOptions)

/**
 * @brief 原型对象基类
 */
class APrototype
{
    // 禁止移动拷贝
    APrototype(APrototype&&) = delete;
    APrototype& operator=(APrototype&&) = delete;
public:
    APrototype()
        : m_id(AID::Identity())
    {
    }

    APrototype(const APrototype &rhs, const FCopyOptions &op = A_DEFAULT_COPY_OPTION)
    {
        if (op & ECopyOption::IDCopy)
        {
            m_id = rhs.m_id;
        }
        else
        {
            m_id = AID::Identity();
        }
    }
    virtual ~APrototype() = default;

    /**
     * @brief 克隆对象
     * @param op 克隆选项
     * @return 新对象
     */
    virtual APrototype *clone(const FCopyOptions &op = A_DEFAULT_COPY_OPTION) const
    {
        return new APrototype(*this, op);
    }

    /** @brief 是否有效 */
    virtual bool isValid() const
    {
        return true;
    }

    /**
     * @brief 获取对象唯一标识
     * @return 对象唯一标识
     */
    inline const AID getId() const noexcept
    {
        return m_id;
    }

private:
    /** @brief 对象唯一标识 */
    AID m_id;
};

APROCH_NAMESPACE_END
