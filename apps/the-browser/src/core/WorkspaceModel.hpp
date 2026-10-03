#pragma once

#include "TabModel.hpp"

#include <QAbstractListModel>
#include <QColor>
#include <QHash>
#include <QString>
#include <QtQml/qqmlregistration.h>

struct WorkspaceItem {
  QString id;
  QString name;
  QString color;
  QString icon;
  QString presetId;
};

class WorkspaceModel : public QAbstractListModel {
  Q_OBJECT
  QML_ELEMENT

  Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY
                 currentIndexChanged)
  Q_PROPERTY(int count READ count NOTIFY countChanged)
  Q_PROPERTY(QString currentWorkspaceId READ currentWorkspaceId NOTIFY
                 currentIndexChanged)

public:
  enum Roles {
    IdRole = Qt::UserRole + 1,
    NameRole,
    ColorRole,
    IconRole,
    PresetIdRole
  };

  explicit WorkspaceModel(QObject *parent = nullptr);

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;
  QHash<int, QByteArray> roleNames() const override;

  int count() const;
  int currentIndex() const;
  void setCurrentIndex(int index);
  QString currentWorkspaceId() const;

  Q_INVOKABLE TabModel *tabModel(const QString &workspaceId);
  Q_INVOKABLE void
  createWorkspace(const QString &name,
                  const QString &color = QStringLiteral("#3B82F6"),
                  const QString &icon = QStringLiteral("browser"),
                  const QString &presetId = QString());
  Q_INVOKABLE void removeWorkspace(int index);
  Q_INVOKABLE void saveAsPreset(int index, const QString &presetName);
  Q_INVOKABLE void loadWorkspaces();

signals:
  void countChanged();
  void currentIndexChanged();

private:
  QList<WorkspaceItem> m_workspaces;
  QHash<QString, TabModel *> m_tabModels;
  int m_currentIndex{0};
};
