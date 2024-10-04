#include "stdafx.h"
#include "AWinUIApplication.h"

#include "QWindowKit/QWKQuick/qwkquickglobal.h"

#include <QTranslator>
#include <QGuiApplication>
#include <QQmlEngine>
#include <QtQuick/QQuickWindow>

APROCH_NAMESPACE_BEGIN

static AWinUIApplication* inst = nullptr;

class AWinUIApplicationPrivate
{
public:
    AWinUIApplicationPrivate(int& argc, char** argv);

    const int major = 1;
    const int minor = 0;
    const char* uri = "Aproch.WinUI";

    int& argc;
    char** argv = nullptr;
    QQmlEngine* engine = nullptr;
    QTranslator* translator = nullptr;
};

AWinUIApplicationPrivate::AWinUIApplicationPrivate(int& argc, char** argv)
    : argc(argc)
    , argv(argv)
{
    if (qApp)
    {
#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
        qFatal() << AStr("Cannot create other QCoreApplication instance");
#else
        qFatal("Cannot create other QCoreApplication instance");
#endif
        return;
    }

#ifdef WIN32
    //::SetUnhandledExceptionFilter(MyUnhandledExceptionFilter);
    qputenv("QT_QPA_PLATFORM", "windows:darkmode=2");
#endif
#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
    qputenv("QT_QUICK_CONTROLS_STYLE", "Basic");
#else
    qputenv("QT_QUICK_CONTROLS_STYLE", "Default");
#endif
#ifdef Q_OS_LINUX
    //fix bug UOSv20 does not print logs
    qputenv("QT_LOGGING_RULES", "");
    //fix bug UOSv20 v-sync does not work
    qputenv("QSG_RENDER_LOOP", "basic");
#endif

    QGuiApplication::setOrganizationName(AStr("Mono Studio"));
    QGuiApplication::setOrganizationDomain("https://github.com/monokelvin");
    QGuiApplication::setApplicationName(AStr("Aproch"));
    QGuiApplication::setApplicationDisplayName(AStr("Aproch"));
    QGuiApplication::setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);

#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);
#endif
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
#endif
#endif
}

////////////////////////////////////////////////////////////////////////////////

AWinUIApplication::AWinUIApplication(int& argc, char** argv)
    : QObject(nullptr)
    , d(new AWinUIApplicationPrivate(argc, argv))
    , m_isUseSystemAppBar(false)
    , m_context(nullptr)
{
    if (inst)
        throw AException("Only one AWinUIApplication instance can be created");

    inst = this;
}

AWinUIApplication::~AWinUIApplication()
{
}

AWinUIApplication* AWinUIApplication::getInstance()
{
    return inst;
}

AWinUIApplication* AWinUIApplication::create(QQmlEngine*, QJSEngine*)
{
    return inst;
}

void AWinUIApplication::init(QObject* qmlContext, const QLocale& locale)
{
    if (this->context())
    {
        qWarning() << AStr("WinUI application has already been initialized");
        return;
    }

    this->context(qmlContext);
    this->locale(locale);
    d->engine = qmlEngine(qmlContext);
    d->translator = new QTranslator(this);

    QGuiApplication::installTranslator(d->translator);

    const QStringList uiLanguages = m_locale.uiLanguages();
    for (const QString& name : uiLanguages)
    {
        const QString baseName = QLocale(name).name();
        if (d->translator->load(":/winui/i18n/" + baseName))
        {
            d->engine->retranslate();
            break;
        }
    }
}

int AWinUIApplication::exec(const QUrl& url)
{
    QGuiApplication app(d->argc, d->argv);
    QQmlApplicationEngine engine;

    QWK::registerTypes(&engine);

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app, [url](QObject* obj, const QUrl& objUrl) 
    {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    engine.load(url);

    const int exec = app.exec();
    if (exec == 931)
    {
        // 重启
        // QProcess::startDetached(qApp->applicationFilePath(), qApp->arguments());
    }

    return exec;
}

APROCH_NAMESPACE_END
