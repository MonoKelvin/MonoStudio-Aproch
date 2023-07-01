/****************************************************************************
**
** Qtitan Library by Developer Machines (Microsoft-Ribbon implementation for Qt.C++)
** 
** Copyright (c) 2009-2022 Developer Machines (https://www.devmachines.com)
**           ALL RIGHTS RESERVED
** 
**  The entire contents of this file is protected by copyright law and
**  international treaties. Unauthorized reproduction, reverse-engineering
**  and distribution of all or any portion of the code contained in this
**  file is strictly prohibited and may result in severe civil and 
**  criminal penalties and will be prosecuted to the maximum extent 
**  possible under the law.
**
**  RESTRICTIONS
**
**  THE SOURCE CODE CONTAINED WITHIN THIS FILE AND ALL RELATED
**  FILES OR ANY PORTION OF ITS CONTENTS SHALL AT NO TIME BE
**  COPIED, TRANSFERRED, SOLD, DISTRIBUTED, OR OTHERWISE MADE
**  AVAILABLE TO OTHER INDIVIDUALS WITHOUT WRITTEN CONSENT
**  AND PERMISSION FROM DEVELOPER MACHINES
**
**  CONSULT THE END USER LICENSE AGREEMENT FOR INFORMATION ON
**  ADDITIONAL RESTRICTIONS.
**
****************************************************************************/
#ifndef A_RIBBONBUTTONSCONTROLS_H
#define A_RIBBONBUTTONSCONTROLS_H

#include <QCheckBox>
#include <QToolButton>
#include <QRadioButton>
#include <QMenu>

#include "ARibbonControls.h"
#include "QtitanDef.h"


QTITAN_BEGIN_NAMESPACE

class RibbonButtonControlPrivate;
/* RibbonButtonControl */
class APROCH_API RibbonButtonControl : public RibbonWidgetControl
{
    Q_OBJECT
public:
    explicit RibbonButtonControl(ARibbonGroup* parent = nullptr);
    virtual ~RibbonButtonControl();
public:
    const QIcon& largeIcon() const;
    void setLargeIcon(const QIcon& icon);

    const QIcon& smallIcon() const;
    void setSmallIcon(const QIcon& icon);

    const QString& label() const;
    void setLabel(const QString& label);

    void setMenu(QMenu* menu);
    QMenu* menu() const;

    void setPopupMode(QToolButton::ToolButtonPopupMode mode);
    QToolButton::ToolButtonPopupMode popupMode() const;

    void setToolButtonStyle(Qt::ToolButtonStyle style);
    Qt::ToolButtonStyle toolButtonStyle() const;

    QToolButton* widget() const;
public:
    virtual QSize sizeHint() const;
    virtual void sizeChanged(ARibbonControlSizeDefinition::GroupSize size);
    virtual void actionChanged();
protected:
    virtual bool event(QEvent* event);
private:
    A_DECLARE_PRIVATE(RibbonButtonControl)
    Q_DISABLE_COPY(RibbonButtonControl)
};


/* RibbonCheckBoxControl */
class APROCH_API RibbonCheckBoxControl : public RibbonWidgetControl
{
public:
    explicit RibbonCheckBoxControl(const QString& text = QString(), ARibbonGroup* parent = nullptr);
    virtual ~RibbonCheckBoxControl();
public:
    QCheckBox* widget() const;
private:
    Q_DISABLE_COPY(RibbonCheckBoxControl)
};

/* RibbonRadioButtonControl */
class APROCH_API RibbonRadioButtonControl : public RibbonWidgetControl
{
public:
    explicit RibbonRadioButtonControl(const QString& text = QString(), ARibbonGroup* parent = nullptr);
    virtual ~RibbonRadioButtonControl();
public:
    QRadioButton* widget() const;
private:
    Q_DISABLE_COPY(RibbonRadioButtonControl)
};

QTITAN_END_NAMESPACE


#endif // A_RIBBONBUTTONSCONTROLS_H


