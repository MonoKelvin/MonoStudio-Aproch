#pragma once

#include <QtWidgets/QWidget>
#include "ui_WinUIWindow.h"

#include "AprochAPI.h"

class WinUIWindow : public aproch::AWindow
{
    Q_OBJECT

public:
    WinUIWindow(QWidget *parent = nullptr);
    ~WinUIWindow();

private:
    Ui::WinUIWindowClass ui;
};
