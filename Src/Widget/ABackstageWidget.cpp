/****************************************************************************
 * @file    ABackstageWidget.cpp
 * @date    2023-07-01 
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
#include "ABackstageWidget.h"
#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QAction>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneContextMenuEvent>
#ifdef A_PRIVATE_HEADERS
#include <private/qwidget_p.h>
#include <private/qgraphicsitem_p.h>
#include <private/qapplication_p.h>
#endif
#include "Platform/APlatform.h"
#ifdef A_SHARED_BACKSTAGE_WINDOW
#include <QAccessibleObject>
#endif
#ifdef A_MEMORY_DEBUG
#include "QtitanMSVSDebug.h"
#endif

#ifndef QT_STATIC
#ifdef _MSC_VER
static
#endif
bool qt_sendSpontaneousEvent(QObject *receiver, QEvent *event)
{
    return QCoreApplication::sendSpontaneousEvent(receiver, event);
}
#else
extern bool qt_sendSpontaneousEvent(QObject *receiver, QEvent *event);
#endif

APROCH_NAMESPACE_BEGIN

static void aproch_activateScene(QGraphicsScene* scene)
{
    if (scene == nullptr || scene->isActive())
        return;
    QEvent event(QEvent::WindowActivate);
    qt_sendSpontaneousEvent(scene, &event);
}

static void aproch_deactivateScene(QGraphicsScene* scene)
{
    if (scene == nullptr || !scene->isActive())
        return;
    QEvent event(QEvent::WindowDeactivate);
    qt_sendSpontaneousEvent(scene, &event);
}

// TODO
#ifdef A_SHARED_BACKSTAGE_WINDOW
class ABackstageSharedWindow;

class QGraphicsSceneEx : public QGraphicsScene
{
public:
    QGraphicsSceneEx(ABackstageSharedWindow* sharedWindow);
protected:
    virtual bool event(QEvent* e);
private:
    ABackstageSharedWindow* m_sharedWindow;
    int m_processDirtyItemsIndex;
};
#ifndef Q_OS_LINUX
#define A_SHAREDWINDOW_PROPERTY "aproch_sharedWindow"
class QGraphicsSceneExAccessible : public QAccessibleObject
{
public:
    QGraphicsSceneExAccessible(QObject* obj) : QAccessibleObject(obj) {}
    virtual bool isValid() const { return true; }
    virtual QAccessibleInterface *parent() const { return QAccessible::queryAccessibleInterface(object()->parent()); }
    virtual QAccessibleInterface *child(int) const { return nullptr; }
    virtual int childCount() const { return -1; }
    virtual int indexOfChild(const QAccessibleInterface *) const { return -1; }
    virtual QString text(QAccessible::Text) const { return object()->objectName(); }
    virtual QAccessible::Role role() const { return QAccessible::Grouping; }
    virtual QAccessible::State state() const { return QAccessible::State(); }
    static QAccessibleInterface* factory(const QString& key, QObject* object)
    {
        Q_UNUSED(key)
        if (qobject_cast<QGraphicsScene*>(object) == nullptr || !object->property(A_SHAREDWINDOW_PROPERTY).toBool())
            return nullptr;
        return new QGraphicsSceneExAccessible(object);
    }
    static bool installFactory()
    {
        static bool _installed = (QAccessible::installFactory(factory), true);
        return _installed;
    }
};
#endif

class ABackstageSharedWindow : public QWidget
{
public:
    ABackstageSharedWindow(QWidget* window);
    virtual ~ABackstageSharedWindow();
    void addWidget(ABackstageWidget* backstageWidget);
    void removeWidget(ABackstageWidget* backstageWidget);
    bool containsWidget(ABackstageWidget* backstageWidget) const;
    bool isEmpty() const;
    QGraphicsScene* scene() const;
    void ensureSize(const QSize& size);
public:
    typedef QHash<QWidget *, ABackstageSharedWindow *> SharedWindowHash;
    static SharedWindowHash* allBackstageWindows;
    static bool setWindow(ABackstageWidget* backstageWidget);
    static void resetWindow(ABackstageWidget* backstageWidget);
    static void deleteNotUsed();
    void updateRequest(bool qwidgetupdate);
protected:
    bool event(QEvent *event) override;
    bool eventFilter(QObject* object, QEvent* event) override;
private:
    void finalize();
    QGraphicsSceneEx m_scene;
    QSet<ABackstageWidget*> m_widgets;
    QGraphicsProxyWidget* m_windowProxyWidget;
};
#endif

#ifdef A_SHARED_BACKSTAGE_WINDOW
/* ABackstageSharedWindow */
ABackstageSharedWindow::SharedWindowHash* ABackstageSharedWindow::allBackstageWindows = nullptr;
bool ABackstageSharedWindow::setWindow(ABackstageWidget* backstageWidget)
{
    QWidget* window = backstageWidget->viewWidget()->window();
    Q_ASSERT(window != nullptr);
    ABackstageSharedWindow* backstageWindow = nullptr;
    if (allBackstageWindows != nullptr)
    {
        if (allBackstageWindows->contains(window))
            backstageWindow = allBackstageWindows->value(window);
    }

    if (backstageWidget->m_backstageWindow != nullptr)
    {
        if (backstageWidget->m_backstageWindow == backstageWindow)
            return false;
    }

    if (backstageWindow == nullptr)
        backstageWindow = new ABackstageSharedWindow(window);

    if (backstageWidget->m_backstageWindow != nullptr)
        resetWindow(backstageWidget);
    Q_ASSERT(backstageWidget->m_backstageWindow == nullptr);
  
    backstageWindow->addWidget(backstageWidget);
    backstageWidget->m_backstageWindow = backstageWindow;
    return true;
}

void ABackstageSharedWindow::resetWindow(ABackstageWidget* backstageWidget)
{
    if (backstageWidget->m_backstageWindow != nullptr)
    {
        backstageWidget->m_backstageWindow->removeWidget(backstageWidget);
        if (backstageWidget->m_backstageWindow->isEmpty())
        {
            backstageWidget->m_backstageWindow->finalize();
            backstageWidget->m_backstageWindow->deleteLater();
        }
    }
    backstageWidget->m_backstageWindow = nullptr;
}

void ABackstageSharedWindow::deleteNotUsed()
{
    while (allBackstageWindows != nullptr)
    {
        bool hasEmpty = false;

        for (SharedWindowHash::iterator it = allBackstageWindows->begin();
            it != allBackstageWindows->end(); ++it)
        {
            ABackstageSharedWindow* w = it.value();
            if (w->isEmpty())
            {
                delete w;
                hasEmpty = true;
                break;
            }
        }
        if (!hasEmpty)
            break;
    }
}

ABackstageSharedWindow::ABackstageSharedWindow(QWidget* window)
    : QWidget(), m_scene(this), m_windowProxyWidget(nullptr)
{
    //Means all toplevel/popup child windows will be created natively.
    setWindowFlags(windowFlags() | Qt::BypassGraphicsProxyWidget);
    setGeometry(0, 0, 400, 200);
    m_scene.setSceneRect(0, 0, 400, 200);
    m_windowProxyWidget = m_scene.addWidget(this);
    aproch_activateScene(&m_scene);
    if (allBackstageWindows == nullptr)
        allBackstageWindows = new SharedWindowHash();
    allBackstageWindows->insert(window, this);
    window->installEventFilter(this);
    setVisible(window->isVisible());
    if (window->isActiveWindow())
        aproch_activateScene(&m_scene);
    else
        aproch_deactivateScene(&m_scene);
}

void ABackstageSharedWindow::finalize()
{
    if (m_windowProxyWidget != nullptr)
        m_windowProxyWidget->setWidget(nullptr);
    A_DELETE_AND_NULL(m_windowProxyWidget);

    Q_ASSERT(isEmpty());

    if (allBackstageWindows != nullptr)
    {
        for (SharedWindowHash::iterator it = allBackstageWindows->begin();
            it != allBackstageWindows->end(); ++it)
        {
            if (it.value() == this)
            {
                it.key()->removeEventFilter(this);
                allBackstageWindows->erase(it);
                break;
            }
        }
        if (allBackstageWindows->size() == 0)
            A_DELETE_AND_NULL(allBackstageWindows);
    }
}

ABackstageSharedWindow::~ABackstageSharedWindow()
{
    finalize();
}

void ABackstageSharedWindow::addWidget(ABackstageWidget* backstageWidget)
{
    m_widgets.insert(backstageWidget);

    QWidget* w = backstageWidget->widget();
    Q_ASSERT(w != nullptr);
    if (w == nullptr)
        return;
    if (backstageWidget->m_proxyWidget != nullptr)
    {
        Q_ASSERT(false);
        backstageWidget->m_proxyWidget->setWidget(nullptr);
        A_DELETE_AND_NULL(backstageWidget->m_proxyWidget);
    }
    w->setParent(this);
    backstageWidget->m_proxyWidget = m_windowProxyWidget->createProxyForChildWidget(w);
}

void ABackstageSharedWindow::removeWidget(ABackstageWidget* backstageWidget)
{
    if (!containsWidget(backstageWidget))
    {
        Q_ASSERT(false);
        return;
    }
    m_widgets.remove(backstageWidget);
    
    if (backstageWidget->m_proxyWidget != nullptr)
    {
        backstageWidget->m_proxyWidget->setWidget(nullptr);
        A_DELETE_AND_NULL(backstageWidget->m_proxyWidget);
    }

    if (backstageWidget->widget() != nullptr)
        backstageWidget->widget()->setParent(nullptr);
}

bool ABackstageSharedWindow::containsWidget(ABackstageWidget* backstageWidget) const
{
    return m_widgets.contains(backstageWidget);
}

bool ABackstageSharedWindow::isEmpty() const
{
    return m_widgets.isEmpty();
}

QGraphicsScene* ABackstageSharedWindow::scene() const
{
    ABackstageSharedWindow* w = const_cast<ABackstageSharedWindow *>(this);
    return &w->m_scene;
}

void ABackstageSharedWindow::ensureSize(const QSize& size)
{
    QSize oldSize = this->size();
    QSize s(qMax(oldSize.width(), size.width()), qMax(oldSize.height(), size.height()));
    if (oldSize != s)
        resize(s);
}

static void aproch_add_dirty_region(QWidget* w, QRegion& region, const QPoint& offset)
{
    if (!w->isVisible())
        return;
    QWidgetPrivate* p = qt_widget_private(w);
    if (p->inDirtyList)
    {
        Q_ASSERT(!p->dirty.isEmpty());
        region += p->dirty.translated(offset);
    }
    else if (p->dirtyOpaqueChildren || p->isScrolled || p->isMoved)
    {
        region += w->rect().translated(offset);
        return;
    }

    for (QObjectList::const_iterator it = w->children().constBegin(); 
        it != w->children().constEnd(); ++it)
    {
        if (!(*it)->isWidgetType())
            continue;
        QWidget* w = static_cast<QWidget *>(*it);
        aproch_add_dirty_region(w, region, offset + w->geometry().topLeft());
    }
}

void ABackstageSharedWindow::updateRequest(bool qwidgetupdate)
{
    for (QSet<ABackstageWidget*>::const_iterator it = m_widgets.constBegin();
        it != m_widgets.constEnd(); ++it)
    {
        ABackstageWidget* backstageWidget = *it;
        QWidget* w = backstageWidget->widget();
        if (w == nullptr)
            continue;
        QRegion region = backstageWidget->m_needsRepaint;

        if (qwidgetupdate)
        {
            aproch_add_dirty_region(w, region, w->geometry().topLeft());
            backstageWidget->m_needsRepaint = region;
        }
        else
        {
            QGraphicsItemPrivate* item =
                QGraphicsItemPrivate::get(backstageWidget->m_proxyWidget);
            Q_ASSERT(item != nullptr);
            if (item != nullptr && item->dirty)
                region += item->needsRepaint.toRect();
            region -= backstageWidget->m_needsRepaint; //Already was painted
            backstageWidget->m_needsRepaint = QRegion();

        }

        if (region.isEmpty())
            continue;
        QList<QRectF> rects;
#if (QT_VERSION >= QT_VERSION_CHECK(5, 8, 0))
        for (QRegion::const_iterator it = region.begin(); it != region.end(); ++it)
            rects.append(*it);
#else
        QVector<QRect> rcs = region.rects();
        for (QVector<QRect>::const_iterator it = rcs.constBegin(); it != rcs.constEnd(); ++it)
            rects.append(*it);
#endif
        backstageWidget->changed(rects);
    }
}

bool ABackstageSharedWindow::event(QEvent *event)
{
    switch (event->type())
    {
        case QEvent::UpdateRequest:
        {
            updateRequest(true);
        }
        break;
        default:
            break;
    }
    return QWidget::event(event);
}

bool ABackstageSharedWindow::eventFilter(QObject* object, QEvent* event)
{
    if (event->type() == QEvent::Show || event->type() == QEvent::Hide)
        setVisible(event->type() == QEvent::Show);
    else if (event->type() == QEvent::Destroy)
    {
        while (m_widgets.size() > 0)
            ABackstageSharedWindow::resetWindow(*m_widgets.begin());
        return false;
    }

    return QWidget::eventFilter(object, event);
}

/* ABackstageWidgetBase */
ABackstageWidgetBase::ABackstageWidgetBase(QWidget* viewWidget, QObject* parent)
    : QObject(parent),
      m_viewWidget(viewWidget), m_backstageWindow(nullptr)
{
}

ABackstageWidgetBase::~ABackstageWidgetBase()
{
}

QWidget* ABackstageWidgetBase::viewWidget() const
{
    return m_viewWidget;
}

/* QGraphicsSceneEx */
QGraphicsSceneEx::QGraphicsSceneEx(ABackstageSharedWindow* sharedWindow)
    : QGraphicsScene(), m_sharedWindow(sharedWindow)
{
    m_processDirtyItemsIndex = metaObject()->indexOfSlot("_q_processDirtyItems()");
#ifdef Q_OS_LINUX
    setProperty(A_SHAREDWINDOW_PROPERTY, true);
    QGraphicsSceneExAccessible::installFactory();
#endif
}

bool QGraphicsSceneEx::event(QEvent* e)
{
    switch (e->type())
    {
    case QEvent::MetaCall:
    {
        QMetaCallEvent *mce = static_cast<QMetaCallEvent*>(e);
        if (mce->id() == m_processDirtyItemsIndex)
            m_sharedWindow->updateRequest(false);
    }
    break;
    default:
        break;
    }
    return QGraphicsScene::event(e);
}

#endif

/* ABackstageWidget */
ABackstageWidget::ABackstageWidget(QWidget* viewWidget, QObject* parent)
    : ABackstageWidgetBase(viewWidget, parent),
    m_widget(nullptr),
    m_cursorWidget(nullptr),
    m_mousePressWidget(nullptr),
    m_proxyWidget(nullptr),
    m_mousePressButtons(Qt::NoButton),
    m_mousePressButton(Qt::NoButton)
#ifdef A_SHARED_BACKSTAGE_WINDOW    
    , m_overWidget(nullptr) 
#else
#endif
{
}

ABackstageWidget::~ABackstageWidget()
{
    disconnectWidget();
    
    delete m_widget;
    m_widget = nullptr;

#ifdef A_SHARED_BACKSTAGE_WINDOW
    ABackstageSharedWindow::deleteNotUsed();
#endif
}

QWidget* ABackstageWidget::widget() const
{
    return m_widget;
}

void ABackstageWidget::setWidget(QWidget* widget, const QRect& geometry)
{
    if (m_widget == widget)
        return;
    if (m_widget != nullptr)
        disconnectWidget();
    m_widget = widget;

    connectWidget(geometry);
}

void ABackstageWidget::syncToView()
{
    QRect r = QRect(m_margins.left(), m_margins.top(),
        m_geometry.size().width() - (m_margins.left() + m_margins.right()),
        m_geometry.size().height() - (m_margins.top() + m_margins.bottom()));
    r.setWidth(qMax(r.width(), 0));
    r.setHeight(qMax(r.height(), 0));

#ifndef A_SHARED_BACKSTAGE_WINDOW
    scene()->setSceneRect(m_geometry);
    const bool _changed = false;
#else
    const bool _changed = ABackstageSharedWindow::setWindow(this);
    m_backstageWindow->ensureSize(r.size());
#endif

    if (m_widget->geometry() != r || _changed)
    {
        m_widget->setGeometry(QRect(r.left() - 6 , 0, r.width(), r.height()+ 1));
        if (r.width() == 0 || r.height() == 0)
        {
            //#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
            m_widget->setVisible(false);
            m_widget->setAttribute(Qt::WA_Mapped, false);
            //#endif
        }
        else
        {
            //#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
            m_widget->setVisible(true);
            m_widget->setAttribute(Qt::WA_Mapped, true);
            //#endif
        }
    }
}

QGraphicsScene* ABackstageWidget::scene() const
{
#ifndef A_SHARED_BACKSTAGE_WINDOW
    return const_cast<ABackstageWidget *>(this);
#else
    return m_backstageWindow != nullptr ? m_backstageWindow->scene() : nullptr;
#endif
}

void ABackstageWidget::widgetDestroyed()
{
    m_widget = nullptr;
    disconnectWidget();
}

void ABackstageWidget::connectWidget(const QRect& geometry)
{
    if (m_widget == nullptr)
        return;
    connect(m_widget, SIGNAL(destroyed()), this, SLOT(widgetDestroyed()));
#ifndef A_SHARED_BACKSTAGE_WINDOW
    m_widget->setWindowFlags(m_widget->windowFlags() | Qt::BypassGraphicsProxyWidget);  //Means all toplevel/popup child windows will be created natively.
    Q_ASSERT(m_proxyWidget == nullptr);
    m_proxyWidget = addWidget(m_widget);
    m_proxyWidget->setMinimumSize(0, 0);
#endif
    if (!geometry.isEmpty())
        setGeometry(geometry);
    else
        setGeometry(m_widget->rect());
}

void ABackstageWidget::disconnectWidget()
{
    if (m_widget != nullptr)
        disconnect(m_widget, SIGNAL(destroyed()), this, SLOT(widgetDestroyed()));
    m_cursorWidget = nullptr;
    m_mousePressWidget = nullptr;
#ifndef A_SHARED_BACKSTAGE_WINDOW
    if (m_proxyWidget == nullptr)
        return;
    m_proxyWidget->setWidget(nullptr);

    delete m_proxyWidget;
    m_proxyWidget = nullptr;
#else
    ABackstageSharedWindow::resetWindow(this);
    Q_ASSERT(m_proxyWidget == nullptr && m_backstageWindow == nullptr);
#endif
}

void ABackstageWidget::checkCursor(const QPoint& pos)
{
    m_cursorWidget = nullptr;
    QWidget* w = findWidgetByPos(pos);
    if (w == nullptr)
        return;
#if 0
    QWidget* w = m_widget != nullptr ? m_widget->childAt(pos - QPoint(m_margins.left(), m_margins.top())) : nullptr;
    if (w == nullptr && m_widget != nullptr && m_widget->underMouse())
        w = m_widget;
#endif

    while (w)
    {
        if (w->testAttribute(Qt::WA_SetCursor))
        {
            m_cursorWidget = w;
            break;
        }
        w = w->parentWidget();
    }
}

void ABackstageWidget::activate()
{
    aproch_activateScene(scene());
}

void ABackstageWidget::deactivate()
{
    aproch_deactivateScene(scene());
}

void ABackstageWidget::setMargins(int left, int top, int right, int bottom)
{
    setMargins(QMargins(left, top, right, bottom));
}

const QMargins& ABackstageWidget::margins() const
{
    return m_margins;
}

bool ABackstageWidget::hasFocusItem()
{
#ifndef A_SHARED_BACKSTAGE_WINDOW
    return scene()->focusItem() != nullptr;
#else
    return m_widget != nullptr && m_widget->hasFocus();
#endif
}

bool ABackstageWidget::hasMouseGrabber()
{
#ifndef A_SHARED_BACKSTAGE_WINDOW
    return scene()->mouseGrabberItem() != nullptr;
#else
    return false;
#endif
}

bool ABackstageWidget::isInputMethodSupported() const
{
    if (widget() == nullptr)
        return false;
    return widget()->testAttribute(Qt::WA_InputMethodEnabled);
}

static QWidget* aproch_get_focus_widget(QWidget* w)
{
    if (w == nullptr)
        return nullptr;
    QWidget* retval = w->focusWidget();
    if (retval == nullptr)
        retval = w;
    return retval;
}

QVariant ABackstageWidget::inputMethodQuery(Qt::InputMethodQuery query) const
{
    if (widget() == nullptr)
        return QVariant();
    QWidget* focusWidget = aproch_get_focus_widget(widget());
    return focusWidget->inputMethodQuery(query);
}

void ABackstageWidget::setMargins(const QMargins& margins)
{
    if (m_margins == margins)
        return;
    m_margins = margins;
    setGeometry(QRect(QPoint(0, 0), geometry().size()));
}

void ABackstageWidget::paintContent(QPainter& painter)
{
    if (m_widget == nullptr)
        return;
    syncToView();
    
    if (painter.opacity() == 0)
        return;

    QPoint offset = m_widget->geometry().topLeft();
    QRect clipRect = painter.hasClipping() ? painter.clipBoundingRect().toRect() :
        QRect(0, 0, painter.device()->width(), painter.device()->height());

	QRegion dirty = clipRect.intersected(m_widget->geometry()).translated(-offset);
    if (dirty.isEmpty())
        return;
    painter.save();
    painter.translate(offset);
    painter.setClipRegion(dirty, Qt::IntersectClip);
    m_widget->render(&painter, dirty.boundingRect().topLeft(),
        dirty, QWidget::RenderFlags(QWidget::DrawChildren));
    painter.restore();

#if 0
    QRect rect(0, 0, scene()->width(), scene()->height());
    rect.adjust(m_margins.left(), m_margins.top(), -m_margins.right(), -m_margins.bottom());
    rect.translate(scene()->sceneRect().left(), scene()->sceneRect().top());

    QRect clipRect = painter.hasClipping() ? painter.clipBoundingRect().toRect() : 
        QRect(0, 0, painter.device()->width(), painter.device()->height());
    clipRect = clipRect.intersected(rect);

    if (clipRect.isEmpty())
        return;

    painter.save();

    painter.setClipRect(clipRect);
    QPoint offset(rect.left(), rect.top());
    painter.translate(offset);

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
    if (painter.opacity() == 1.0)
        painter.setOpacity(0.9999); //Hack to fix the flickering under Qt4.
#endif

#if 1
    QStyleOptionGraphicsItem option;
    option.exposedRect = QRect(0, 0, rect.width(), rect.height());
    m_proxyWidget->paint(&painter, &option, nullptr);
#else
    Q_ASSERT(false);
    m_scene.render(&painter, paintRect, paintRect);
#endif
    painter.restore();
#endif
}

QWidget* ABackstageWidget::findWidgetByPos(const QPoint& pos)
{
    if (m_widget == nullptr)
        return nullptr;
    QRect geom = m_widget->geometry();
    if (!geom.contains(pos))
        return nullptr;
    QWidget* w = m_widget->childAt(pos - geom.topLeft());
    if (w == nullptr)
        w = m_widget;
    return w;
}

bool ABackstageWidget::tryShortcut(QKeyEvent* e)
{
    if (e->key() == Qt::Key_unknown)
        return false;

    if (e->key() >= Qt::Key_Shift &&
        e->key() <= Qt::Key_Alt)
        return false;

    if (m_widget == nullptr || !m_widget->isEnabled())
    {
        e->ignore();
        return false;
    }

    QKeySequence keySequence(e->key() | e->modifiers());
    QList<QWidget*> lstWidget = m_widget->findChildren<QWidget*>();
    for (QList<QWidget*>::const_iterator it = lstWidget.constBegin(); it != lstWidget.constEnd(); ++it)
    {
        QList<QAction*> lstAction = (*it)->actions();
        for (QList<QAction*>::const_iterator itAct = lstAction.constBegin(); itAct != lstAction.constEnd(); ++itAct)
        {
            QAction* action = (*itAct);
            if (action->shortcutContext() == Qt::WindowShortcut &&
                action->isEnabled() && keySequence == action->shortcut())
            {
                action->trigger();
                e->setAccepted(true);
                return true;
            }
        }
    }
    return false;
}

void ABackstageWidget::send_mouseOverEvent(const QPoint& pos)
{
    updateMousePressWidget(nullptr, Qt::NoButton);

    if (m_widget == nullptr || !m_widget->isEnabled())
        return;
    QPoint screenPos = QCursor::pos();
#ifdef A_SHARED_BACKSTAGE_WINDOW
    QPointer<QWidget> w = findWidgetByPos(pos);
    QApplicationPrivate::dispatchEnterLeave(w, m_overWidget, screenPos);
    m_overWidget = w;
    checkCursor(pos);
#else
    QGraphicsSceneHoverEvent hoverEvent(QEvent::GraphicsSceneHoverEnter);
    hoverEvent.setScenePos(pos - scene()->sceneRect().topLeft());
    hoverEvent.setScreenPos(screenPos);
    hoverEvent.setLastScenePos(hoverEvent.scenePos());
    hoverEvent.setLastScreenPos(hoverEvent.screenPos());
    hoverEvent.setWidget(viewWidget());

    qt_sendSpontaneousEvent(scene(), &hoverEvent);

    checkCursor(hoverEvent.scenePos().toPoint());
#endif
}

void ABackstageWidget::send_mouseOutEvent(const QPoint& pos)
{
    updateMousePressWidget(nullptr, Qt::NoButton);

    if (m_widget == nullptr || !m_widget->isEnabled())
        return;

#ifdef A_SHARED_BACKSTAGE_WINDOW
    Q_UNUSED(pos);
    QApplicationPrivate::dispatchEnterLeave(nullptr, m_overWidget, QPoint(-1, -1));
    m_overWidget = nullptr;
    checkCursor(pos);
#else
    QPoint screenPos = QCursor::pos();

    if (scene()->mouseGrabberItem() != nullptr)
        scene()->mouseGrabberItem()->ungrabMouse();

    QGraphicsSceneHoverEvent hoverEvent(QEvent::GraphicsSceneHoverLeave);
    hoverEvent.setScenePos(pos - scene()->sceneRect().topLeft());
    hoverEvent.setScreenPos(screenPos);
    hoverEvent.setLastScenePos(hoverEvent.scenePos());
    hoverEvent.setLastScreenPos(hoverEvent.screenPos());

    hoverEvent.setWidget(viewWidget());

    qt_sendSpontaneousEvent(scene(), &hoverEvent);
#endif
}

void ABackstageWidget::send_mouseMoveEvent(QMouseEvent* e)
{
    updateMousePressWidget(m_mousePressWidget, e->buttons());

    if (m_widget == nullptr || !m_widget->isEnabled())
    {
        e->ignore();
        return;
    }
#ifdef A_SHARED_BACKSTAGE_WINDOW
    QPointer<QWidget> w = m_mousePressWidget != nullptr ?
        m_mousePressWidget : findWidgetByPos(e->pos());
    QApplicationPrivate::dispatchEnterLeave(w, m_overWidget, e->screenPos());
    m_overWidget = w;
    if (w != nullptr)
    {
        QPoint offset = w->mapTo(m_widget, m_widget->geometry().topLeft());
        MouseEventQt mouseEvent(*e, e->pos() - offset);
        qt_sendSpontaneousEvent(w, &mouseEvent);
        bool isAccepted = mouseEvent.isAccepted();
        e->setAccepted(isAccepted);
    }
    checkCursor(e->pos());
#else
    QGraphicsSceneMouseEvent mouseEvent(QEvent::GraphicsSceneMouseMove);
    mouseEvent.setWidget(viewWidget());
    mouseEvent.setButtonDownScenePos(m_mousePressButton, m_mousePressPosition);
    mouseEvent.setButtonDownScreenPos(m_mousePressButton, m_mousePressScreenPosition);
    mouseEvent.setScenePos(e->pos() - scene()->sceneRect().topLeft());
    mouseEvent.setScreenPos(e->globalPos());
    mouseEvent.setLastScenePos(m_lastMouseMoveScenePosition);
    mouseEvent.setLastScreenPos(m_lastMouseMoveScreenPosition);
    mouseEvent.setButtons(e->buttons());
    mouseEvent.setButton(e->button());
    mouseEvent.setModifiers(e->modifiers());
#if (QT_VERSION >= QT_VERSION_CHECK(5, 4, 0))
    mouseEvent.setSource(e->source());
    mouseEvent.setFlags(e->flags());
#endif
    m_lastMouseMoveScenePosition = mouseEvent.scenePos();
    m_lastMouseMoveScreenPosition = mouseEvent.screenPos();
    mouseEvent.setAccepted(false);

    qt_sendSpontaneousEvent(scene(), &mouseEvent);

    bool isAccepted = mouseEvent.isAccepted();
    e->setAccepted(isAccepted);
    checkCursor(mouseEvent.scenePos().toPoint());
#endif
}

void ABackstageWidget::updateMousePressWidget(QWidget* w, Qt::MouseButtons buttons)
{
    if (m_mousePressWidget == nullptr)
    {
        m_mousePressWidget = w;
        m_mousePressButtons = w != nullptr ? buttons : Qt::NoButton;
        return;
    }

    if (m_mousePressWidget != w || m_mousePressButtons != buttons)
    {
        m_mousePressWidget = nullptr;
        m_mousePressButtons = Qt::NoButton;
        return;
    }
}

void ABackstageWidget::send_mousePressEvent(QMouseEvent* e)
{
    if (m_widget == nullptr || !m_widget->isEnabled())
    {
        e->ignore();
        return;
    }
    setGlobalPos(e->globalPos() + geometry().topLeft() - e->pos());


#ifdef A_SHARED_BACKSTAGE_WINDOW
    QWidget* w = findWidgetByPos(e->pos());
    if (w == nullptr)
        return;
    
    updateMousePressWidget(w, e->buttons());

    QPoint offset = w->mapTo(m_widget, m_widget->geometry().topLeft());

    MouseEventQt mouseEvent(*e, e->pos() - offset);
    mouseEvent.accept();
    qt_sendSpontaneousEvent(w, &mouseEvent);
    bool isAccepted = mouseEvent.isAccepted();
    e->setAccepted(isAccepted);
#else
    m_mousePressButton = e->button();
    m_mousePressPosition = e->pos() - scene()->sceneRect().topLeft();
    m_mousePressScreenPosition = e->globalPos();
    m_lastMouseMoveScenePosition = m_mousePressPosition;
    m_lastMouseMoveScreenPosition = m_mousePressScreenPosition;

    QGraphicsSceneMouseEvent mouseEvent(QEvent::GraphicsSceneMousePress);
    mouseEvent.setWidget(viewWidget());
    mouseEvent.setButtonDownScenePos(m_mousePressButton, m_mousePressPosition);
    mouseEvent.setButtonDownScreenPos(m_mousePressButton, m_mousePressScreenPosition);
    mouseEvent.setScenePos(m_mousePressPosition);
    mouseEvent.setScreenPos(m_mousePressScreenPosition);
    mouseEvent.setLastScenePos(m_lastMouseMoveScenePosition);
    mouseEvent.setLastScreenPos(m_lastMouseMoveScreenPosition);
    mouseEvent.setButtons(e->buttons());
    mouseEvent.setButton(e->button());
    mouseEvent.setModifiers(e->modifiers());
#if (QT_VERSION >= QT_VERSION_CHECK(5, 4, 0))
    mouseEvent.setSource(e->source());
    mouseEvent.setFlags(e->flags());
#endif
    mouseEvent.setAccepted(false);
    qt_sendSpontaneousEvent(scene(), &mouseEvent);
    bool isAccepted = mouseEvent.isAccepted();
    e->setAccepted(isAccepted);
#endif
}

void ABackstageWidget::send_mouseReleaseEvent(QMouseEvent* e)
{
    if (m_widget == nullptr || !m_widget->isEnabled())
    {
        updateMousePressWidget(nullptr, Qt::NoButton);
        e->ignore();
        return;
    }

    setGlobalPos(e->globalPos() + geometry().topLeft() - e->pos());

#ifdef A_SHARED_BACKSTAGE_WINDOW
    QPointer<QWidget> widgetUnderMouse = findWidgetByPos(e->pos());
    QPointer<QWidget> w = m_mousePressWidget != nullptr ?
        m_mousePressWidget : widgetUnderMouse.data();
    if (w != nullptr)
    {
        QPoint offset = w->mapTo(m_widget, m_widget->geometry().topLeft());
        MouseEventQt mouseEvent(*e, e->pos() - offset);
        mouseEvent.accept();
        qt_sendSpontaneousEvent(w, &mouseEvent);
        bool isAccepted = mouseEvent.isAccepted();
        e->setAccepted(isAccepted);
    }

    w = widgetUnderMouse;
    QApplicationPrivate::dispatchEnterLeave(w, m_overWidget, e->screenPos());
    m_overWidget = w;
    checkCursor(e->pos());
#else
    QGraphicsSceneMouseEvent mouseEvent(QEvent::GraphicsSceneMouseRelease);
    mouseEvent.setWidget(viewWidget());
    mouseEvent.setButtonDownScenePos(m_mousePressButton, m_mousePressPosition);
    mouseEvent.setButtonDownScreenPos(m_mousePressButton, m_mousePressScreenPosition);
    mouseEvent.setScenePos(e->pos() - scene()->sceneRect().topLeft());
    mouseEvent.setScreenPos(e->globalPos());
    mouseEvent.setLastScenePos(m_lastMouseMoveScenePosition);
    mouseEvent.setLastScreenPos(m_lastMouseMoveScreenPosition);
    mouseEvent.setButtons(e->buttons());
    mouseEvent.setButton(e->button());
    mouseEvent.setModifiers(e->modifiers());
#if (QT_VERSION >= QT_VERSION_CHECK(5, 4, 0))
    mouseEvent.setSource(e->source());
    mouseEvent.setFlags(e->flags());
#endif
    mouseEvent.setAccepted(false);

    qt_sendSpontaneousEvent(scene(), &mouseEvent);

    bool isAccepted = mouseEvent.isAccepted();
    e->setAccepted(isAccepted);
#endif
}

void ABackstageWidget::send_mouseDoubleClickEvent(QMouseEvent* e)
{
    if (m_widget == nullptr || !m_widget->isEnabled())
    {
        e->ignore();
        return;
    }

    setGlobalPos(e->globalPos() + geometry().topLeft() - e->pos());

#ifdef A_SHARED_BACKSTAGE_WINDOW
    QWidget* w = findWidgetByPos(e->pos());
    if (w == nullptr)
        return;
    QPoint offset = w->mapTo(m_widget, m_widget->geometry().topLeft());

    MouseEventQt mouseEvent(*e, e->pos() - offset);
    mouseEvent.accept();
    qt_sendSpontaneousEvent(w, &mouseEvent);
    bool isAccepted = mouseEvent.isAccepted();
    e->setAccepted(isAccepted);
#else
    m_mousePressButton = e->button();
    m_mousePressPosition = e->pos() - scene()->sceneRect().topLeft();
    m_mousePressScreenPosition = e->globalPos();
    m_lastMouseMoveScenePosition = m_mousePressPosition;
    m_lastMouseMoveScreenPosition = m_mousePressScreenPosition;

    QGraphicsSceneMouseEvent mouseEvent(QEvent::GraphicsSceneMouseDoubleClick);
    mouseEvent.setWidget(viewWidget());

    mouseEvent.setButtonDownScenePos(m_mousePressButton, m_mousePressPosition);
    mouseEvent.setButtonDownScreenPos(m_mousePressButton, m_mousePressScreenPosition);
    mouseEvent.setScenePos(m_mousePressPosition);
    mouseEvent.setScreenPos(m_mousePressScreenPosition);
    mouseEvent.setLastScenePos(m_lastMouseMoveScenePosition);
    mouseEvent.setLastScreenPos(m_lastMouseMoveScreenPosition);
    mouseEvent.setButtons(e->buttons());
    mouseEvent.setButton(e->button());
    mouseEvent.setModifiers(e->modifiers());
#if (QT_VERSION >= QT_VERSION_CHECK(5, 4, 0))
    mouseEvent.setSource(e->source());
    mouseEvent.setFlags(e->flags());
#endif
    mouseEvent.setAccepted(false);

    qt_sendSpontaneousEvent(scene(), &mouseEvent);

    const bool isAccepted = mouseEvent.isAccepted();
    e->setAccepted(isAccepted);
#endif
}

void ABackstageWidget::send_wheelEvent(QWheelEvent* e)
{
    if (m_widget == nullptr || !m_widget->isEnabled())
    {
        e->ignore();
        return;
    }
#ifdef A_SHARED_BACKSTAGE_WINDOW
#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
    QPoint pos = e->position().toPoint();
#else
    QPoint pos = e->pos();
#endif

    QWidget* w = findWidgetByPos(pos);
    if (w == nullptr)
        return;
    QPoint offset = w->mapTo(m_widget, m_widget->geometry().topLeft());

    WheelEventQt wheelEvent(*e, pos - offset);
    wheelEvent.accept(); 
#if 0 //MACOSX bug
    qt_sendSpontaneousEvent(w, &wheelEvent);
#endif
    QCoreApplication::sendEvent(w, &wheelEvent);
    bool isAccepted = wheelEvent.isAccepted();
    e->setAccepted(isAccepted);
#else
    QGraphicsSceneWheelEvent wheelEvent(QEvent::GraphicsSceneWheel);
    wheelEvent.setWidget(viewWidget());
    wheelEvent.setScenePos(e->pos() - geometry().topLeft());
    wheelEvent.setScreenPos(e->globalPos());
    wheelEvent.setButtons(e->buttons());
    wheelEvent.setModifiers(e->modifiers());
    wheelEvent.setDelta(e->delta());
    wheelEvent.setOrientation(e->orientation());
    wheelEvent.setAccepted(false);
    qt_sendSpontaneousEvent(scene(), &wheelEvent);
    e->setAccepted(wheelEvent.isAccepted());
#endif
}

void ABackstageWidget::send_keyPressEvent(QKeyEvent* e, const QPoint& globalPosition)
{
    if (m_widget == nullptr || !m_widget->isEnabled())
    {
        e->ignore();
        return;
    }

    setGlobalPos(globalPosition);

    const bool keyTab = !(e->modifiers() & (Qt::ControlModifier | Qt::AltModifier)) &&
         (e->key() == Qt::Key_Tab || e->key() == Qt::Key_Backtab
            /*|| (e->key() == Qt::Key_Tab && (e->modifiers() & Qt::ShiftModifier))*/);

    QWidget* oldfocus = m_widget->focusWidget();
    QWidget* w = keyTab ? widget() : aproch_get_focus_widget(widget());
    qt_sendSpontaneousEvent(w, e);
    if (keyTab && oldfocus == m_widget->focusWidget())
            e->ignore(); //focusNextPrevChild(bool next) actually returns false, 
                         //but event is accepted by default. We have to fix it. 
                         //See for details - QGraphicsScene::keyPressEvent(QKeyEvent *keyEvent)
}

void ABackstageWidget::send_keyReleaseEvent(QKeyEvent* e)
{
    if (m_widget == nullptr || !m_widget->isEnabled())
    {
        e->ignore();
        return;
    }
    QWidget* w = aproch_get_focus_widget(widget());
    qt_sendSpontaneousEvent(w, e);
}

void ABackstageWidget::send_shortcutEvent(QKeyEvent* e)
{
    if (m_widget == nullptr || !m_widget->isEnabled())
    {
        e->ignore();
        return;
    }

    e->ignore();
    QWidget* w = aproch_get_focus_widget(widget());
    qt_sendSpontaneousEvent(w, e);
    if (!e->isAccepted())
        tryShortcut(e);
}

void ABackstageWidget::send_shortcutOverrideEvent(QKeyEvent* e)
{
    send_shortcutEvent(e);
}

void ABackstageWidget::send_inputMethodEvent(QInputMethodEvent* e)
{
    if (m_widget == nullptr || !m_widget->isEnabled())
    {
        e->ignore();
        return;
    }
    QWidget* w = aproch_get_focus_widget(widget());
    qt_sendSpontaneousEvent(w, e);
}

void ABackstageWidget::send_focusInEvent(Qt::FocusReason reason)
{
#ifndef A_SHARED_BACKSTAGE_WINDOW
    activate();
#endif
    if (m_widget == nullptr || !m_widget->isEnabled())
        return;

    syncToView();
    QWidget* w = aproch_get_focus_widget(widget());
    if (w != nullptr)
        w->setFocus(reason);
}

void ABackstageWidget::send_focusOutEvent(Qt::FocusReason reason)
{
#ifndef A_SHARED_BACKSTAGE_WINDOW
    deactivate();
#endif
    if (m_widget == nullptr || !m_widget->isEnabled())
        return;

    if (scene() != nullptr)
        scene()->setFocusItem(nullptr, reason);
}

void ABackstageWidget::send_contextMenuEvent(QContextMenuEvent* e)
{
    if (m_widget == nullptr || !m_widget->isEnabled())
    {
        e->ignore();
        return;
    }
#ifdef A_SHARED_BACKSTAGE_WINDOW
    QWidget* w = findWidgetByPos(e->pos());
    if (w == nullptr)
        return;
    QPoint offset = w->mapTo(m_widget, m_widget->geometry().topLeft());

    ContextMenuEventQt contextEvent(*e);
    contextEvent.accept();
    contextEvent.setPos(e->pos() - offset);
    qt_sendSpontaneousEvent(w, &contextEvent);
    e->setAccepted(contextEvent.isAccepted());
#else
    QGraphicsSceneContextMenuEvent contextEvent(QEvent::GraphicsSceneContextMenu);
    contextEvent.setWidget(viewWidget());
    contextEvent.setScenePos(e->pos() - geometry().topLeft());
    contextEvent.setScreenPos(e->globalPos());
    contextEvent.setModifiers(e->modifiers());
    contextEvent.setReason((QGraphicsSceneContextMenuEvent::Reason)(e->reason()));
    contextEvent.setAccepted(e->isAccepted());
    qt_sendSpontaneousEvent(scene(), &contextEvent);
    e->setAccepted(contextEvent.isAccepted());
#endif
}

QCursor ABackstageWidget::cursor() const
{
    if (hasCursor())
        return m_cursorWidget->cursor();
    else
        return QCursor(Qt::ForbiddenCursor);
}

bool ABackstageWidget::hasCursor() const
{
    return m_cursorWidget != nullptr;
}

void ABackstageWidget::setGeometry(const QRect& geometry)
{
    m_geometry = geometry;
}

QRect ABackstageWidget::geometry() const
{
#ifndef A_SHARED_BACKSTAGE_WINDOW
    return scene()->sceneRect().toRect();
#else
    return m_geometry;
#endif
}

void ABackstageWidget::setGlobalPos(const QPoint& globalPos) const
{
    if (m_widget == nullptr)
        return;
    QPoint gp = globalPos;
#ifndef A_SHARED_BACKSTAGE_WINDOW
    gp += QPoint(margins().left(), margins().top());
#endif
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    QWindow* w = m_widget->window()->windowHandle();
    if (w != nullptr)
    {
        QRect r(gp, w->geometry().size());
        w->setGeometry(r);
#ifdef A_SHARED_BACKSTAGE_WINDOW
        m_widget->window()->setGeometry(r); //Bug: Its reset leaves widgets in bsp scene tree.
#endif
    }
#else
    aproch_setWidgetPostion(m_widget, gp);
#endif
}

APROCH_NAMESPACE_END