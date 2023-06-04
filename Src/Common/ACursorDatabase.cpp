#include "stdafx.h"
#include "ACursorDatabase.h"

#include <QCoreApplication>

APROCH_NAMESPACE_BEGIN

class ACursorDatabasePrivate
{
public:
    void appendCursor(Qt::CursorShape shape, const QString& name, const QIcon& icon)
    {
        if (m_cursorShapeToValue.contains(shape))
        {
            const int index = m_cursorShapeToValue.value(shape);
            m_cursorNames[index] = name;
            m_cursorIcons[index] = icon;
            return;
        }

        const int value = m_cursorNames.count();
        m_cursorNames.append(name);
        m_cursorIcons.insert(value, icon);
        m_valueToCursorShape.insert(value, shape);
        m_cursorShapeToValue.insert(shape, value);
    }

    QStringList m_cursorNames;
    QMap<int, QIcon> m_cursorIcons;
    QMap<int, Qt::CursorShape> m_valueToCursorShape;
    QMap<Qt::CursorShape, int> m_cursorShapeToValue;
};

ACursorDatabase::ACursorDatabase()
    : d_ptr(new ACursorDatabasePrivate())
{
    d_ptr->appendCursor(Qt::ArrowCursor, QCoreApplication::translate("ACursorDatabase", "Arrow"),
                        QIcon(QLatin1String(":/qt-project.org/qtpropertybrowser/images/cursor-arrow.png")));
    d_ptr->appendCursor(Qt::UpArrowCursor, QCoreApplication::translate("ACursorDatabase", "Up Arrow"),
                        QIcon(QLatin1String(":/qt-project.org/qtpropertybrowser/images/cursor-uparrow.png")));
    d_ptr->appendCursor(Qt::CrossCursor, QCoreApplication::translate("ACursorDatabase", "Cross"),
                        QIcon(QLatin1String(":/qt-project.org/qtpropertybrowser/images/cursor-cross.png")));
    d_ptr->appendCursor(Qt::WaitCursor, QCoreApplication::translate("ACursorDatabase", "Wait"),
                        QIcon(QLatin1String(":/qt-project.org/qtpropertybrowser/images/cursor-wait.png")));
    d_ptr->appendCursor(Qt::IBeamCursor, QCoreApplication::translate("ACursorDatabase", "IBeam"),
                        QIcon(QLatin1String(":/qt-project.org/qtpropertybrowser/images/cursor-ibeam.png")));
    d_ptr->appendCursor(Qt::SizeVerCursor, QCoreApplication::translate("ACursorDatabase", "Size Vertical"),
                        QIcon(QLatin1String(":/qt-project.org/qtpropertybrowser/images/cursor-sizev.png")));
    d_ptr->appendCursor(Qt::SizeHorCursor, QCoreApplication::translate("ACursorDatabase", "Size Horizontal"),
                        QIcon(QLatin1String(":/qt-project.org/qtpropertybrowser/images/cursor-sizeh.png")));
    d_ptr->appendCursor(Qt::SizeFDiagCursor, QCoreApplication::translate("ACursorDatabase", "Size Backslash"),
                        QIcon(QLatin1String(":/qt-project.org/qtpropertybrowser/images/cursor-sizef.png")));
    d_ptr->appendCursor(Qt::SizeBDiagCursor, QCoreApplication::translate("ACursorDatabase", "Size Slash"),
                        QIcon(QLatin1String(":/qt-project.org/qtpropertybrowser/images/cursor-sizeb.png")));
    d_ptr->appendCursor(Qt::SizeAllCursor, QCoreApplication::translate("ACursorDatabase", "Size All"),
                        QIcon(QLatin1String(":/qt-project.org/qtpropertybrowser/images/cursor-sizeall.png")));
    d_ptr->appendCursor(Qt::BlankCursor, QCoreApplication::translate("ACursorDatabase", "Blank"),
                        QIcon());
    d_ptr->appendCursor(Qt::SplitVCursor, QCoreApplication::translate("ACursorDatabase", "Split Vertical"),
                        QIcon(QLatin1String(":/qt-project.org/qtpropertybrowser/images/cursor-vsplit.png")));
    d_ptr->appendCursor(Qt::SplitHCursor, QCoreApplication::translate("ACursorDatabase", "Split Horizontal"),
                        QIcon(QLatin1String(":/qt-project.org/qtpropertybrowser/images/cursor-hsplit.png")));
    d_ptr->appendCursor(Qt::PointingHandCursor, QCoreApplication::translate("ACursorDatabase", "Pointing Hand"),
                        QIcon(QLatin1String(":/qt-project.org/qtpropertybrowser/images/cursor-hand.png")));
    d_ptr->appendCursor(Qt::ForbiddenCursor, QCoreApplication::translate("ACursorDatabase", "Forbidden"),
                        QIcon(QLatin1String(":/qt-project.org/qtpropertybrowser/images/cursor-forbidden.png")));
    d_ptr->appendCursor(Qt::OpenHandCursor, QCoreApplication::translate("ACursorDatabase", "Open Hand"),
                        QIcon(QLatin1String(":/qt-project.org/qtpropertybrowser/images/cursor-openhand.png")));
    d_ptr->appendCursor(Qt::ClosedHandCursor, QCoreApplication::translate("ACursorDatabase", "Closed Hand"),
                        QIcon(QLatin1String(":/qt-project.org/qtpropertybrowser/images/cursor-closedhand.png")));
    d_ptr->appendCursor(Qt::WhatsThisCursor, QCoreApplication::translate("ACursorDatabase", "What's This"),
                        QIcon(QLatin1String(":/qt-project.org/qtpropertybrowser/images/cursor-whatsthis.png")));
    d_ptr->appendCursor(Qt::BusyCursor, QCoreApplication::translate("ACursorDatabase", "Busy"),
                        QIcon(QLatin1String(":/qt-project.org/qtpropertybrowser/images/cursor-busy.png")));
}

ACursorDatabase::~ACursorDatabase()
{
}

void ACursorDatabase::clear()
{
    d_ptr->m_cursorNames.clear();
    d_ptr->m_cursorIcons.clear();
    d_ptr->m_valueToCursorShape.clear();
    d_ptr->m_cursorShapeToValue.clear();
}

QStringList ACursorDatabase::getCursorShapeNames() const
{
    return d_ptr->m_cursorNames;
}

QMap<int, QIcon> ACursorDatabase::getCursorShapeIcons() const
{
    return d_ptr->m_cursorIcons;
}

QString ACursorDatabase::getCursorShapeName(const QCursor& cursor) const
{
    int val = getCursorIndex(cursor);
    if (val >= 0)
        return d_ptr->m_cursorNames.at(val);
    return QString();
}

QIcon ACursorDatabase::getCursorShapeIcon(const QCursor& cursor) const
{
    int val = getCursorIndex(cursor);
    return d_ptr->m_cursorIcons.value(val);
}

int ACursorDatabase::getCursorIndex(const QCursor& cursor) const
{
#ifndef QT_NO_CURSOR
    Qt::CursorShape shape = cursor.shape();
    if (d_ptr->m_cursorShapeToValue.contains(shape))
        return d_ptr->m_cursorShapeToValue[shape];
#endif
    return -1;
}

#ifndef QT_NO_CURSOR
QCursor ACursorDatabase::getCursor(int value) const
{
    if (d_ptr->m_valueToCursorShape.contains(value))
        return QCursor(d_ptr->m_valueToCursorShape[value]);
    return QCursor();
}
#endif

void ACursorDatabase::appendCursor(Qt::CursorShape shape, const QString& name, const QIcon& icon)
{
    d_ptr->appendCursor(shape, name, icon);
}

APROCH_NAMESPACE_END