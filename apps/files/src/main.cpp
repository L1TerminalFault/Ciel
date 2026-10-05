
#include "TabManager.hpp"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <qqml.h>

int main(int argc, char *argv[]) {
  QGuiApplication app(argc, argv);

  app.setApplicationName("ciel-files");
  app.setOrganizationName("ciel");
  app.setDesktopFileName("org.ciel.files");

  QQmlApplicationEngine engine;

  engine.addImportPath("/home/k/projects/ciel/ciel-ui/build");

  engine.loadFromModule("Ciel.Files", "Main");

  if (engine.rootObjects().isEmpty())
    return -1;

  return app.exec();
}
