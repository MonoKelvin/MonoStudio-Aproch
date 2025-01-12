/****************************************************************************
 * @file    AFontComboBox.cpp
 * @date    2025-01-11 
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
#include "AFontSizeComboBox.h"
#include "Widget/Private/AComboBox_p.h"
#include <QApplication>
#include <QListView>
#include <QStringListModel>
#include <QAccessible>
#include <QStyledItemDelegate>

using namespace Qt::StringLiterals;

APROCH_NAMESPACE_BEGIN

static QFontDatabase::WritingSystem writingSystemFromScript(QLocale::Script script)
{
    switch (script)
    {
    case QLocale::ArabicScript:
        return QFontDatabase::Arabic;
    case QLocale::CyrillicScript:
        return QFontDatabase::Cyrillic;
    case QLocale::GurmukhiScript:
        return QFontDatabase::Gurmukhi;
    case QLocale::SimplifiedHanScript:
        return QFontDatabase::SimplifiedChinese;
    case QLocale::TraditionalHanScript:
        return QFontDatabase::TraditionalChinese;
    case QLocale::LatinScript:
        return QFontDatabase::Latin;
    case QLocale::ArmenianScript:
        return QFontDatabase::Armenian;
    case QLocale::BengaliScript:
        return QFontDatabase::Bengali;
    case QLocale::DevanagariScript:
        return QFontDatabase::Devanagari;
    case QLocale::GeorgianScript:
        return QFontDatabase::Georgian;
    case QLocale::GreekScript:
        return QFontDatabase::Greek;
    case QLocale::GujaratiScript:
        return QFontDatabase::Gujarati;
    case QLocale::HebrewScript:
        return QFontDatabase::Hebrew;
    case QLocale::JapaneseScript:
        return QFontDatabase::Japanese;
    case QLocale::KhmerScript:
        return QFontDatabase::Khmer;
    case QLocale::KannadaScript:
        return QFontDatabase::Kannada;
    case QLocale::KoreanScript:
        return QFontDatabase::Korean;
    case QLocale::LaoScript:
        return QFontDatabase::Lao;
    case QLocale::MalayalamScript:
        return QFontDatabase::Malayalam;
    case QLocale::MyanmarScript:
        return QFontDatabase::Myanmar;
    case QLocale::TamilScript:
        return QFontDatabase::Tamil;
    case QLocale::TeluguScript:
        return QFontDatabase::Telugu;
    case QLocale::ThaanaScript:
        return QFontDatabase::Thaana;
    case QLocale::ThaiScript:
        return QFontDatabase::Thai;
    case QLocale::TibetanScript:
        return QFontDatabase::Tibetan;
    case QLocale::SinhalaScript:
        return QFontDatabase::Sinhala;
    case QLocale::SyriacScript:
        return QFontDatabase::Syriac;
    case QLocale::OriyaScript:
        return QFontDatabase::Oriya;
    case QLocale::OghamScript:
        return QFontDatabase::Ogham;
    case QLocale::RunicScript:
        return QFontDatabase::Runic;
    case QLocale::NkoScript:
        return QFontDatabase::Nko;
    default:
        return QFontDatabase::Any;
    }
}

static QFontDatabase::WritingSystem writingSystemFromLocale()
{
    QStringList uiLanguages = QLocale::system().uiLanguages();
    QLocale::Script script;
    if (!uiLanguages.isEmpty())
        script = QLocale(uiLanguages.at(0)).script();
    else
        script = QLocale::system().script();

    return writingSystemFromScript(script);
}

static QFontDatabase::WritingSystem writingSystemForFont(const QFont& font, bool* hasLatin)
{
    QList<QFontDatabase::WritingSystem> writingSystems = QFontDatabase::writingSystems(font.families().first());
    // qDebug() << font.families().first() << writingSystems;

    // this just confuses the algorithm below. Vietnamese is Latin with lots of special chars
    writingSystems.removeOne(QFontDatabase::Vietnamese);
    *hasLatin = writingSystems.removeOne(QFontDatabase::Latin);

    if (writingSystems.isEmpty())
        return QFontDatabase::Any;

    QFontDatabase::WritingSystem system = writingSystemFromLocale();

    if (writingSystems.contains(system))
        return system;

    if (system == QFontDatabase::TraditionalChinese
        && writingSystems.contains(QFontDatabase::SimplifiedChinese))
    {
        return QFontDatabase::SimplifiedChinese;
    }

    if (system == QFontDatabase::SimplifiedChinese
        && writingSystems.contains(QFontDatabase::TraditionalChinese))
    {
        return QFontDatabase::TraditionalChinese;
    }

    system = writingSystems.constLast();

    if (!*hasLatin)
    {
        // we need to show something
        return system;
    }

    if (writingSystems.size() == 1 && system > QFontDatabase::Cyrillic)
        return system;

    if (writingSystems.size() <= 2 && system > QFontDatabase::Armenian && system < QFontDatabase::Vietnamese)
        return system;

    if (writingSystems.size() <= 5 && system >= QFontDatabase::SimplifiedChinese && system <= QFontDatabase::Korean)
        return system;

    return QFontDatabase::Any;
}

class AFontComboBoxPrivate : public AComboBoxPrivate
{
public:
    static QRect availableScreenGeometry(const QWidget* widget)
    {
        return widget->screen()->availableGeometry();
    }

    QFontComboBox::FontFilters filters = QFontComboBox::AllFonts;
    QFont currentFont;
    QHash<QFontDatabase::WritingSystem, QString> sampleTextForWritingSystem;
    QHash<QString, QString> sampleTextForFontFamily;
    QHash<QString, QFont> displayFontForFontFamily;
};

class AFontFamilyDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit AFontFamilyDelegate(QObject* parent, AFontComboBoxPrivate* comboP);

    // painting
    void paint(QPainter* painter,
               const QStyleOptionViewItem& option,
               const QModelIndex& index) const override;

    const QIcon truetype;
    const QIcon bitmap;
    QFontDatabase::WritingSystem writingSystem;
    AFontComboBoxPrivate* comboPrivate;
};

AFontFamilyDelegate::AFontFamilyDelegate(QObject* parent, AFontComboBoxPrivate* comboP)
    : QStyledItemDelegate(parent),
    // TODO
    truetype(QStringLiteral(":/qt-project.org/styles/commonstyle/images/fonttruetype-16.png")),
    bitmap(QStringLiteral(":/qt-project.org/styles/commonstyle/images/fontbitmap-16.png")),
    writingSystem(QFontDatabase::Any),
    comboPrivate(comboP)
{
}

void AFontFamilyDelegate::paint(QPainter* painter,
                                const QStyleOptionViewItem& option,
                                const QModelIndex& index) const
{
    QString text = index.data(Qt::DisplayRole).toString();
    QFont font(option.font);
    font.setPointSize(QFontInfo(font).pointSize() * 5 / 4);
    QFont font2 = font;
    font2.setFamilies(QStringList{ text });

    bool hasLatin;
    QFontDatabase::WritingSystem system = writingSystemForFont(font2, &hasLatin);
    if (hasLatin)
        font = font2;

    font = comboPrivate->displayFontForFontFamily.value(text, font);

    QRect r = option.rect;

    /*if (option.state & QStyle::State_Selected)
    {
        painter->save();
        painter->setBrush(option.palette.highlight());
        painter->setPen(Qt::NoPen);
        painter->drawRect(option.rect);
        painter->setPen(QPen(option.palette.highlightedText(), 0));
    }*/

    QStyleOptionViewItem newOption = option;
    newOption.text = QString();
    auto model = const_cast<QAbstractItemModel*>(index.model());
    model->setData(index, QString(), Qt::DisplayRole);
    painter->save();
    QStyledItemDelegate::paint(painter, newOption, index);
    painter->restore();
    model->setData(index, text, Qt::DisplayRole);

    // TODO: open interface to supports
    bool showFontTypeIcon = false;
    QSize actualSize;
    if (showFontTypeIcon)
    {
        const QIcon* icon = &bitmap;
        if (QFontDatabase::isSmoothlyScalable(text))
        {
            icon = &truetype;
        }
        actualSize = icon->actualSize(r.size());
        const QRect iconRect = QStyle::alignedRect(option.direction, option.displayAlignment, actualSize, r);
        icon->paint(painter, iconRect, Qt::AlignLeft | Qt::AlignVCenter);
    }

    // TODO: use AStyleSheet::value(parent(), "QAbstractItemView::item", "padding-left").toInt()
    if (option.direction == Qt::RightToLeft)
        r.setRight(r.right() - actualSize.width() - 8);
    else
        r.setLeft(r.left() + actualSize.width() + 8);

    QFont old = painter->font();
    painter->setFont(font);

    const Qt::Alignment textAlign = QStyle::visualAlignment(option.direction, option.displayAlignment);
    // If the ascent of the font is larger than the height of the rect,
    // we will clip the text, so it's better to align the tight bounding rect in this case
    // This is specifically for fonts where the ascent is very large compared to
    // the descent, like certain of the Stix family.
    QFontMetricsF fontMetrics(font);
    if (fontMetrics.ascent() > r.height())
    {
        QRectF tbr = fontMetrics.tightBoundingRect(text);
        QRect textRect(r);
        textRect.setHeight(textRect.height() + (r.height() - tbr.height()));
        painter->drawText(textRect, Qt::AlignBottom | Qt::TextSingleLine | textAlign, text);
    }
    else
    {
        painter->drawText(r, Qt::AlignVCenter | Qt::TextSingleLine | textAlign, text);
    }

    if (writingSystem != QFontDatabase::Any)
        system = writingSystem;

    const QString sampleText = comboPrivate->sampleTextForFontFamily.value(text, comboPrivate->sampleTextForWritingSystem.value(system));
    if (system != QFontDatabase::Any || !sampleText.isEmpty())
    {
        int w = painter->fontMetrics().horizontalAdvance(text + "  "_L1);
        painter->setFont(font2);
        const QString sample = !sampleText.isEmpty() ? sampleText : QFontDatabase::writingSystemSample(system);
        if (option.direction == Qt::RightToLeft)
            r.setRight(r.right() - w);
        else
            r.setLeft(r.left() + w);
        painter->drawText(r, Qt::AlignVCenter | Qt::TextSingleLine | textAlign, sample);
    }
    painter->setFont(old);

    /*if (option.state & QStyle::State_Selected)
        painter->restore();*/
}

AFontComboBox::AFontComboBox(QWidget* parent)
    : AComboBox(*new AFontComboBoxPrivate, parent)
{
    auto d = (AFontComboBoxPrivate*)d_ptr;
    d->currentFont = font();
    setEditable(true);

    QStringListModel* m = new QStringListModel(this);
    setModel(m);
    setInsertPolicy(QComboBox::NoInsert);
    setItemDelegate(new AFontFamilyDelegate(this, d));
    QListView* lview = qobject_cast<QListView*>(view());
    if (lview)
        lview->setUniformItemSizes(true);
    setWritingSystem(QFontDatabase::Any);

    connect(this, &AFontComboBox::currentTextChanged, this, &AFontComboBox::_currentChanged);
    connect(qApp, &QApplication::fontDatabaseChanged, this, &AFontComboBox::_updateModel);
}

AFontComboBox::~AFontComboBox()
{
}

void AFontComboBox::setWritingSystem(QFontDatabase::WritingSystem script)
{
    AFontFamilyDelegate* delegate = qobject_cast<AFontFamilyDelegate*>(view()->itemDelegate());
    if (delegate)
        delegate->writingSystem = script;
    _updateModel();
}

QFontDatabase::WritingSystem AFontComboBox::getWritingSystem() const
{
    AFontFamilyDelegate* delegate = qobject_cast<AFontFamilyDelegate*>(view()->itemDelegate());
    if (delegate)
        return delegate->writingSystem;
    return QFontDatabase::Any;
}

void AFontComboBox::setFontFilters(QFontComboBox::FontFilters filters)
{
    auto d = (AFontComboBoxPrivate*)d_ptr;
    d->filters = filters;
    _updateModel();
}

QFontComboBox::FontFilters AFontComboBox::getFontFilters() const
{
    auto d = (AFontComboBoxPrivate*)d_ptr;
    return d->filters;
}

QFont AFontComboBox::getCurrentFont() const
{
    auto d = (AFontComboBoxPrivate*)d_ptr;
    return d->currentFont;
}

void AFontComboBox::setCurrentFont(const QFont& font)
{
    auto d = (AFontComboBoxPrivate*)d_ptr;
    if (font != d->currentFont)
    {
        d->currentFont = font;
        _updateModel();
        if (d->currentFont == font)
        {
            // else the signal has already be emitted by _updateModel
            emit currentFontChanged(d->currentFont);
        }
    }
}

QSize AFontComboBox::sizeHint() const
{
    QSize sz = QComboBox::sizeHint();
    QFontMetrics fm(font());
    sz.setWidth(fm.horizontalAdvance(u'm') * 14);
    return sz;
}

void AFontComboBox::setSampleTextForSystem(QFontDatabase::WritingSystem writingSystem, const QString& sampleText)
{
    auto d = (AFontComboBoxPrivate*)d_ptr;
    d->sampleTextForWritingSystem[writingSystem] = sampleText;
}

QString AFontComboBox::getSampleTextForSystem(QFontDatabase::WritingSystem writingSystem) const
{
    auto d = (AFontComboBoxPrivate*)d_ptr;
    return d->sampleTextForWritingSystem.value(writingSystem);
}

void AFontComboBox::setSampleTextForFont(const QString& fontFamily, const QString& sampleText)
{
    auto d = (AFontComboBoxPrivate*)d_ptr;
    d->sampleTextForFontFamily[fontFamily] = sampleText;
}

QString AFontComboBox::getSampleTextForFont(const QString& fontFamily) const
{
    auto d = (AFontComboBoxPrivate*)d_ptr;
    return d->sampleTextForFontFamily.value(fontFamily);
}

void AFontComboBox::setDisplayFont(const QString& fontFamily, const QFont& font)
{
    auto d = (AFontComboBoxPrivate*)d_ptr;
    d->displayFontForFontFamily[fontFamily] = font;
}

std::optional<QFont> AFontComboBox::getDisplayFont(const QString& fontFamily) const
{
    auto d = (AFontComboBoxPrivate*)d_ptr;
    return d->displayFontForFontFamily.value(fontFamily, {});
}

void AFontComboBox::_currentChanged(const QString& text)
{
    auto d = (AFontComboBoxPrivate*)d_ptr;
    QStringList families = d->currentFont.families();
    if (families.isEmpty() || families.first() != text)
    {
        d->currentFont.setFamilies(QStringList{ text });
        emit currentFontChanged(d->currentFont);
    }
}

void AFontComboBox::_updateModel()
{
    auto d = (AFontComboBoxPrivate*)d_ptr;
    if (QCoreApplication::closingDown())
        return;

    const int scalableMask = (QFontComboBox::ScalableFonts | QFontComboBox::NonScalableFonts);
    const int spacingMask = (QFontComboBox::ProportionalFonts | QFontComboBox::MonospacedFonts);

    QStringListModel* m = qobject_cast<QStringListModel*>(model());
    if (!m)
        return;
    AFontFamilyDelegate* delegate = qobject_cast<AFontFamilyDelegate*>(view()->itemDelegate());
    QFontDatabase::WritingSystem system = delegate ? delegate->writingSystem : QFontDatabase::Any;

    QStringList list = QFontDatabase::families(system);
    QStringList result;

    int offset = 0;
    QFontInfo fi(d->currentFont);

    for (int i = 0; i < list.size(); ++i)
    {
        if (QFontDatabase::isPrivateFamily(list.at(i)))
            continue;

        if ((d->filters & scalableMask) && (d->filters & scalableMask) != scalableMask)
        {
            if (bool(d->filters & QFontComboBox::ScalableFonts) != QFontDatabase::isSmoothlyScalable(list.at(i)))
                continue;
        }
        if ((d->filters & spacingMask) && (d->filters & spacingMask) != spacingMask)
        {
            if (bool(d->filters & QFontComboBox::MonospacedFonts) != QFontDatabase::isFixedPitch(list.at(i)))
                continue;
        }
        result += list.at(i);
        if (list.at(i) == fi.family() || list.at(i).startsWith(fi.family() + " ["_L1))
            offset = result.size() - 1;
    }
    list = result;

    //we need to block the signals so that the model doesn't emit reset
    //this prevents the current index from changing
    //it will be updated just after this
    ///TODO: we should finda way to avoid blocking signals and have a real update of the model
    {
        const QSignalBlocker blocker(m);
        m->setStringList(list);
        // Since the modelReset signal is blocked the view will not emit an accessibility event
#if QT_CONFIG(accessibility)
        if (QAccessible::isActive())
        {
            QAccessibleTableModelChangeEvent accessibleEvent(view(), QAccessibleTableModelChangeEvent::ModelReset);
            QAccessible::updateAccessibility(&accessibleEvent);
        }
#endif
    }

    if (list.isEmpty())
    {
        if (d->currentFont != QFont())
        {
            d->currentFont = QFont();
            emit currentFontChanged(d->currentFont);
        }
    }
    else
    {
        setCurrentIndex(offset);
    }
}

APROCH_NAMESPACE_END

#include "AFontComboBox.moc"
