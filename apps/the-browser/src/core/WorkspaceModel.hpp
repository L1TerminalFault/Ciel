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
  Q_PROPERTY(
      bool ephemeral READ ephemeral WRITE setEphemeral NOTIFY ephemeralChanged)
  Q_PROPERTY(
      bool incognito READ incognito WRITE setIncognito NOTIFY incognitoChanged)
  Q_PROPERTY(
      QUrl startUrl READ startUrl WRITE setStartUrl NOTIFY startUrlChanged)

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

  bool ephemeral() const { return m_ephemeral; }
  void setEphemeral(bool on);
  bool incognito() const { return m_incognito; }
  void setIncognito(bool on) {
    if (m_incognito != on) {
      m_incognito = on;
      emit incognitoChanged();
    }
  }
  QUrl startUrl() const { return m_startUrl; }
  void setStartUrl(const QUrl &u) {
    if (m_startUrl != u) {
      m_startUrl = u;
      emit startUrlChanged();
    }
  }

signals:
  void countChanged();
  void currentIndexChanged();
  void ephemeralChanged();
  void incognitoChanged();
  void startUrlChanged();

private:
  void unloadWorkspaces();
  void releaseTabModels();
  void readWorkspaceRows();
  void seedDefaultWorkspaces();
  WorkspaceItem insertWorkspaceRecord(const QString &name, const QString &color,
                                      const QString &icon,
                                      const QString &presetId);
  QString uniqueName(const QString &requested) const;
  void persistWorkspaceOrder();
  void persistCurrentWorkspace();

  void loadEphemeralWorkspaces();
  bool m_ephemeral = false;
  bool m_incognito = false;
  QUrl m_startUrl;

  QList<WorkspaceItem> m_workspaces;
  QHash<QString, TabModel *> m_tabModels;
  int m_currentIndex{0};
};
