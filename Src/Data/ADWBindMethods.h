#pragma once

APROCH_NAMESPACE_BEGIN

class APROCH_API ASpinBoxBindMethod : public ADataWidgetBindMethod
{
    Q_OBJECT
public:
    ASpinBoxBindMethod(QObject* parent = nullptr);
    ~ASpinBoxBindMethod();

    virtual bool bind(const ADWBindParameter& param) override;
    virtual bool unbind(AData* data, QWidget* widget, const QString& propName = QString()) override;
    virtual void onValueChanged(const AData* data, QWidget* widget, const QString& propertyName, const QVariant& old) override;
    virtual void onWidgetValueChanged(AData* data, const QWidget* widget, const QString& propertyName) override;

protected Q_SLOTS:
    void spinboxValueChanged(int);
};

class APROCH_API ADoubleSpinBoxBindMethod : public ADataWidgetBindMethod
{
    Q_OBJECT
public:
    ADoubleSpinBoxBindMethod(QObject* parent = nullptr);
    ~ADoubleSpinBoxBindMethod();

    virtual bool bind(const ADWBindParameter& param) override;
    virtual bool unbind(AData* data, QWidget* widget, const QString& propName = QString()) override;
    virtual void onValueChanged(const AData* data, QWidget* widget, const QString& propertyName, const QVariant& old) override;
    virtual void onWidgetValueChanged(AData* data, const QWidget* widget, const QString& propertyName) override;

protected Q_SLOTS:
    void spinboxValueChanged(double);
};

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

class APROCH_API ACheckBoxBindMethod : public ADataWidgetBindMethod
{
    Q_OBJECT
public:
    ACheckBoxBindMethod(QObject* parent = nullptr);
    ~ACheckBoxBindMethod();

    virtual bool bind(const ADWBindParameter& param) override;
    virtual bool unbind(AData* data, QWidget* widget, const QString& propName = QString()) override;
    virtual void onValueChanged(const AData* data, QWidget* widget, const QString& propertyName, const QVariant& old) override;
    virtual void onWidgetValueChanged(AData* data, const QWidget* widget, const QString& propertyName) override;

protected Q_SLOTS:
    void checkBoxValueChanged(int);
};

class APROCH_API ARadioButtonBindMethod : public ADataWidgetBindMethod
{
    Q_OBJECT
public:
    ARadioButtonBindMethod(QObject* parent = nullptr);
    ~ARadioButtonBindMethod();

    virtual bool bind(const ADWBindParameter& param) override;
    virtual bool unbind(AData* data, QWidget* widget, const QString& propName = QString()) override;
    virtual void onValueChanged(const AData* data, QWidget* widget, const QString& propertyName, const QVariant& old) override;
    virtual void onWidgetValueChanged(AData* data, const QWidget* widget, const QString& propertyName) override;

protected Q_SLOTS:
    void radioButtonValueChanged(bool);
};

class APROCH_API AComboBoxBindMethod : public ADataWidgetBindMethod
{
    Q_OBJECT
public:
    AComboBoxBindMethod(QObject* parent = nullptr);
    ~AComboBoxBindMethod();

    /** @brief Supported property names */
    static const QString Items;             // "items"
    static const QString CurrentIndex;      // "currentIndex"
    static const QString CurrentText;       // "currentText"

    virtual bool checkBind(const ADWBindParameter& param) const;
    virtual bool bind(const ADWBindParameter& param) override;
    virtual bool unbind(AData* data, QWidget* widget, const QString& propName = QString()) override;
    virtual void onValueChanged(const AData* data, QWidget* widget, const QString& propertyName, const QVariant& old) override;
    virtual void onWidgetValueChanged(AData* data, const QWidget* widget, const QString& propertyName) override;

protected Q_SLOTS:
    void comboBoxValueChanged(const QString&);
    void comboBoxValueChanged(int);
};


APROCH_NAMESPACE_END
