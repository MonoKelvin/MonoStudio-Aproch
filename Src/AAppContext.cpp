/****************************************************************************
 * @file    AAppContext.cpp
 * @date    2021-9-30
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
#include "stdafx.h"
#include "AAppContext.h"

#include <QApplication>
#include <QReadWriteLock>
#include <QProcess>
#include <QStyleFactory>

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

#include <Windows.h>

#elif defined(linux) || defined(__linux)

#include <string.h>
#include <unistd.h>
#include <dlfcn.h>

#endif // WINDOWS

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
static HMODULE _getSelfModuleHandle()
{
    MEMORY_BASIC_INFORMATION mbi;
    return (
        (::VirtualQuery(_getSelfModuleHandle, &mbi, sizeof(mbi)) != 0)
        ? (HMODULE)mbi.AllocationBase : NULL
        );
}

QString _getCurrentProgramDirectory()
{
    std::wstring strCurrentPath;
    wchar_t curDirector[260] = { 0 };
    GetModuleFileName(_getSelfModuleHandle(), curDirector, 260);
    strCurrentPath = curDirector;

    size_t nameStart = strCurrentPath.rfind(L"\\");
    strCurrentPath = strCurrentPath.substr(0, nameStart + 1);
    return QString::fromStdWString(strCurrentPath);
}

#elif defined(linux) || defined(__linux)
QString _getCurrentProgramDirectory()
{
    std::string strCurrentPath = "";
    char szCurWorkPath[256];
    memset(szCurWorkPath, '\0', 256);
    int nRet = readlink("/proc/self/exe", szCurWorkPath, 256);
    if (nRet > 256 || nRet < 0)
    {
        return QString::fromStdString(strCurrentPath);
    }

    for (int i = nRet; i > 0; i--)
    {
        if (szCurWorkPath[i] == '/' || szCurWorkPath[i] == '\\')
        {
            szCurWorkPath[i] = '\0';
            break;
        }
    }

    strCurrentPath = szCurWorkPath;

    return QString::fromStdString(strCurrentPath);
}
#endif

APROCH_NAMESPACE_BEGIN

#define A_D() const QScopedPointer<AAppContextPrivate>& d = AAppContext::getInstance()->d_ptr
APROCH_OBJECT_INIT_SINGLETON(AAppContext)

class AAppContextPrivate
{
public:
    QReadWriteLock rwLock;
    QMap<QString, QSharedPointer<IService>> serviceMap;
    QPointer<QWidget> mainWindow;

    bool handleProcess(int& step, int& totalStep, unsigned int added = 1)
    {
        if (step > totalStep)
            step = totalStep;

        bool canContinue = true;
        emit AAppContext::getInstance()->loading(step, totalStep, canContinue);
        step += added;
        return canContinue;
    }
};

AAppContext::AAppContext(QObject *parent)
    : QObject(parent)
    , d_ptr(new AAppContextPrivate)
{
}

AAppContext::~AAppContext()
{
}

QWidget* AAppContext::getMainWindow()
{
    A_D();
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    return d->mainWindow.get();
#else
    return d->mainWindow;
#endif
}

bool AAppContext::setMainWindow(QWidget* mainWindow)
{
    A_D();

    if (d->mainWindow)
    {
        qWarning() << "The application has already set the main window";
        return false;
    }
    if (!mainWindow)
    {
        qWarning() << "The main window cannot be set to null";
        return false;
    }

    d->mainWindow = mainWindow;

    return true;
}

int AAppContext::run(int argc, char* argv[])
{
    A_D();

    // 启动
    bool canContinue = true;
    emit AAppContext::getInstance()->launch(canContinue);
    if (!canContinue)
        return -1;  // todo: specific code

    int totalStep = 4;
    int step = 1;

    // 添加qt库路径
    QString appPath = _getCurrentProgramDirectory();
    QApplication::addLibraryPath(appPath + "qtplugins");

    // 设置程序相关属性
    QApplication::setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);
    QApplication::setEffectEnabled(Qt::UI_AnimateCombo, false); // 仅用默认动画，使用自定义动画

    // 启用高DPI缩放
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);

    // 设置控件样式
    aproch::AWinUIStyle* winUIStyle = new aproch::AWinUIStyle();
    QApplication::setStyle(winUIStyle);

    // 启动程序
    QApplication app(argc, argv);

    app.installEventFilter(AAppContext::getInstance());
    connect(AAppContext::getInstance(), &AAppContext::exit, &app, &QApplication::exit);

    // 加载插件
    APluginManager::getInstance()->loadPlugins();

    if (!d->handleProcess(step, totalStep))
        return -1;

    // 注册默认图标字体
    //AFontDatabase::addFont();

    // 注册默认的服务
    registerService<AKeyExpressionService>(AServiceName_KeyExpression);
    registerService<AAppConfigService>(AServiceName_AppConfig);
    registerService<ASqlDataBase>(AServiceName_DataBase);

    // 运行所有插件
    const auto &plugins = APluginManager::getInstance()->getPlugins();
    for (auto plg = plugins.begin(); plg != plugins.end(); ++plg)
    {
        Q_ASSERT(nullptr != plg.key());
        plg.key()->run();
    }

    if (!d->handleProcess(step, totalStep))
        return -1;

    // 初始化框架相关默认设置
    aproch::ATheme::setSystemThemeWatchEnabled(true);

    int retCode = 0;
    try
    {
        // 准备启动
        emit AAppContext::getInstance()->ready(canContinue);
        if (!canContinue)
            return -1;

        if (!d->handleProcess(step, totalStep))
            return -1;

        if (!getMainWindow())
        {
            qWarning() << "No startup window has been set up";
            return -1;
        }

        getMainWindow()->setAttribute(Qt::WA_DeleteOnClose);

        // 启动
        emit AAppContext::getInstance()->start();

        if (!d->handleProcess(step, totalStep))
            return -1;

        retCode = app.exec();
    }
    catch (...)
    {
    }

    return retCode;
}

QSharedPointer<IService> AAppContext::getService(const QString& name)
{
    A_D();

    d->rwLock.lockForRead();
    QSharedPointer<IService> service = d->serviceMap.value(name);
    d->rwLock.unlock();

    return service;
}

bool AAppContext::registerService(const QString& name, QSharedPointer<IService> service)
{
    if (name.trimmed().isEmpty() || nullptr == service)
        return false;

    A_D();

    d->rwLock.lockForWrite();
    //d->serviceMap.remove(serviceName);
    d->serviceMap.insert(name, service);
    d->rwLock.unlock();

    return true;
}

void AAppContext::unregisterService(const QString& name)
{
    A_D();

    d->rwLock.lockForRead();
    QSharedPointer<IService> service = d->serviceMap.value(name);
    d->rwLock.unlock();

    if (nullptr != service)
    {
        d->rwLock.lockForWrite();
        d->serviceMap.remove(name);
        d->rwLock.unlock();
    }
}

void AAppContext::restart()
{
    QApplication::exit(0);
    QProcess::startDetached("\"" + QApplication::applicationFilePath() + "\"", QStringList());
}

bool AAppContext::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == qApp)
    {
        if (event->type() == QEvent::Quit)
        {
            emit exit(0);
            return true;
        }
    }

    return QObject::eventFilter(watched, event);
}

APROCH_NAMESPACE_END
