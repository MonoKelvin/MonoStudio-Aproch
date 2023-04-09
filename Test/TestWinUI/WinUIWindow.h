#pragma once

#include <QtWidgets/QWidget>
#include "ui_WinUIWindow.h"

class WinUIWindow : public QWidget
{
    Q_OBJECT

public:
    WinUIWindow(QWidget *parent = nullptr);
    ~WinUIWindow();

private:
    Ui::WinUIWindowClass ui;
};
