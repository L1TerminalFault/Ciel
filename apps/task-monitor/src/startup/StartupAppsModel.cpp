#include "StartupAppsModel.hpp"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QRegularExpression>
#include <QStandardPaths>
#include <QTextStream>

StartupAppsModel::StartupAppsModel(QObject *parent)
    : QAbstractListModel(parent) {
  refresh();
}

int StartupAppsModel::rowCount(const QModelIndex &parent) const {
  if (parent.isValid())
    return 0;
  return m_visibleEntries.size();
}

QVariant StartupAppsModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid() || index.row() < 0 ||
      index.row() >= m_visibleEntries.size())
    return QVariant();

  const StartupAppEntry &entry = m_visibleEntries.at(index.row());

  switch (role) {
  case NameRole:
    return entry.name;
  case CommentRole:
    return entry.comment;
  case ExecRole:
    return entry.exec;
  case IconRole:
    return entry.icon.isEmpty() ? "app-window" : entry.icon;
  case FileNameRole:
    return entry.fileName;
  case ScopeRole:
    return entry.isUserScope ? "User" : "System";
  case EnabledRole:
    return entry.enabled;
  default:
    return QVariant();
  }
}

QHash<int, QByteArray> StartupAppsModel::roleNames() const {
  QHash<int, QByteArray> roles;
  roles[NameRole] = "appName";
  roles[CommentRole] = "appComment";
  roles[ExecRole] = "appExec";
  roles[IconRole] = "appIcon";
  roles[FileNameRole] = "appFileName";
  roles[ScopeRole] = "appScope";
  roles[EnabledRole] = "appEnabled";
  return roles;
}

StartupAppsModel::ScopeFilter StartupAppsModel::scopeFilter() const {
  return m_scopeFilter;
}

void StartupAppsModel::setScopeFilter(ScopeFilter filter) {
  if (m_scopeFilter == filter)
    return;
  m_scopeFilter = filter;
  emit scopeFilterChanged();
  applyFilter();
}

int StartupAppsModel::count() const { return m_visibleEntries.size(); }

void StartupAppsModel::applyFilter() {
  beginResetModel();
  m_visibleEntries.clear();

  for (const StartupAppEntry &entry : m_allEntries) {
    if (m_scopeFilter == UserScope && !entry.isUserScope)
      continue;
    if (m_scopeFilter == SystemScope && entry.isUserScope)
      continue;
    m_visibleEntries.append(entry);
  }

  endResetModel();
  emit countChanged();
}

void StartupAppsModel::parseDesktopFile(const QString &filePath,
                                        bool isUserScope,
                                        QList<StartupAppEntry> &outList) {
  QFile file(filePath);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return;

  QTextStream in(&file);
  QString name;
  QString comment;
  QString exec;
  QString icon;
  bool hidden = false;
  bool gnomeEnabled = true;
  bool isDesktopEntrySection = false;

  while (!in.atEnd()) {
    QString line = in.readLine().trimmed();
    if (line.isEmpty() || line.startsWith('#'))
      continue;

    if (line.startsWith('[')) {
      isDesktopEntrySection = (line == "[Desktop Entry]");
      continue;
    }

    if (!isDesktopEntrySection)
      continue;

    int eqIdx = line.indexOf('=');
    if (eqIdx <= 0)
      continue;

    QString key = line.left(eqIdx).trimmed();
    QString val = line.mid(eqIdx + 1).trimmed();

    if (key == "Name" && name.isEmpty()) {
      name = val;
    } else if (key == "Comment" && comment.isEmpty()) {
      comment = val;
    } else if (key == "Exec" && exec.isEmpty()) {
      exec = val;
    } else if (key == "Icon" && icon.isEmpty()) {
      icon = val;
    } else if (key == "Hidden") {
      hidden = (val.compare("true", Qt::CaseInsensitive) == 0);
    } else if (key == "X-GNOME-Autostart-enabled") {
      gnomeEnabled = (val.compare("false", Qt::CaseInsensitive) != 0);
    }
  }
  file.close();

  if (name.isEmpty() && exec.isEmpty())
    return;

  QFileInfo fi(filePath);
  QString base = fi.fileName();

  if (icon.contains('/')) {
    icon = QFileInfo(icon).baseName();
  }

  StartupAppEntry entry;
  entry.name = name.isEmpty() ? fi.baseName() : name;
  entry.comment = comment;
  entry.exec = exec;
  entry.icon = icon;
  entry.fileName = base;
  entry.filePath = filePath;
  entry.isUserScope = isUserScope;
  entry.enabled = (!hidden && gnomeEnabled);

  outList.append(entry);
}

void StartupAppsModel::refresh() {
  m_allEntries.clear();

  QString userAutostart =
      QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) +
      "/autostart";
  QDir userDir(userAutostart);

  QList<StartupAppEntry> userEntries;
  if (userDir.exists()) {
    QStringList files = userDir.entryList(QStringList() << "*.desktop",
                                          QDir::Files | QDir::Readable);
    for (const QString &f : files) {
      parseDesktopFile(userDir.filePath(f), true, userEntries);
    }
  }

  QSet<QString> maskedFiles;
  for (const StartupAppEntry &e : userEntries) {
    maskedFiles.insert(e.fileName);
  }

  QString systemAutostart = "/etc/xdg/autostart";
  QDir sysDir(systemAutostart);
  QList<StartupAppEntry> sysEntries;
  if (sysDir.exists()) {
    QStringList files = sysDir.entryList(QStringList() << "*.desktop",
                                         QDir::Files | QDir::Readable);
    for (const QString &f : files) {
      if (!maskedFiles.contains(f)) {
        parseDesktopFile(sysDir.filePath(f), false, sysEntries);
      }
    }
  }

  m_allEntries = userEntries + sysEntries;

  std::sort(m_allEntries.begin(), m_allEntries.end(),
            [](const StartupAppEntry &a, const StartupAppEntry &b) {
              return a.name.toLower() < b.name.toLower();
            });

  applyFilter();
}

void StartupAppsModel::writeOverride(const StartupAppEntry &entry,
                                     bool enable) {
  QString userAutostart =
      QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) +
      "/autostart";
  QDir dir(userAutostart);
  if (!dir.exists()) {
    dir.mkpath(".");
  }

  QString destPath = dir.filePath(entry.fileName);

  if (entry.isUserScope && QFile::exists(destPath)) {
    QFile file(destPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
      return;

    QStringList lines;
    QTextStream in(&file);
    bool foundHidden = false;

    while (!in.atEnd()) {
      QString line = in.readLine();
      if (line.startsWith("Hidden=")) {
        lines.append(enable ? "Hidden=false" : "Hidden=true");
        foundHidden = true;
      } else if (line.startsWith("X-GNOME-Autostart-enabled=")) {
        lines.append(enable ? "X-GNOME-Autostart-enabled=true"
                            : "X-GNOME-Autostart-enabled=false");
      } else {
        lines.append(line);
      }
    }
    file.close();

    if (!foundHidden) {
      lines.append(enable ? "Hidden=false" : "Hidden=true");
    }

    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate |
                  QIODevice::Text)) {
      QTextStream out(&file);
      for (const QString &l : lines) {
        out << l << "\n";
      }
      file.close();
    }
    return;
  }

  if (!entry.isUserScope) {
    if (enable) {
      if (QFile::exists(destPath)) {
        QFile::remove(destPath);
      }
    } else {
      QFile srcFile(entry.filePath);
      if (!srcFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

      QString content = srcFile.readAll();
      srcFile.close();

      if (content.contains("Hidden=")) {
        content.replace(QRegularExpression("Hidden=.*"), "Hidden=true");
      } else {
        content.append("\nHidden=true\n");
      }

      QFile outFile(destPath);
      if (outFile.open(QIODevice::WriteOnly | QIODevice::Truncate |
                       QIODevice::Text)) {
        QTextStream out(&outFile);
        out << content;
        outFile.close();
      }
    }
  }
}

void StartupAppsModel::toggleEnabled(int index) {
  if (index < 0 || index >= m_visibleEntries.size())
    return;

  bool targetState = !m_visibleEntries[index].enabled;
  m_visibleEntries[index].enabled = targetState;

  for (StartupAppEntry &e : m_allEntries) {
    if (e.fileName == m_visibleEntries[index].fileName &&
        e.isUserScope == m_visibleEntries[index].isUserScope) {
      e.enabled = targetState;
      break;
    }
  }

  writeOverride(m_visibleEntries[index], targetState);

  QModelIndex idx = this->index(index, 0);
  emit dataChanged(idx, idx, {EnabledRole});
}
