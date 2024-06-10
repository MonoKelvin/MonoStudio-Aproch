#ifndef AWINUIAPPLICATION_H
#define AWINUIAPPLICATION_H

#include <QtQuick/QQuickPaintedItem>

class AWinUIApplication : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_DISABLE_COPY(AWinUIApplication)
public:
    explicit AWinUIApplication(QQuickItem *parent = nullptr);
    ~AWinUIApplication() override;
};

#endif // AWINUIAPPLICATION_H
