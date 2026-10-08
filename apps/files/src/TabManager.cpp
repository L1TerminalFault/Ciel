#include "TabManager.hpp"
#include <QDir>
#include <QFileInfo>
#include <algorithm>
#include <qqmlengine.h>

static TabManager *s_tabManagerInstance = nullptr;

TabManager *TabManager::instance() { return s_tabManagerInstance; }

TabManager *TabManager::create(QQmlEngine *qmlEngine, QJSEngine *jsEngine) {
  return new TabManager(qmlEngine);
}

TabManager::TabManager(QObject *parent) : QAbstractListModel(parent) {
  s_tabManagerInstance = this;
}

QHash<int, QByteArray> TabManager::roleNames() const {
  return {{IdRole, "id"},
          {TitleRole, "title"},
          {PathRole, "path"},
          {IconRole, "icon"}};
}

int TabManager::rowCount(const QModelIndex &parent) const {
  if (parent.isValid())
    return 0;
  return m_tabs.size();
}

QVariant TabManager::data(const QModelIndex &index, int role) const {
  if (!index.isValid() || index.row() < 0 || index.row() >= m_tabs.size())
    return QVariant();

  const TabItem &tab = m_tabs.at(index.row());

  switch (role) {
  case IdRole:
    return tab.id.toString();
  case TitleRole:
  case Qt::DisplayRole:
    return tab.title;
  case PathRole:
    return tab.path;
  case IconRole:
    return tab.icon;
  default:
    return QVariant();
  }
}

int TabManager::indexOf(const QUuid &id) const {
  for (int i = 0; i < m_tabs.size(); ++i) {
    if (m_tabs[i].id == id)
      return i;
  }
  return -1;
}

QString TabManager::currentTabId() const { return m_activeTabId.toString(); }

int TabManager::currentIndex() const { return indexOf(m_activeTabId); }

QString TabManager::currentPath() const {
  int idx = currentIndex();
  return (idx >= 0) ? m_tabs[idx].path : QString();
}

bool TabManager::isRowSelected(int row) const {
  int idx = currentIndex();
  if (idx < 0)
    return false;
  const auto &sel = m_tabs[idx].selectedFiles;
  return std::find(sel.begin(), sel.end(), row) != sel.end();
}

const QVector<int> &TabManager::currentTabSelection() const {
  static const QVector<int> empty;
  int idx = currentIndex();
  if (idx < 0)
    return empty;
  return m_tabs[idx].selectedFiles;
}

int TabManager::currentTabAnchor() const {
  int idx = currentIndex();
  if (idx < 0)
    return -1;
  return m_tabs[idx].selectionAnchor;
}

void TabManager::setSelection(const QVector<int> &selection, int anchor) {
  int idx = currentIndex();
  if (idx < 0)
    return;
  m_tabs[idx].selectedFiles = selection;
  m_tabs[idx].selectionAnchor = anchor;
}

void TabManager::toggleSelection(int row) {
  int idx = currentIndex();
  if (idx < 0)
    return;
  auto &sel = m_tabs[idx].selectedFiles;
  auto it = std::find(sel.begin(), sel.end(), row);
  if (it != sel.end()) {
    sel.erase(it);
  } else {
    sel.append(row);
  }
  m_tabs[idx].selectionAnchor = row;
}

void TabManager::selectRange(int start, int end) {
  int idx = currentIndex();
  if (idx < 0)
    return;
  auto &sel = m_tabs[idx].selectedFiles;
  sel.clear();
  int s = std::min(start, end);
  int e = std::max(start, end);
  sel.reserve(e - s + 1);
  for (int i = s; i <= e; ++i) {
    sel.append(i);
  }
}

void TabManager::selectSingle(int row) {
  int idx = currentIndex();
  if (idx < 0)
    return;
  m_tabs[idx].selectedFiles.clear();
  m_tabs[idx].selectedFiles.append(row);
  m_tabs[idx].selectionAnchor = row;
}

void TabManager::selectAll(int count) {
  int idx = currentIndex();
  if (idx < 0)
    return;
  auto &sel = m_tabs[idx].selectedFiles;
  sel.clear();
  sel.reserve(count);
  for (int i = 0; i < count; ++i) {
    sel.append(i);
  }
}

void TabManager::clearSelection() {
  int idx = currentIndex();
  if (idx < 0)
    return;
  m_tabs[idx].selectedFiles.clear();
  m_tabs[idx].selectionAnchor = -1;
}

void TabManager::setCurrentTabId(const QString &uuid) {
  QUuid id(uuid);
  if (id == m_activeTabId)
    return;

  int idx = indexOf(id);
  if (idx < 0)
    return;

  m_activeTabId = id;

  m_history.removeAll(id);
  m_history.append(id);

  emit currentTabIdChanged();
  emit currentIndexChanged();
  emit currentPathChanged();
  emit currentSettingsChanged();
}

bool TabManager::setCurrentPath(const QString &path) {
  int idx = currentIndex();
  if (idx < 0)
    return false;

  QDir dir(path);
  if (!dir.exists())
    return false;

  if (m_tabs[idx].path == path)
    return true;

  m_tabs[idx].path = path;
  m_tabs[idx].selectedFiles.clear();
  m_tabs[idx].selectionAnchor = -1;

  QString folderName = dir.dirName();
  m_tabs[idx].title = folderName.isEmpty() ? path : folderName;

  emit currentPathChanged();

  QModelIndex modelIdx = index(idx);
  emit dataChanged(modelIdx, modelIdx, {TitleRole, PathRole});

  return true;
}

void TabManager::addTab(const QString &path) {
  QString folderName = QFileInfo(path).fileName();
  if (folderName.isEmpty())
    folderName = path.isEmpty() ? QStringLiteral("New Tab") : path;

  TabItem item;
  item.id = QUuid::createUuid();
  item.title = folderName;
  item.path = path;
  item.icon = "";

  m_activeTabId = item.id;
  m_history.removeAll(item.id);
  m_history.append(item.id);

  emit currentTabIdChanged();
  emit currentPathChanged();

  int newIndex = m_tabs.size();
  beginInsertRows(QModelIndex(), newIndex, newIndex);
  m_tabs.append(item);
  endInsertRows();

  emit currentIndexChanged();
}

void TabManager::closeTab(const QString &uuid) {
  QUuid closedTabId(uuid);
  int idx = indexOf(closedTabId);
  if (idx < 0)
    return;

  bool wasActive = (m_activeTabId == closedTabId);

  beginRemoveRows(QModelIndex(), idx, idx);
  m_tabs.removeAt(idx);
  endRemoveRows();

  m_history.removeAll(closedTabId);

  if (!wasActive) {
    emit currentIndexChanged();
    return;
  }

  QUuid nextId;
  while (!m_history.isEmpty()) {
    QUuid candidateId = m_history.last();
    if (indexOf(candidateId) >= 0) {
      nextId = candidateId;
      break;
    }
    m_history.removeLast();
  }

  if (nextId.isNull() && !m_tabs.isEmpty()) {
    int fallbackIndex = qBound(0, idx - 1, m_tabs.size() - 1);
    nextId = m_tabs[fallbackIndex].id;
  }

  m_activeTabId = nextId;

  emit currentTabIdChanged();
  emit currentIndexChanged();
  emit currentPathChanged();
}

void TabManager::goUp() {
  int idx = indexOf(m_activeTabId);
  if (idx < 0)
    return;

  QString current = m_tabs[idx].path;
  if (current == "/" || current.isEmpty())
    return;

  QString parentPath = QFileInfo(current).dir().absolutePath();
  setCurrentPath(parentPath);
}

void TabManager::openFolder(const QString &folderName) {
  int idx = indexOf(m_activeTabId);
  if (idx < 0)
    return;

  QString newPath = QDir(m_tabs[idx].path).filePath(folderName);
  setCurrentPath(newPath);
}

FileViewSettings TabManager::currentSettings() const {
  auto idx = indexOf(m_activeTabId);
  if (idx == -1)
    return FileViewSettings{};
  return m_tabs[idx].settings;
}

void TabManager::toggleViewMode() {
  auto idx = indexOf(m_activeTabId);
  if (idx < 0)
    return;
  m_tabs[idx].settings.listViewMode = !m_tabs[idx].settings.listViewMode;
  emit currentSettingsChanged();
}

void TabManager::toggleHiddenFiles() {
  auto idx = indexOf(m_activeTabId);
  if (idx < 0)
    return;
  m_tabs[idx].settings.showHiddenFiles = !m_tabs[idx].settings.showHiddenFiles;
  emit currentSettingsChanged();
}

void TabManager::setSortBy(FileViewSettings::SortBy criteria) {
  auto idx = indexOf(m_activeTabId);
  if (idx < 0)
    return;
  m_tabs[idx].settings.sortBy = criteria;
  emit currentSettingsChanged();
}

void TabManager::toggleAscending() {
  auto idx = indexOf(m_activeTabId);
  if (idx < 0)
    return;
  m_tabs[idx].settings.ascending = !m_tabs[idx].settings.ascending;
  emit currentSettingsChanged();
}

void TabManager::toggleFoldersFirst() {
  auto idx = indexOf(m_activeTabId);
  if (idx < 0)
    return;
  m_tabs[idx].settings.foldersFirstSorting =
      !m_tabs[idx].settings.foldersFirstSorting;
  emit currentSettingsChanged();
}

void TabManager::toggleSymlinks() {
  auto idx = indexOf(m_activeTabId);
  if (idx < 0)
    return;
  m_tabs[idx].settings.showSymlinks = !m_tabs[idx].settings.showSymlinks;
  emit currentSettingsChanged();
}
