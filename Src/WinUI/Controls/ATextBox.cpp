/****************************************************************************
 * @file    ATextBox.cpp
 * @date    2023-04-05 
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
#include "stdafx.h"
#include "ATextBox.h"

#include "private/qlineedit_p.h"
#include "private/qstylesheetstyle_p.h"

APROCH_NAMESPACE_BEGIN

class ATextBoxPrivate : public QLineEditPrivate
{
    Q_DECLARE_PUBLIC(ATextBox)
public:
    ATextBoxPrivate();
};

ATextBoxPrivate::ATextBoxPrivate()
{
}

// ----------------------------------------------------------------------------------------------------

void s(const char* name, const QVariant& val, QWidget* w)
{
    w->setProperty(name, val);
}

ATextBox::ATextBox(QWidget* parent)
    : QLineEdit(parent)
{
    setClearButtonEnabled(true);

    bindStyleObject(this);
}

ATextBox::ATextBox(const QString& content, QWidget* parent)
    : ATextBox(parent)
{
    setText(content);
}

ATextBox::~ATextBox()
{
}

QSize ATextBox::sizeHint() const
{
    return QSize(QLineEdit::sizeHint().width(), 40);
}

void ATextBox::paintEvent(QPaintEvent* event)
{
    return __super::paintEvent(event);
    Q_D(ATextBox);

    QPainter p(this);
    QPalette pal = palette();

    QStyleOptionFrame panel;
    initStyleOption(&panel);
    style()->drawPrimitive(QStyle::PE_PanelLineEdit, &panel, &p, this);
    QRect r = style()->subElementRect(QStyle::SE_LineEditContents, &panel, this);
    //r = r.marginsRemoved(d->effectiveTextMargins());
    p.setClipRect(r);

    const QMargins Margin = getStyleValue<QMargins>(ATextBox::Margins, TextBoxDefaultContentMargins);

    QFontMetrics fm = fontMetrics();
    Qt::Alignment va = QStyle::visualAlignment(d->control->layoutDirection(), QFlag(d->alignment));
    switch (va & Qt::AlignVertical_Mask)
    {
    case Qt::AlignBottom:
        d->vscroll = r.y() + r.height() - fm.height() - Margin.bottom();
        break;
    case Qt::AlignTop:
        d->vscroll = r.y() + Margin.top();
        break;
    default:
        //center
        d->vscroll = r.y() + (r.height() - fm.height() + 1) / 2;
        break;
    }
    QRect lineRect(r.x() + Margin.left(), d->vscroll, r.width() - Margin.right() - Margin.left(), fm.height());

    if (d->shouldShowPlaceholderText())
    {
        if (!d->placeholderText.isEmpty())
        {
            const Qt::LayoutDirection layoutDir = d->placeholderText.isRightToLeft() ? Qt::RightToLeft : Qt::LeftToRight;
            const Qt::Alignment alignPhText = QStyle::visualAlignment(layoutDir, QFlag(d->alignment));
            const QColor col = pal.placeholderText().color();
            QPen oldpen = p.pen();
            p.setPen(col);
            Qt::LayoutDirection oldLayoutDir = p.layoutDirection();
            p.setLayoutDirection(layoutDir);

            const QString elidedText = fm.elidedText(d->placeholderText, Qt::ElideRight, lineRect.width());
            p.drawText(lineRect, alignPhText, elidedText);
            p.setPen(oldpen);
            p.setLayoutDirection(oldLayoutDir);
        }
    }

    int cix = qRound(d->control->cursorToX());

    // horizontal scrolling. d->hscroll is the left indent from the beginning
    // of the text line to the left edge of lineRect. we update this value
    // depending on the delta from the last paint event; in effect this means
    // the below code handles all scrolling based on the textline (widthUsed),
    // the line edit rect (lineRect) and the cursor position (cix).
    int widthUsed = qRound(d->control->naturalTextWidth()) + 1;
    if (widthUsed <= lineRect.width())
    {
        // text fits in lineRect; use hscroll for alignment
        switch (va & ~(Qt::AlignAbsolute | Qt::AlignVertical_Mask))
        {
        case Qt::AlignRight:
        d->hscroll = widthUsed - lineRect.width() + 1;
        break;
        case Qt::AlignHCenter:
        d->hscroll = (widthUsed - lineRect.width()) / 2;
        break;
        default:
        // Left
        d->hscroll = 0;
        break;
        }
    }
    else if (cix - d->hscroll >= lineRect.width())
    {
        // text doesn't fit, cursor is to the right of lineRect (scroll right)
        d->hscroll = cix - lineRect.width() + 1;
    }
    else if (cix - d->hscroll < 0 && d->hscroll < widthUsed)
    {
        // text doesn't fit, cursor is to the left of lineRect (scroll left)
        d->hscroll = cix;
    }
    else if (widthUsed - d->hscroll < lineRect.width())
    {
        // text doesn't fit, text document is to the left of lineRect; align
        // right
        d->hscroll = widthUsed - lineRect.width() + 1;
    }
    else
    {
        //in case the text is bigger than the lineedit, the hscroll can never be negative
        d->hscroll = qMax(0, d->hscroll);
    }

    // the y offset is there to keep the baseline constant in case we have script changes in the text.
    // Needs to be kept in sync with ATextBoxPrivate::adjustedControlRect
    QPoint topLeft = lineRect.topLeft() - QPoint(d->hscroll, d->control->ascent() - fm.ascent());

    // draw text, selections and cursors
//#ifndef QT_NO_STYLE_STYLESHEET
//    if (QStyleSheetStyle* cssStyle = qt_styleSheet(style()))
//    {
//        cssStyle->styleSheetPalette(this, &panel, &pal);
//    }
//#endif
    p.setPen(pal.text().color());

    int flags = QWidgetLineControl::DrawText;

#ifdef QT_KEYPAD_NAVIGATION
    if (!QApplicationPrivate::keypadNavigationEnabled() || hasEditFocus())
#endif
        if (d->control->hasSelectedText() || (d->cursorVisible && !d->control->inputMask().isEmpty() && !d->control->isReadOnly()))
        {
            flags |= QWidgetLineControl::DrawSelections;
            // Palette only used for selections/mask and may not be in sync
            if (d->control->palette() != pal
                || d->control->palette().currentColorGroup() != pal.currentColorGroup())
                d->control->setPalette(pal);
        }

    // Asian users see an IM selection text as cursor on candidate
    // selection phase of input method, so the ordinary cursor should be
    // invisible if we have a preedit string.
    if (d->cursorVisible && !d->control->isReadOnly())
        flags |= QWidgetLineControl::DrawCursor;

    d->control->setCursorWidth(style()->pixelMetric(QStyle::PM_TextCursorWidth, &panel));
    d->control->draw(&p, topLeft, r, flags);
}

APROCH_NAMESPACE_END
