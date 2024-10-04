#include "stdafx.h"
#include "AIconButton.h"
#include "Style/AUIStyleObject.h"

#include <QApplication>
#include <QScreen>

APROCH_NAMESPACE_BEGIN

class AIconButtonPrivate
{
public:
    QString glyph;
};

AIconButton::AIconButton(QWidget* parent)
    : AIconButton(QIcon(), parent)
{
}

AIconButton::AIconButton(const QString& glyph, QWidget* parent)
    : AIconButton(AFontIcon::icon(glyph), parent)
{
    d_ptr->glyph = glyph;
}

AIconButton::AIconButton(const QIcon& icon, QWidget* parent)
    : AButton(QString(), parent)
    , d_ptr(new AIconButtonPrivate)
{
    setFixedSize(AFontIcon::DefaultIconSize.width() + AprochDefaultPaddings.left(),
                 AFontIcon::DefaultIconSize.height() + AprochDefaultPaddings.top());
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setAttribute(Qt::WA_StyledBackground);
    setFont(AFontDatabase::getDefaultIconFont());
    setIcon(icon);
}

AIconButton::~AIconButton()
{
}

QSize AIconButton::sizeHint() const
{
    return QSize(AFontIcon::DefaultIconSize.width() + AprochDefaultPaddings.left(),
                 AFontIcon::DefaultIconSize.height() + AprochDefaultPaddings.top());
}

void AIconButton::paintEvent(QPaintEvent* e)
{
    QColor currentColor = getCurrentTextColor();
    if (!isEnabled())
        currentColor = isChecked() ? currentColor.darker(120) : getDisabledColor();

    QStyleOptionButton styleOption;
    initStyleOption(&styleOption);

    auto r = styleOption.rect;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    /** draw background */
    drawBackground(&painter);

    /** draw icon */
    QSize isize = styleOption.iconSize;
    QRect iconRect((r.width() - isize.width()) / 2, (r.height() - isize.height()) / 2,
                   isize.width(), isize.height());

    if (!d_ptr->glyph.isEmpty())
    {
        /*ic = AFontIcon::icon(d_ptr->glyph, AFontDatabase::getDefaultIconFont(),
                             getCurrentTextColor(), isize);*/
        qreal screenRatio = QApplication::primaryScreen()->devicePixelRatio();
        QImage iconImg(isize * screenRatio, QImage::Format_ARGB32);
        iconImg.fill(Qt::transparent);
        iconImg.setDevicePixelRatio(screenRatio);

        QTextOption textOp;
        textOp.setAlignment(Qt::AlignCenter);

        painter.setPen(currentColor);
        painter.setRenderHint(QPainter::SmoothPixmapTransform);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        painter.setRenderHint(QPainter::VerticalSubpixelPositioning);
#endif
        painter.setRenderHint(QPainter::LosslessImageRendering);
        painter.drawText(iconRect, d_ptr->glyph, textOp);
    }
    else
    {
        QIcon::Mode imode = QIcon::Normal;
        if (styleOption.state & QStyle::State_Active)
            imode = QIcon::Active;
        else if (styleOption.state & QStyle::State_Selected)
            imode = QIcon::Selected;
        else if (!this->isEnabled())
            imode = QIcon::Disabled;

        QIcon::State istate = (styleOption.state & QStyle::State_On) ? QIcon::On : QIcon::Off;
        painter.drawPixmap(iconRect, styleOption.icon.pixmap(isize, imode, istate));
    }
}

APROCH_NAMESPACE_END
