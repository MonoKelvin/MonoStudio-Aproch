#pragma once

#include <QtWidgets/QWidget>
#include "ui_WinUIWindow.h"

#include "AprochAPI.h"

#include <QMenu>

class WinUIWindow : public aproch::AWindow
{
    Q_OBJECT

public:
    WinUIWindow(QWidget *parent = nullptr);
    ~WinUIWindow();

private:
    Ui::WinUIWindowClass ui;
};

class AMenu : public QMenu
{
    Q_OBJECT;
public:
    explicit AMenu(QWidget* parent = nullptr);
    explicit AMenu(const QString& title, QWidget* parent = nullptr);

protected:
    void showEvent(QShowEvent* evt) override;
};

class MyWidget : public QFrame
{
public:
    explicit MyWidget(QWidget* parent = nullptr)
        : QFrame(parent)
    {
    }
};
