#pragma once
#include "AprochExportDefine.h"
#include <QSplitterHandle>

APROCH_NAMESPACE_BEGIN

class APROCH_API ASplitterHandle : public QSplitterHandle
{
    Q_OBJECT;
    A_DEFINE_PROPERTY_NOSIGNAL(QColor, barColor, BarColor);
public:
    explicit ASplitterHandle(QSplitter* parent = nullptr);
    explicit ASplitterHandle(Qt::Orientation o, QSplitter* parent = nullptr);
    ~ASplitterHandle();

    void moveSplitterEx(int pos);

    virtual QSize sizeHint() const override;

protected:
    virtual void paintEvent(QPaintEvent* evt) override;

private:
    Q_DISABLE_COPY_MOVE(ASplitterHandle);
};

APROCH_NAMESPACE_END