#pragma once

#include <QtWidgets/QWidget>
#include "ui_WinUIWindow.h"

#include "AprochAPI.h"

#include <QMenu>
#include <QDialog>

#ifdef _DEBUG
class ShadowWidget : public QDialog
{
    //Q_OBJECT;
public:
    ShadowWidget(QWidget* parent = nullptr);
    void paintEvent(QPaintEvent* evt);
};
#endif

class WinUIWindow : public aproch::AWindow
{
    Q_OBJECT

public:
    WinUIWindow(QWidget *parent = nullptr);
    ~WinUIWindow();

private:
    Ui::WinUIWindowClass ui;
};