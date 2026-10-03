#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>

int main(int argc, char *argv[]) {
  QGuiApplication app(argc, argv);

  // Desktop identity & predictability contract
  app.setApplicationName("ciel-task-monitor");
  app.setOrganizationName("ciel");
  app.setDesktopFileName("org.ciel.taskmonitor");

  QQmlApplicationEngine engine;

  // Automatically resolve Ciel.Ui QML module
  engine.addImportPath("/home/k/projects/ciel/ciel-ui/build");

  engine.loadFromModule("Ciel.TaskMonitor", "Main");

  if (engine.rootObjects().isEmpty())
    return -1;

  return app.exec();
}
