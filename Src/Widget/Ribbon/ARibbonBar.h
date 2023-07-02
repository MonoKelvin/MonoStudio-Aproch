/****************************************************************************
 * @file    ARibbonBar.h
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
#pragma once

#include <QMenuBar>
#include <QMenu>

 // 
 // The most of the following code is copied from Qtitan.
 // 
 // Qtitan Library by Developer Machines(Microsoft - Ribbon implementation for Qt.C++)
 // Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
 // 

inline void initRibbonResource()
{
    Q_INIT_RESOURCE(RC_RibbonResources);
    Q_INIT_RESOURCE(RC_RibbonTranslations);
}

APROCH_NAMESPACE_BEGIN

class ARibbonPage;
class ARibbonBarPrivate;
class ARibbonSystemButton;
class ARibbonSystemMenu;
class ARibbonQuickAccessBar;
class ARibbonSearchBar;
class ARibbonCustomizeManager;
class ARibbonCustomizeDialog;

class APROCH_API ARibbonBar : public QMenuBar
{
    Q_OBJECT;
    Q_ENUMS(BarPosition);
    Q_PROPERTY(BarPosition quickAccessBarPosition READ quickAccessBarPosition WRITE setQuickAccessBarPosition);
    Q_PROPERTY(BarPosition tabBarPosition READ tabBarPosition WRITE setTabBarPosition);
    Q_PROPERTY(SearchBarAppearance searchBarAppearance READ searchBarAppearance WRITE setSearchBarAppearance);
    Q_PROPERTY(QPixmap titleBackground READ titleBackground WRITE setTitleBackground);
    Q_PROPERTY(int currentPageIndex READ currentPageIndex WRITE setCurrentPageIndex NOTIFY currentPageIndexChanged);
    Q_PROPERTY(bool minimized READ isMinimized WRITE setMinimized NOTIFY minimizationChanged);
    Q_PROPERTY(bool frameThemeEnabled READ isFrameThemeEnabled WRITE setFrameThemeEnabled NOTIFY frameThemeChanged DESIGNABLE true);
    Q_PROPERTY(bool acrilycEnabled READ isAcrilycEnabled WRITE setAcrilycEnabled);
    Q_PROPERTY(bool contextualTabsVisible READ isContextualTabsVisible WRITE setContextualTabsVisible);
    Q_PROPERTY(bool titleGroupsVisible READ isTitleGroupsVisible WRITE setTitleGroupsVisible);
    Q_PROPERTY(bool backstageVisible READ isBackstageVisible);
    Q_PROPERTY(bool simplifiedMode READ simplifiedMode WRITE setSimplifiedMode NOTIFY simplifiedModeChanged);
    Q_PROPERTY(bool movableTabs READ isMovableTabs WRITE setMovableTabs);
    Q_PROPERTY(Qt::TextElideMode tabsElideMode READ tabsElideMode WRITE setTabsElideMode);
public:
    enum BarPosition
    {
        TopPosition = 1,
        BottomPosition
    };

    enum SearchBarAppearance
    {
        SearchBarCentral = 1,
        SearchBarCompact,
        SearchBarHidden
    };

public:
    explicit ARibbonBar(QWidget* parent = nullptr);
    virtual ~ARibbonBar();

public:
    ARibbonPage* addPage(const QString& text);
    void addPage(ARibbonPage* page);
    ARibbonPage* insertPage(int index, const QString& text);
    void insertPage(int index, ARibbonPage* page);

    void movePage(ARibbonPage* page, int newIndex);
    void movePage(int index, int newIndex);

    void removePage(ARibbonPage* page);
    void removePage(int index);

    void detachPage(ARibbonPage* page);
    void detachPage(int index);

    void clearPages();

    ARibbonPage* page(int index) const;
    int pageCount() const;
    int currentPageIndex() const;
    ARibbonPage* currentPage() const;
    QList<ARibbonPage *> pages() const;
    int pageIndex(ARibbonPage* page) const;

    bool isKeyTipsShowing() const;
    bool keyTipsEnabled() const;
    void setKeyTipsEnable(bool enable);
    bool isKeyTipsComplement() const;
    void setKeyTipsComplement(bool complement);
    void setKeyTip(QAction* action, const QString& keyTip);

    bool isMovableTabs() const;
    void setMovableTabs(bool movable);

    Qt::TextElideMode tabsElideMode() const;
    void setTabsElideMode(Qt::TextElideMode mode);

    void setLogoPixmap(const QPixmap& pixmap, Qt::AlignmentFlag alignment);
    QPixmap logoPixmap() const;

    void setTitleBackground(const QPixmap& pixmap);
    const QPixmap& titleBackground() const;

    ARibbonQuickAccessBar* quickAccessBar() const;
    ARibbonSearchBar* searchBar() const;

    void setQuickAccessBarPosition(BarPosition position);
    BarPosition quickAccessBarPosition() const;

    void setSearchBarAppearance(SearchBarAppearance appearance);
    SearchBarAppearance searchBarAppearance() const;

    void setTabBarPosition(BarPosition position);
    BarPosition tabBarPosition() const;

    void minimize();
    bool isMinimized() const;
    void setMinimized(bool flag);
        
    void maximize();
    bool isMaximized() const;
    void setMaximized(bool flag);

    void setMinimizationEnabled(bool enabled);
    bool isMinimizationEnabled() const;

    void visualRibbonBarView(bool bVisble);

    /*
    * @brief 窄模式
    * @return 是否是窄模式
    */
    bool getNarrowMode() const;

    /*
    * @brief 设置窄模式
    * @param [in] bNarrow 窄模式
    */
    void setNarrowMode(bool bNarrow = true);

    /**
    * @brief 获取自动简化模式
    * @return bool 是否自动简化
    */
    bool getAutoReduceMode();

    /**
    * @brief 设置自动简化模式
    * @param [in] bReduce 简化模式
    */
    void setAutoReduceMode(bool bReduce = true);

    QAction* simplifiedAction() const;

    bool simplifiedMode() const;
    void setSimplifiedMode(bool flag);

    bool simplifiedModeEnabled() const;
    void setSimplifiedModeEnabled(bool enabled);

    Qt::LayoutDirection expandDirection() const;
    void setExpandDirection(Qt::LayoutDirection direction);

    ARibbonCustomizeManager* customizeManager();
    ARibbonCustomizeDialog* customizeDialog();

    QAction* addTitleButton(const QIcon& icon, const QString& help);
    void removeTitleButton(QAction* action);

    QMenu* addMenu(const QString& text);

    using QWidget::addAction;
    QAction* addAction(const QIcon& icon, const QString& text, Qt::ToolButtonStyle style, QMenu* menu = nullptr);
    QAction* addAction(QAction* action, Qt::ToolButtonStyle style);
    QAction* addSystemButton(const QString& text);
    QAction* addSystemButton(const QIcon& icon, const QString& text);

    ARibbonSystemButton* systemButton() const;

    bool isFrameThemeEnabled() const;
    bool isAcrilycEnabled() const;
    bool isContextualTabsVisible() const;
    bool isTitleGroupsVisible() const;

    int rowItemHeight() const;
    int rowItemCount() const;
    bool isBackstageVisible() const;
    void updateLayout();
    void beginUpdate();
    void endUpdate();

    static bool loadTranslation(const QString& country = QString()); //Format de_de, fr_fr, ru_ru
    static QString tr_compatible(const char* s, const char* c = nullptr);

public:
    void setVisible(bool visible) override;
    int heightForWidth(int) const override;
    QSize sizeHint() const override;

public Q_SLOTS:
    void setCurrentPageIndex(int index);
    void showCustomizeDialog();
    void setFrameThemeEnabled(bool enabled = true);
    void setAcrilycEnabled(bool enabled);
    void setContextualTabsVisible(bool visible);
    void setTitleGroupsVisible(bool visible);

Q_SIGNALS:
    void simplifiedModeChanged(bool enabled);
    void minimizationChanged(bool minimized);
    void pageAboutToBeChanged(ARibbonPage* page, bool& changed);
    void currentPageIndexChanged(int index);
    void currentPageChanged(ARibbonPage* page);
    void keyTipsShowed(bool showed);
    void showRibbonContextMenu(QMenu* menu, QContextMenuEvent* event);
    void frameThemeChanged(bool enabled);

    /**
    * 打开文件
    * @param [in] strFile 文件路径
    */
    void openRecentFile(const QString& strFile);

protected:
    void setMinimizedFlag(bool flag);

protected:
    virtual bool eventFilter(QObject* object, QEvent* event) override;
    virtual bool event(QEvent* event) override;
    virtual void paintEvent(QPaintEvent* p) override;
    virtual void changeEvent(QEvent* event) override;
    virtual void resizeEvent(QResizeEvent* event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void wheelEvent(QWheelEvent* event) override;
    virtual void contextMenuEvent(QContextMenuEvent* event) override;

private:
    friend class ARibbonPage;
    friend class ARibbonBackstageViewPrivate;
    friend class ARibbonBackstageViewMenu;
    friend class ARibbonBarPrivate;
    A_DECLARE_PRIVATE(ARibbonBar);
    Q_DISABLE_COPY(ARibbonBar);
};

class ARibbonBarAutoUpdater
{
public:
    ARibbonBarAutoUpdater(ARibbonBar* ribbonBar)
        : m_ribbonBar(ribbonBar)
    {
        if (m_ribbonBar != nullptr)
            m_ribbonBar->beginUpdate();
    }

    ~ARibbonBarAutoUpdater()
    {
        if (m_ribbonBar != nullptr)
            m_ribbonBar->endUpdate();
    }
private:
    ARibbonBar* m_ribbonBar;
};

APROCH_NAMESPACE_END
