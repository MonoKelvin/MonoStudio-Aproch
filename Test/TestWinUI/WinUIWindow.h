#pragma once

#include <QtWidgets/QWidget>
#include "ui_WinUIWindow.h"

#include "AprochAPI.h"

class WinUIWindow : public ARibbonMainWindow
{
    Q_OBJECT

public:
    WinUIWindow(QWidget *parent = nullptr);
    ~WinUIWindow();

private:
    void createRibbonBar();
    void createGroupClipboard(ARibbonPage* page);

private:
    Ui::WinUIWindowClass ui;
};
