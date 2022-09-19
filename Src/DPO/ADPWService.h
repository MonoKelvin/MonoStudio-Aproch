/****************************************************************************
 * @file    ADPWService.h
 * @date    2022-07-06
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
#include "DPO/IDPOService.h"
#include <QtXml/QDomElement>

#define AServiceName_DPW "aproch.DPWService"

namespace aproch
{
    /**
     * @brief DPW数据
     */
    class APROCH_API ADPWData : public aproch::IDPOData
    {
    public:
        /**
         * @brief 获取属性对应的值
         * @param attrName 属性名称
         * @return 属性对应的值，没有属性返回空
         */
        QString value(const QString &attrName) const;

        /**
         * @brief 获取所有的属性
         * @param normalize 是否标准化，去除属性名称的前后空格(QString::trimmed)、全部转换为小写(QString::toLower)
         * @return 属性表
         * @see properties
         */
        QMap<QString, QString> attributes(bool normalize = false) const;

        /** @brief XML元素节点 */
        QDomElement domElement;

        /** @brief 键值名称 */
        static const QString key;
    };

    /**
     * @brief 动态解析控件（DPW, Dynamically Parse Widget）服务。支持创建最外层节点为布局 QLayout 对象
     * @note 解析方案为xml解析
     */
    class APROCH_API ADPWService : public aproch::IDPOService
    {
    public:
        ~ADPWService();

        /** @brief 对象创建器的字符分割符号 */
        static QString nameSplitterChar;

        virtual bool registerObjectCreator(const QSharedPointer<aproch::IObjectCreator> &creator) override;
        virtual bool unregisterObjectCreator(const QString &name) override;
        virtual QSharedPointer<aproch::IObjectCreator> getObjectCreator(const QString &name) const override;
        virtual QObject *prase(const QString &data, QObject *parent = nullptr) override;
        virtual QString serialize(QObject *object) override;

    protected:
        /**
         * @brief 根据XML节点创建对象
         * @param domElement XML节点
         * @param parent 父对象
         * @return 控件（或布局）对象
         */
        QObject *createObjectByDomElement(const QDomElement &domElement, QObject *parent);

    private:
        const QString _key(const QString &str) const;

    private:
        /** @brief 对象创建器表 */
        QMap<QString, QSharedPointer<aproch::IObjectCreator>> mCreatorMap;
    };
}
