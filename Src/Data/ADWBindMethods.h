#pragma once

APROCH_NAMESPACE_BEGIN

class APROCH_API ASpinbBoxBindMethod : public ADataWidgetBindMethod
{
    Q_OBJECT
public:
    ASpinbBoxBindMethod(QObject* parent = nullptr);
    ~ASpinbBoxBindMethod();

    virtual bool bind(const ADWBindParameter& param) override;
    virtual bool unbind(AData* data, QWidget* widget, const QString& propName = QString()) override;
    virtual void onValueChanged(const AData* data, QWidget* widget, const QString& propertyName, const QVariant& old) override;
    virtual void onWidgetValueChanged(AData* data, const QWidget* widget, const QString& propertyName) override;

protected Q_SLOTS:
    void spinboxValueChanged(int);
};

// ----------------------------------------------------------------------------------------------------

class APROCH_API ALineEditBindMethod : public ADataWidgetBindMethod
{
    Q_OBJECT
public:
    ALineEditBindMethod(QObject* parent = nullptr);
    ~ALineEditBindMethod();

    virtual bool bind(const ADWBindParameter& param) override;
    virtual bool unbind(AData* data, QWidget* widget, const QString& propName = QString()) override;
    virtual void onValueChanged(const AData* data, QWidget* widget, const QString& propertyName, const QVariant& old) override;
    virtual void onWidgetValueChanged(AData* data, const QWidget* widget, const QString& propertyName) override;

protected Q_SLOTS:
    void lineEditValueChanged(const QString&);
};

APROCH_NAMESPACE_END
