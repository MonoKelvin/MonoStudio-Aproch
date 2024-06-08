#include "stdafx.h"
#include "../Controls/ATextBox.h"
#include "AWinUIStyleObjects.h"
#include "../AWinUIStyleEngine.h"

APROCH_NAMESPACE_BEGIN

// ----------------------------------------------------------------------------------------------------

const SCornerF FrameDefaultBorderRadius{ 8 };
const SCornerF TextBoxDefaultBorderRadius = FrameDefaultBorderRadius;

// ----------------------------------------------------------------------------------------------------

void AWinUIFrameStyleObject::initProperties(QWidget* widget)
{
    AUIStyleObject::initProperties(widget);

    addStyle(BorderRadius, FrameDefaultBorderRadius);
}

void AWinUIFrameStyleObject::applyStyle(QWidget* widget)
{
    Q_ASSERT_X(nullptr != widget, Q_FUNC_INFO, "widget is null");

#ifdef QT_DEBUG
    qDebug() << widget->metaObject()->className();
#endif // QT_DEBUG

    QPalette& palette = AWinUIStyleEngine::instance()->getPalette(widget->metaObject()->className());
    widget->setPalette(palette);
}

// ----------------------------------------------------------------------------------------------------

void ATextBoxStyleObject::initProperties(QWidget* widget)
{
    AWinUIFrameStyleObject::initProperties(widget);

    addStyle(BorderRadius, TextBoxDefaultBorderRadius);

    applyStyle(widget);
}

APROCH_NAMESPACE_END
