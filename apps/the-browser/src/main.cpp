#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickWebEngineProfile>
#include <QRegularExpression>
#include <QtWebEngineQuick/qtwebenginequickglobal.h>

int main(int argc, char *argv[]) {
  qputenv("QTWEBENGINE_CHROMIUM_FLAGS",
          "--enable-features=VaapiVideoDecodeLinuxGL "
          "--disable-gpu-memory-buffer-video-frames");

  QtWebEngineQuick::initialize();

  QGuiApplication app(argc, argv);

  app.setApplicationName("ciel-browser");
  app.setOrganizationName("ciel");
  app.setDesktopFileName("org.ciel.browser");

  auto *profile = QQuickWebEngineProfile::defaultProfile();
  profile->setStorageName(QStringLiteral("ciel-browser"));
  profile->setPersistentCookiesPolicy(
      QQuickWebEngineProfile::AllowPersistentCookies);

  QString ua = profile->httpUserAgent();
  ua.remove(QRegularExpression(QStringLiteral("QtWebEngine/\\S+\\s*")));
  profile->setHttpUserAgent(ua);

  QQmlApplicationEngine engine;

  engine.addImportPath("/home/k/projects/ciel/ciel-ui/build");

  engine.loadFromModule("Ciel.Browser", "Main");

  if (engine.rootObjects().isEmpty())
    return -1;

  return app.exec();
}
