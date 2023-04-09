/****************************************************************************
 * @file    AAbstractEditorFactory.h
 * @date    2022-7-16
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

#include <QSet>

namespace aproch
{
    /**
     * @brief 用于创建数据编辑控件的抽象工厂基类
     */
    class AAbstractEditorFactoryBase : public QObject
    {
        Q_OBJECT
    public:
        /** @brief 创建绑定给定数据的控件 */
        virtual QWidget* createEditor(AData* data, QWidget* parent) = 0;

    protected:
        explicit AAbstractEditorFactoryBase(QObject* parent = 0)
            : QObject(parent)
        {
        }

        /** @brief 断开和创建的控件关联的信号 */
        virtual void breakConnection(AAbstractDataManager* manager) = 0;

    protected Q_SLOTS:
        virtual void managerDestroyed(QObject* manager) = 0;
    };

    /**
     * @brief 用于创建数据编辑控件的抽象工厂类
     */
    template <class DataManager>
    class AAbstractEditorFactory : public AAbstractEditorFactoryBase
    {
    public:
        explicit AAbstractEditorFactory(QObject* parent)
            : AAbstractEditorFactoryBase(parent)
        {
        }

        QWidget* createEditor(AData* data, QWidget* parent)
        {
            for (DataManager* manager : qAsConst(m_managers))
            {
                if (manager == data->dataManager())
                {
                    return createEditorImpl(manager, data, parent);
                }
            }
            return nullptr;
        }

        void addDataManager(DataManager* manager)
        {
            if (m_managers.contains(manager))
                return;
            m_managers.insert(manager);
            connectDataManager(manager);
            connect(manager, SIGNAL(destroyed(QObject*)), this, SLOT(managerDestroyed(QObject*)));
        }

        void removeDataManager(DataManager* manager)
        {
            if (!m_managers.contains(manager))
                return;
            disconnect(manager, SIGNAL(destroyed(QObject*)), this, SLOT(managerDestroyed(QObject*)));
            disconnectDataManager(manager);
            m_managers.remove(manager);
        }

        QSet<DataManager*> dataManagers() const
        {
            return m_managers;
        }

        DataManager* dataManager(AData* data) const
        {
            AAbstractDataManager* manager = data->dataManager();
            for (DataManager* m : qAsConst(m_managers))
            {
                if (m == manager)
                {
                    return m;
                }
            }
            return nullptr;
        }

    protected:
        virtual void connectDataManager(DataManager* manager) = 0;
        virtual QWidget* createEditorImpl(DataManager* manager, AData* data, QWidget* parent) = 0;
        virtual void disconnectDataManager(DataManager* manager) = 0;
        void managerDestroyed(QObject* manager)
        {
            for (DataManager* m : qAsConst(m_managers))
            {
                if (m == manager)
                {
                    m_managers.remove(m);
                    return;
                }
            }
        }

    private:
        void breakConnection(AAbstractDataManager* manager)
        {
            for (DataManager* m : qAsConst(m_managers))
            {
                if (m == manager)
                {
                    removeDataManager(m);
                    return;
                }
            }
        }

    private:
        QSet<DataManager*> m_managers;
    };
}
