#include "Daemon.hpp"
#include <QCoreApplication>

int main(int argc, char *argv[]) {
  QCoreApplication app(argc, argv);
  app.setApplicationName("ciel-system-daemon");
  app.setOrganizationName("ciel");

  Daemon daemon;
  if (!daemon.init()) {
    return 1;
  }

  return app.exec();
}
