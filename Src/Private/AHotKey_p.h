/****************************************************************************
 * @file    AHotKey_p.h
 * @date    2024-06-09 
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

#include "AHotKey.h"
#include <QAbstractNativeEventFilter>
#include <QMultiHash>
#include <QMutex>
#include <QGlobalStatic>

APROCH_NAMESPACE_BEGIN

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#define _NATIVE_EVENT_RESULT qintptr
#else
#define _NATIVE_EVENT_RESULT long
#endif

class APROCH_API AHotkeyPrivate : public QObject, public QAbstractNativeEventFilter
{
	Q_OBJECT;
public:
	AHotkeyPrivate();
	~AHotkeyPrivate();

	static AHotkeyPrivate* instance();
	static bool isPlatformSupported();

	AHotkey::NativeShortcut nativeShortcut(Qt::Key keycode, Qt::KeyboardModifiers modifiers);

	bool addShortcut(AHotkey* hotkey);
	bool removeShortcut(AHotkey* hotkey);

protected:
	void activateShortcut(AHotkey::NativeShortcut shortcut);
	void releaseShortcut(AHotkey::NativeShortcut shortcut);

	virtual quint32 nativeKeycode(Qt::Key keycode, bool& ok) = 0;//platform implement
	virtual quint32 nativeModifiers(Qt::KeyboardModifiers modifiers, bool& ok) = 0;//platform implement

	virtual bool registerShortcut(AHotkey::NativeShortcut shortcut) = 0;//platform implement
	virtual bool unregisterShortcut(AHotkey::NativeShortcut shortcut) = 0;//platform implement

	QString error;

private:
	QHash<QPair<Qt::Key, Qt::KeyboardModifiers>, AHotkey::NativeShortcut> mapping;
	QMultiHash<AHotkey::NativeShortcut, AHotkey*> shortcuts;

	Q_INVOKABLE void addMappingInvoked(Qt::Key keycode, Qt::KeyboardModifiers modifiers, AHotkey::NativeShortcut nativeShortcut);
	Q_INVOKABLE bool addShortcutInvoked(AHotkey* hotkey);
	Q_INVOKABLE bool removeShortcutInvoked(AHotkey* hotkey);
	Q_INVOKABLE AHotkey::NativeShortcut nativeShortcutInvoked(Qt::Key keycode, Qt::KeyboardModifiers modifiers);
};

#define NATIVE_INSTANCE(ClassName) \
	Q_GLOBAL_STATIC(ClassName, hotkeyPrivate) \
	AHotkeyPrivate *AHotkeyPrivate::instance()\
	{\
		return hotkeyPrivate;\
	}

APROCH_NAMESPACE_END
