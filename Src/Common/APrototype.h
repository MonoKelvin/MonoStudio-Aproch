/****************************************************************************
 * @file    APrototypeT.h
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

/** @brief 继承自原型对象<APrototypeT>的子类需要添加该宏以创建克隆原型，不可克隆的原型不需要添加 */
#define APROCH_PROTOTYPE(_ClassName_) \
virtual aproch::APrototype* clone(const aproch::FCopyOptions& op = aproch::DeepCopy) const \
{ return new _ClassName_(*this, op); } \

/** @brief 默认拷贝选项 */
#ifdef A_DEFAULT_COPY_WITH_ID
#define A_Default_Copy_Option (aproch::ECopyOption::DeepCopyWithID)
#else
#define A_Default_Copy_Option (aproch::ECopyOption::DeepCopy)
#endif

namespace aproch
{
    /**
     * @brief 拷贝选项
     */
    enum ECopyOption
    {
        IDCopy = 0x0001,                        // 仅拷贝ID
        ShallowCopy = 0x0002,                   // 浅拷贝，但不拷贝ID
        ShallowCopyWithID = 0x0004 ^ IDCopy,    // 浅拷贝，但不拷贝ID
        DeepCopy = 0xFFFF ^ IDCopy,             // 深度拷贝，但不拷贝ID
        DeepCopyWithID = 0xFFFF,                // 深度拷贝，同时拷贝ID
    };
    Q_DECLARE_FLAGS(FCopyOptions, ECopyOption)
    Q_DECLARE_OPERATORS_FOR_FLAGS(FCopyOptions)

    /**
     * @brief 原型对象基类。提供基本的原型构造（拷贝）、唯一标识、属性配置等功能
     * @note 在所有的拷贝中，都无法拷贝类中继承自QObject的字段，这是Qt对象机制决定的。同时子类不允许是抽象类。一般要拷贝的对象如果指定了父对象，
     * 则拷贝出的新对象的父对象也会被指定到该同一个父对象
     */
    template<bool IsClonable>
    class APrototypeT
    {
        Q_DISABLE_COPY_MOVE(APrototypeT)
    public:
        APrototypeT()
            : mID(aproch::AID::Identity())
        {
            mAttributeMap.insert(AStr("id"), mID.toString());
        }

        APrototypeT(const APrototypeT& rhs, const FCopyOptions& op/* = A_Default_Copy_Option*/)
        {
            if (op & ECopyOption::IDCopy)
            {
                mID = rhs.mID;
            }
            else
            {
                mID = aproch::AID::Identity();
            }

            mAttributeMap = rhs.mAttributeMap;
            mAttributeMap[AStr("id")] = mID.toString();
        }
        virtual ~APrototypeT() = default;

        /**
         * @brief 克隆对象
         * @param op 克隆选项
         * @return 新对象
         */
        virtual APrototypeT* clone(const FCopyOptions& op = A_Default_Copy_Option) const
        {
            if constexpr (IsClonable)
            {
                return new APrototypeT(*this, op);
            }
            else
            {
                throw std::exception("the prototype is discloneable.");
            }
            return nullptr;
        }

        /**
         * @brief 获取对象唯一标识
         * @return 对象唯一标识
         */
        inline const aproch::AID getID() const noexcept
        {
            return mID;
        }

        /**
         * @brief 获取属性表
         * @return 属性表
         */
        inline QVariantMap getAttributeMap() const noexcept
        {
            return mAttributeMap;
        }

        /**
         * @brief 获取属性表（引用）
         * @return 属性表（引用）
         */
        inline QVariantMap& getAttributeMap() noexcept
        {
            return mAttributeMap;
        }

        /**
         * @brief 设置属性表
         * @param attributeMap 属性表
         */
        inline void setAttributeMap(const QVariantMap& attributeMap)
        {
            mAttributeMap = attributeMap;
        }

        /**
         * @brief 添加属性。如果属性已经存在，则会覆盖源属性
         * @param attrName 属性名
         * @param attrValue 属性值
         */
        void addAttribute(const QString& attrName, const QVariant& attrValue)
        {
            mAttributeMap[attrName] = attrValue;
        }

        /**
         * @brief 添加属性表。如果属性已存在，则会覆盖源属性
         * @param rhs 属性表
         */
        void addAttribute(const QVariantMap& rhs)
        {
            const auto& keys = rhs.keys();
            for (const auto& k : keys)
            {
                mAttributeMap[k] = rhs.value(k);
            }
        }

        /**
         * @brief 获取属性值
         * @param key 键
         * @return 属性值
         */
        QVariant getAttribute(const QString& key) const
        {
            auto iter = mAttributeMap.find(key);
            if (mAttributeMap.cend() == iter)
                return QVariant();
            return iter.value();
        }

        /**
         * @brief 添加属性
         * @param attrName 属性名
         * @return 是否移除成功
         */
        bool removeAttribute(const QString& attrName)
        {
            return 0 == mAttributeMap.remove(attrName);
        }

        /**
         * @brief 改变属性
         * @param attrName 属性名
         * @param newAttrValue 新的属性值
         * @return 是否更改成功
         */
        bool changeAttribute(const QString& attrName, const QVariant& newAttrValue)
        {
            auto iter = mAttributeMap.find(attrName);
            if (iter != mAttributeMap.end())
            {
                iter->setValue(newAttrValue);
                return true;
            }

            return false;
        }

    private:
        /** @brief 对象唯一标识 */
        aproch::AID mID;

        /** @brief 属性表 */
        QVariantMap mAttributeMap;
    };

    using APrototype = APrototypeT<true>;
    using APrototypeDisclonable = APrototypeT<false>;
}
