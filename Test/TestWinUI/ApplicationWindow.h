#pragma once
#include "AprochAPI.h"

class ApplicationWindow : public aproch::AWindow
{
	Q_OBJECT
public:
	explicit ApplicationWindow(QWidget* parent = nullptr);
	~ApplicationWindow();

private:
	QWidget* mMainWidget = nullptr;
};
