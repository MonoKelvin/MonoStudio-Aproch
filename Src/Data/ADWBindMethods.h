#pragma once

APROCH_NAMESPACE_BEGIN

class APROCH_API ADWBM_SpinBox : public ADataWidgetBindMethod
{
    Q_OBJECT
public:
    ADWBM_SpinBox(QObject* parent = nullptr);
    ~ADWBM_SpinBox();

    virtual bool bind(const ADWBindParameter& param) override;
    virtual bool unbind(AData* data, QWidget* widget, const QString& propName = QString()) override;
    virtual void onValueChanged(const AData* data, QWidget* widget, const QString& propertyName, const QVariant& old) override;
    virtual void onWidgetValueChanged(AData* data, const QWidget* widget, const QString& propertyName) override;

protected Q_SLOTS:
    void spinboxValueChanged(int);
};

APROCH_NAMESPACE_END
