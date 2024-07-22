#include "stdafx.h"
#include "AHotKey.h"
#include "Private/AHotKey_p.h"

#include <QCoreApplication>
#include <QAbstractEventDispatcher>
#include <QMetaMethod>
#include <QThread>
#include <QDebug>

APROCH_NAMESPACE_BEGIN

Q_LOGGING_CATEGORY(logAHotkey, "AHotkey")

void AHotkey::addGlobalMapping(const QKeySequence& shortcut, AHotkey::NativeShortcut nativeShortcut)
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    const int key = shortcut[0].toCombined();
#else
    const int key = shortcut[0];
#endif

    QMetaObject::invokeMethod(AHotkeyPrivate::getInstance(), "addMappingInvoked", Qt::QueuedConnection,
                              Q_ARG(Qt::Key, Qt::Key(key & ~Qt::KeyboardModifierMask)),
                              Q_ARG(Qt::KeyboardModifiers, Qt::KeyboardModifiers(key & Qt::KeyboardModifierMask)),
                              Q_ARG(AHotkey::NativeShortcut, nativeShortcut));
}

bool AHotkey::isPlatformSupported()
{
    return AHotkeyPrivate::isPlatformSupported();
}

AHotkey::AHotkey(QObject* parent) 
    : QObject(parent)
    , _keyCode(Qt::Key_unknown)
    , _modifiers(Qt::NoModifier)
    , _registered(false)
{
    // 设置为系统热键
    if (qApp)
        setParent(qApp);

    connect(this, &AHotkey::sequenceChanged, this, [this](const QString& seq) {
        if (qApp)
            setParent(qApp);

        setShortcut(QKeySequence(seq), true);
    });
}

AHotkey::AHotkey(const QKeySequence& shortcut, bool autoRegister, QObject* parent)
    : AHotkey(parent)
{
    setShortcut(shortcut, autoRegister);
}

AHotkey::AHotkey(Qt::Key keyCode, Qt::KeyboardModifiers modifiers, bool autoRegister, QObject* parent)
    : AHotkey(parent)
{
    setShortcut(keyCode, modifiers, autoRegister);
}

AHotkey::AHotkey(AHotkey::NativeShortcut shortcut, bool autoRegister, QObject* parent)
    : AHotkey(parent)
{
    setNativeShortcut(shortcut, autoRegister);
}

AHotkey::~AHotkey()
{
    if (_registered)
        AHotkeyPrivate::getInstance()->removeShortcut(this);
}

QKeySequence AHotkey::getShortcut() const
{
    if (_keyCode == Qt::Key_unknown)
        return QKeySequence();

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    return QKeySequence((_keyCode | _modifiers).toCombined());
#else
    return QKeySequence(static_cast<int>(_keyCode | _modifiers));
#endif
}

Qt::Key AHotkey::getKeyCode() const
{
    return _keyCode;
}

Qt::KeyboardModifiers AHotkey::getModifiers() const
{
    return _modifiers;
}

AHotkey::NativeShortcut AHotkey::getCurrentNativeShortcut() const
{
    return _nativeShortcut;
}

bool AHotkey::isRegistered() const
{
    return _registered;
}

bool AHotkey::setShortcut(const QKeySequence& shortcut, bool autoRegister)
{
    if (shortcut.isEmpty())
        return resetShortcut();
    if (shortcut.count() > 1)
    {
        qCWarning(logAHotkey, "Keysequences with multiple shortcuts are not allowed! "
                  "Only the first shortcut will be used!");
    }

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    const int key = shortcut[0].toCombined();
#else
    const int key = shortcut[0];
#endif

    return setShortcut(Qt::Key(key & ~Qt::KeyboardModifierMask),
                       Qt::KeyboardModifiers(key & Qt::KeyboardModifierMask),
                       autoRegister);
}

bool AHotkey::setShortcut(Qt::Key keyCode, Qt::KeyboardModifiers modifiers, bool autoRegister)
{
    if (_registered)
    {
        if (autoRegister)
        {
            if (!AHotkeyPrivate::getInstance()->removeShortcut(this))
                return false;
        }
        else
            return false;
    }

    if (keyCode == Qt::Key_unknown)
    {
        _keyCode = Qt::Key_unknown;
        _modifiers = Qt::NoModifier;
        _nativeShortcut = NativeShortcut();
        return true;
    }

    _keyCode = keyCode;
    _modifiers = modifiers;
    _nativeShortcut = AHotkeyPrivate::getInstance()->nativeShortcut(keyCode, modifiers);
    if (_nativeShortcut.isValid())
    {
        if (autoRegister)
            return AHotkeyPrivate::getInstance()->addShortcut(this);
        return true;
    }

    qCWarning(logAHotkey) << "Unable to map shortcut to native keys. Key:" << keyCode << "Modifiers:" << modifiers;
    _keyCode = Qt::Key_unknown;
    _modifiers = Qt::NoModifier;
    _nativeShortcut = NativeShortcut();
    return false;
}

bool AHotkey::resetShortcut()
{
    if (_registered &&
        !AHotkeyPrivate::getInstance()->removeShortcut(this))
    {
        return false;
    }

    _keyCode = Qt::Key_unknown;
    _modifiers = Qt::NoModifier;
    _nativeShortcut = NativeShortcut();
    return true;
}

bool AHotkey::setNativeShortcut(AHotkey::NativeShortcut nativeShortcut, bool autoRegister)
{
    if (_registered)
    {
        if (autoRegister)
        {
            if (!AHotkeyPrivate::getInstance()->removeShortcut(this))
                return false;
        }
        else
            return false;
    }

    if (nativeShortcut.isValid())
    {
        _keyCode = Qt::Key_unknown;
        _modifiers = Qt::NoModifier;
        _nativeShortcut = nativeShortcut;
        if (autoRegister)
            return AHotkeyPrivate::getInstance()->addShortcut(this);
        return true;
    }

    _keyCode = Qt::Key_unknown;
    _modifiers = Qt::NoModifier;
    _nativeShortcut = NativeShortcut();
    return true;
}

bool AHotkey::setRegistered(bool registered)
{
    if (_registered && !registered)
        return AHotkeyPrivate::getInstance()->removeShortcut(this);
    if (!_registered && registered)
    {
        if (!_nativeShortcut.isValid())
            return false;
        return AHotkeyPrivate::getInstance()->addShortcut(this);
    }
    return true;
}



// ---------- AHotkeyPrivate implementation ----------

AHotkeyPrivate::AHotkeyPrivate()
{
    Q_ASSERT_X(qApp, Q_FUNC_INFO, "AHotkey requires QCoreApplication to be instantiated");
    qApp->eventDispatcher()->installNativeEventFilter(this);
}

AHotkeyPrivate::~AHotkeyPrivate()
{
    if (!shortcuts.isEmpty())
        qCWarning(logAHotkey) << "AHotkeyPrivate destroyed with registered shortcuts!";
    if (qApp && qApp->eventDispatcher())
        qApp->eventDispatcher()->removeNativeEventFilter(this);
}

AHotkey::NativeShortcut AHotkeyPrivate::nativeShortcut(Qt::Key keycode, Qt::KeyboardModifiers modifiers)
{
    Qt::ConnectionType conType = (QThread::currentThread() == thread() ?
                                  Qt::DirectConnection :
                                  Qt::BlockingQueuedConnection);
    AHotkey::NativeShortcut res;
    if (!QMetaObject::invokeMethod(this, "nativeShortcutInvoked", conType,
        Q_RETURN_ARG(AHotkey::NativeShortcut, res),
        Q_ARG(Qt::Key, keycode),
        Q_ARG(Qt::KeyboardModifiers, modifiers)))
    {
        return AHotkey::NativeShortcut();
    }
    return res;
}

bool AHotkeyPrivate::addShortcut(AHotkey* hotkey)
{
    if (hotkey->_registered)
        return false;

    Qt::ConnectionType conType = (QThread::currentThread() == thread() ?
                                  Qt::DirectConnection :
                                  Qt::BlockingQueuedConnection);
    bool res = false;
    if (!QMetaObject::invokeMethod(this, "addShortcutInvoked", conType,
        Q_RETURN_ARG(bool, res),
        Q_ARG(AHotkey*, hotkey)))
    {
        return false;
    }

    if (res)
        emit hotkey->registeredChanged(true);
    return res;
}

bool AHotkeyPrivate::removeShortcut(AHotkey* hotkey)
{
    if (!hotkey->_registered)
        return false;

    Qt::ConnectionType conType = (QThread::currentThread() == thread() ?
                                  Qt::DirectConnection :
                                  Qt::BlockingQueuedConnection);
    bool res = false;
    if (!QMetaObject::invokeMethod(this, "removeShortcutInvoked", conType,
        Q_RETURN_ARG(bool, res),
        Q_ARG(AHotkey*, hotkey)))
    {
        return false;
    }

    if (res)
        emit hotkey->registeredChanged(false);
    return res;
}

void AHotkeyPrivate::activateShortcut(AHotkey::NativeShortcut shortcut)
{
    QMetaMethod signal = QMetaMethod::fromSignal(&AHotkey::activated);
    for (AHotkey* hkey : shortcuts.values(shortcut))
        signal.invoke(hkey, Qt::QueuedConnection);
}

void AHotkeyPrivate::releaseShortcut(AHotkey::NativeShortcut shortcut)
{
    QMetaMethod signal = QMetaMethod::fromSignal(&AHotkey::released);
    for (AHotkey* hkey : shortcuts.values(shortcut))
        signal.invoke(hkey, Qt::QueuedConnection);
}

void AHotkeyPrivate::addMappingInvoked(Qt::Key keycode, Qt::KeyboardModifiers modifiers, AHotkey::NativeShortcut nativeShortcut)
{
    mapping.insert({ keycode, modifiers }, nativeShortcut);
}

bool AHotkeyPrivate::addShortcutInvoked(AHotkey* hotkey)
{
    AHotkey::NativeShortcut shortcut = hotkey->_nativeShortcut;

    if (!shortcuts.contains(shortcut))
    {
        if (!registerShortcut(shortcut))
        {
            qCWarning(logAHotkey) << AHotkey::tr("Failed to register %1. Error: %2").arg(hotkey->getShortcut().toString(), error);
            return false;
        }
    }

    shortcuts.insert(shortcut, hotkey);
    hotkey->_registered = true;
    return true;
}

bool AHotkeyPrivate::removeShortcutInvoked(AHotkey* hotkey)
{
    AHotkey::NativeShortcut shortcut = hotkey->_nativeShortcut;

    if (shortcuts.remove(shortcut, hotkey) == 0)
        return false;
    hotkey->_registered = false;
    emit hotkey->registeredChanged(true);
    if (shortcuts.count(shortcut) == 0)
    {
        if (!unregisterShortcut(shortcut))
        {
            qCWarning(logAHotkey) << AHotkey::tr("Failed to unregister %1. Error: %2").arg(hotkey->getShortcut().toString(), error);
            return false;
        }
        return true;
    }
    return true;
}

AHotkey::NativeShortcut AHotkeyPrivate::nativeShortcutInvoked(Qt::Key keycode, Qt::KeyboardModifiers modifiers)
{
    if (mapping.contains({ keycode, modifiers }))
        return mapping.value({ keycode, modifiers });

    bool ok1 = false;
    auto k = nativeKeycode(keycode, ok1);
    bool ok2 = false;
    auto m = nativeModifiers(modifiers, ok2);
    if (ok1 && ok2)
        return { k, m };
    return {};
}



AHotkey::NativeShortcut::NativeShortcut()
    : key(), modifier(), valid(false)
{
}

AHotkey::NativeShortcut::NativeShortcut(quint32 key, quint32 modifier)
    : key(key), modifier(modifier), valid(true)
{
}

bool AHotkey::NativeShortcut::isValid() const
{
    return valid;
}

bool AHotkey::NativeShortcut::operator ==(AHotkey::NativeShortcut other) const
{
    return (key == other.key) && (modifier == other.modifier) && valid == other.valid;
}

bool AHotkey::NativeShortcut::operator !=(AHotkey::NativeShortcut other) const
{
    return (key != other.key) || (modifier != other.modifier) || valid != other.valid;
}

AHOTKEY_HASH_SEED qHash(AHotkey::NativeShortcut key)
{
    return qHash(key.key) ^ qHash(key.modifier);
}

AHOTKEY_HASH_SEED qHash(AHotkey::NativeShortcut key, AHOTKEY_HASH_SEED seed)
{
    return qHash(key.key, seed) ^ qHash(key.modifier, seed);
}

APROCH_NAMESPACE_END
