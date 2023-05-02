#pragma once

APROCH_NAMESPACE_BEGIN

class APROCH_API ADWBM_SpinBox : public ADataWidgetBindMethod
{
public:
    virtual ~ADWBM_SpinBox();

    /** @brief 绑定 */
    virtual bool bind(AData* data, QWidget* widget, const SBindParameter& parameter) override;

    /** @brief 解绑 */
    virtual bool unbind(AData* data, QWidget* widget, const char* propName) override;

    /** @brief 数据修改更新控件的方法 */
    virtual void onValueChanged(const AData* data, QWidget* widget, const QVariant& old) override;

    /** @brief 控件修改更新数据的方法 */
    virtual void onWidgetValueChanged(AData* data, const QWidget* widget) override;
};

APROCH_NAMESPACE_END
