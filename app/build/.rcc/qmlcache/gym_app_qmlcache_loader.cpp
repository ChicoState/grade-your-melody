#include <QtQml/qqmlprivate.h>
#include <QtCore/qdir.h>
#include <QtCore/qurl.h>
#include <QtCore/qhash.h>
#include <QtCore/qstring.h>

namespace QmlCacheGeneratedCode {
namespace _qt_qml_GradeYourMelodyUI_App_qml { 
    extern const unsigned char qmlData[];
    extern const QQmlPrivate::AOTCompiledFunction aotBuiltFunctions[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), &aotBuiltFunctions[0], nullptr
    };
}
namespace _qt_qml_GradeYourMelodyUI_NoteSlot_qml { 
    extern const unsigned char qmlData[];
    extern const QQmlPrivate::AOTCompiledFunction aotBuiltFunctions[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), &aotBuiltFunctions[0], nullptr
    };
}
namespace _qt_qml_GradeYourMelodyUI_Screen01_qml { 
    extern const unsigned char qmlData[];
    extern const QQmlPrivate::AOTCompiledFunction aotBuiltFunctions[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), &aotBuiltFunctions[0], nullptr
    };
}
namespace _qt_qml_GradeYourMelodyUI_GradeYourMelodyUI_Constants_qml { 
    extern const unsigned char qmlData[];
    extern const QQmlPrivate::AOTCompiledFunction aotBuiltFunctions[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), &aotBuiltFunctions[0], nullptr
    };
}
namespace _qt_qml_GradeYourMelodyUI_GradeYourMelodyUI_EventListModel_qml { 
    extern const unsigned char qmlData[];
    extern const QQmlPrivate::AOTCompiledFunction aotBuiltFunctions[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), &aotBuiltFunctions[0], nullptr
    };
}
namespace _qt_qml_GradeYourMelodyUI_GradeYourMelodyUI_EventListSimulator_qml { 
    extern const unsigned char qmlData[];
    extern const QQmlPrivate::AOTCompiledFunction aotBuiltFunctions[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), &aotBuiltFunctions[0], nullptr
    };
}

}
namespace {
struct Registry {
    Registry();
    ~Registry();
    QHash<QString, const QQmlPrivate::CachedQmlUnit*> resourcePathToCachedUnit;
    static const QQmlPrivate::CachedQmlUnit *lookupCachedUnit(const QUrl &url);
};

Q_GLOBAL_STATIC(Registry, unitRegistry)


Registry::Registry() {
    resourcePathToCachedUnit.insert(QStringLiteral("/qt/qml/GradeYourMelodyUI/App.qml"), &QmlCacheGeneratedCode::_qt_qml_GradeYourMelodyUI_App_qml::unit);
    resourcePathToCachedUnit.insert(QStringLiteral("/qt/qml/GradeYourMelodyUI/NoteSlot.qml"), &QmlCacheGeneratedCode::_qt_qml_GradeYourMelodyUI_NoteSlot_qml::unit);
    resourcePathToCachedUnit.insert(QStringLiteral("/qt/qml/GradeYourMelodyUI/Screen01.qml"), &QmlCacheGeneratedCode::_qt_qml_GradeYourMelodyUI_Screen01_qml::unit);
    resourcePathToCachedUnit.insert(QStringLiteral("/qt/qml/GradeYourMelodyUI/GradeYourMelodyUI/Constants.qml"), &QmlCacheGeneratedCode::_qt_qml_GradeYourMelodyUI_GradeYourMelodyUI_Constants_qml::unit);
    resourcePathToCachedUnit.insert(QStringLiteral("/qt/qml/GradeYourMelodyUI/GradeYourMelodyUI/EventListModel.qml"), &QmlCacheGeneratedCode::_qt_qml_GradeYourMelodyUI_GradeYourMelodyUI_EventListModel_qml::unit);
    resourcePathToCachedUnit.insert(QStringLiteral("/qt/qml/GradeYourMelodyUI/GradeYourMelodyUI/EventListSimulator.qml"), &QmlCacheGeneratedCode::_qt_qml_GradeYourMelodyUI_GradeYourMelodyUI_EventListSimulator_qml::unit);
    QQmlPrivate::RegisterQmlUnitCacheHook registration;
    registration.structVersion = 0;
    registration.lookupCachedQmlUnit = &lookupCachedUnit;
    QQmlPrivate::qmlregister(QQmlPrivate::QmlUnitCacheHookRegistration, &registration);
}

Registry::~Registry() {
    QQmlPrivate::qmlunregister(QQmlPrivate::QmlUnitCacheHookRegistration, quintptr(&lookupCachedUnit));
}

const QQmlPrivate::CachedQmlUnit *Registry::lookupCachedUnit(const QUrl &url) {
    if (url.scheme() != QLatin1String("qrc"))
        return nullptr;
    QString resourcePath = QDir::cleanPath(url.path());
    if (resourcePath.isEmpty())
        return nullptr;
    if (!resourcePath.startsWith(QLatin1Char('/')))
        resourcePath.prepend(QLatin1Char('/'));
    return unitRegistry()->resourcePathToCachedUnit.value(resourcePath, nullptr);
}
}
int QT_MANGLE_NAMESPACE(qInitResources_qmlcache_gym_app)() {
    ::unitRegistry();
    return 1;
}
Q_CONSTRUCTOR_FUNCTION(QT_MANGLE_NAMESPACE(qInitResources_qmlcache_gym_app))
int QT_MANGLE_NAMESPACE(qCleanupResources_qmlcache_gym_app)() {
    return 1;
}
