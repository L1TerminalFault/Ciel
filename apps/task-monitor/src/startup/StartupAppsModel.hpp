#pragma once

#include <QAbstractListModel>
#include <QList>
#include <QString>
#include <QtQml/QQmlEngine>

struct StartupAppEntry {
  QString name;
  QString comment;
  QString exec;
  QString icon;
  QString fileName;
  QString filePath;
  bool isUserScope = true;
  bool enabled = true;
};

class StartupAppsModel : public QAbstractListModel {
  Q_OBJECT
  QML_ELEMENT

  Q_PROPERTY(ScopeFilter scopeFilter READ scopeFilter WRITE setScopeFilter
                 NOTIFY scopeFilterChanged)
  Q_PROPERTY(int count READ count NOTIFY countChanged)

public:
  enum Roles {
    NameRole = Qt::UserRole + 1,
    CommentRole,
    ExecRole,
    IconRole,
    FileNameRole,
    ScopeRole,
    EnabledRole
  };

  enum ScopeFilter { AllScope = 0, UserScope, SystemScope };
  Q_ENUM(ScopeFilter)

  explicit StartupAppsModel(QObject *parent = nullptr);
  ~StartupAppsModel() override = default;

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;
  QHash<int, QByteArray> roleNames() const override;

  ScopeFilter scopeFilter() const;
  void setScopeFilter(ScopeFilter filter);

  int count() const;

  Q_INVOKABLE void refresh();
  Q_INVOKABLE void toggleEnabled(int index);

signals:
  void countChanged();
  void scopeFilterChanged();

private:
  ScopeFilter m_scopeFilter = AllScope;
  QList<StartupAppEntry> m_allEntries;
  QList<StartupAppEntry> m_visibleEntries;

  void parseDesktopFile(const QString &filePath, bool isUserScope,
                        QList<StartupAppEntry> &outList);
  void writeOverride(const StartupAppEntry &entry, bool enable);
  void applyFilter();
};
