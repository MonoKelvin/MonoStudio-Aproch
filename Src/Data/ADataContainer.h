#pragma once

APROCH_NAMESPACE_BEGIN

class AAbstractDataManager;
class ADataContainerPrivate;

/**
 * @brief 数据容器
 */
class APROCH_API ADataContainer : public QObject
{
    Q_OBJECT
    friend class AAbstractDataManager;
public:
    explicit ADataContainer(QObject* parent = nullptr);
    ~ADataContainer();

    /** @brief 添加数据管理器 */
    bool addManager(AAbstractDataManager* manager, EMetaType type);

    template<class T>
    Q_ALWAYS_INLINE T* getManager(EMetaType type) const
    {
        return qobject_cast<T*>(getManager(type));
    }
    AAbstractDataManager* getManager(EMetaType type) const;

    QVariant getDefaultValue(AData* dt) const;
    bool setDefaultValue(AData* dt, const QVariant& defaultVal);
    void resetValues(const ADataSet& dataSet);

    /** @brief 获取数据集，如果管理器器对应数据集不存在则创建 */
    ADataSet* getDataSet(AAbstractDataManager* manager, bool isCreateIfNull = true);
    ADataSet getAllData() const;

    /** @brief 添加指定类型的数据 */
    AData* addData(EMetaType type, const QString& name = QString());

    /** @brief 克隆数据，同时克隆所有子数据 */
    AData* cloneData(AData* srcData);

    /** @brief 删除数据集中的数据 */
    void deleteData(ADataSet& dataset);

Q_SIGNALS:
    void dataInserted(AData* data, AData* parent, AData* after);
    void dataChanged(AData* data);
    void dataRemoved(AData* data, AData* parent);
    void dataDestroyed(AData* data);
    void defaultValueChanged(AData* data, const QVariant& oldValue);
    void valueChanged(AData* data, const QVariant& oldValue);

public Q_SLOTS:
    void resetValue(AData* dt);
    void deleteData(AData*& dt);

private:
    /** @brief 添加已有的数据 */
    bool addData(AAbstractDataManager* manager, AData* dt, const QVariant& defaultVal);

private:
    Q_DISABLE_COPY(ADataContainer);
    Q_DECLARE_PRIVATE(ADataContainer);
};

APROCH_NAMESPACE_END