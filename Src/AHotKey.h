/****************************************************************************
 * @file    AHotKey.h
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

#include "AprochGlobal.h"

#include <QObject>
#include <QKeySequence>
#include <QPair>
#include <QLoggingCategory>

APROCH_NAMESPACE_BEGIN

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#define AHOTKEY_HASH_SEED size_t
#else
#define AHOTKEY_HASH_SEED uint
#endif

/** @brief 系统热键定义 */
class APROCH_API AHotkey : public QObject
{
	Q_OBJECT;
	Q_PROPERTY(bool registered READ isRegistered WRITE setRegistered NOTIFY registeredChanged);
	Q_PROPERTY(QKeySequence shortcut READ getShortcut WRITE setShortcut RESET resetShortcut);
	A_DEFINE_PROPERTY(QString, name, Name);
	A_DEFINE_PROPERTY(QString, sequence, Sequence);
public:
	class APROCH_API NativeShortcut
	{
	public:
		quint32 key;
		quint32 modifier;

	public:
		NativeShortcut();
		NativeShortcut(quint32 key, quint32 modifier = 0);

		bool isValid() const;
		bool operator ==(NativeShortcut other) const;
		bool operator !=(NativeShortcut other) const;

	private:
		bool valid;
	};

	explicit AHotkey(QObject* parent = nullptr);
	explicit AHotkey(const QKeySequence& shortcut, bool autoRegister = false, QObject* parent = nullptr);
	explicit AHotkey(Qt::Key keyCode, Qt::KeyboardModifiers modifiers, bool autoRegister = false, QObject* parent = nullptr);
	explicit AHotkey(NativeShortcut shortcut, bool autoRegister = false, QObject* parent = nullptr);
	~AHotkey() override;

	bool isRegistered() const;
	QKeySequence getShortcut() const;
	Qt::Key getKeyCode() const;
	Qt::KeyboardModifiers getModifiers() const;
	NativeShortcut getCurrentNativeShortcut() const;

	static void addGlobalMapping(const QKeySequence& shortcut, NativeShortcut nativeShortcut);
	static bool isPlatformSupported();

public Q_SLOTS:
	bool setRegistered(bool registered);

	bool setShortcut(const QKeySequence& shortcut, bool autoRegister = false);
	bool setShortcut(Qt::Key keyCode, Qt::KeyboardModifiers modifiers, bool autoRegister = false);
	bool resetShortcut();

	bool setNativeShortcut(AHotkey::NativeShortcut nativeShortcut, bool autoRegister = false);

Q_SIGNALS:
	void activated(QPrivateSignal);
	void released(QPrivateSignal);
	void registeredChanged(bool registered);

private:
	friend class AHotkeyPrivate;

private:
	Qt::Key _keyCode;
	Qt::KeyboardModifiers _modifiers;
	NativeShortcut _nativeShortcut;
	bool _registered;
};

AHOTKEY_HASH_SEED APROCH_API qHash(AHotkey::NativeShortcut key);
AHOTKEY_HASH_SEED APROCH_API qHash(AHotkey::NativeShortcut key, AHOTKEY_HASH_SEED seed);

APROCH_API Q_DECLARE_LOGGING_CATEGORY(logAHotkey)

Q_DECLARE_METATYPE(AHotkey::NativeShortcut)

APROCH_NAMESPACE_END
