#pragma once

#include <QAbstractListModel>
#include <QString>
#include <QUrl>
#include <QtQml/qqmlregistration.h>

struct TabItem {
  QString id;
  QString title;
  QUrl url;
  bool loading{false};
  int progress{0};
  bool canGoBack{false};
  bool canGoForward{false};
  bool isPinned{false};
};

class TabModel : public QAbstractListModel {
  Q_OBJECT
  QML_ELEMENT

  Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY
                 currentIndexChanged)
  Q_PROPERTY(int count READ count NOTIFY countChanged)
  Q_PROPERTY(QString workspaceId READ workspaceId WRITE setWorkspaceId NOTIFY
                 workspaceIdChanged)
  Q_PROPERTY(bool canRestoreClosedTab READ canRestoreClosedTab NOTIFY
                 closedTabsChanged)

public:
  enum Roles {
    IdRole = Qt::UserRole + 1,
    TitleRole,
    UrlRole,
    LoadingRole,
    ProgressRole,
    CanGoBackRole,
    CanGoForwardRole,
    IsPinnedRole
  };

  explicit TabModel(QObject *parent = nullptr);

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;
  QHash<int, QByteArray> roleNames() const override;

  int currentIndex() const;
  void setCurrentIndex(int index);
  int count() const;

  QString workspaceId() const;
  void setWorkspaceId(const QString &wsId);
  bool canRestoreClosedTab() const { return !m_closedTabs.isEmpty(); }
  void startEphemeral(bool isPrivate, const QUrl &firstUrl = QUrl());

  Q_INVOKABLE void restoreClosedTab();

  Q_INVOKABLE void
  addTab(const QUrl &url = QUrl(QStringLiteral("about:blank")));
  Q_INVOKABLE void closeTab(int index);
  Q_INVOKABLE void updateTitle(int index, const QString &title);
  Q_INVOKABLE void updateUrl(int index, const QUrl &url);
  Q_INVOKABLE void updateLoading(int index, bool loading);
  Q_INVOKABLE void updateProgress(int index, int progress);
  Q_INVOKABLE void updateNavigation(int index, bool canGoBack,
                                    bool canGoForward);
  Q_INVOKABLE void moveTab(int from, int to);
  Q_INVOKABLE void togglePin(int index);
  Q_INVOKABLE void setPinned(int index, bool pinned);

  Q_INVOKABLE void closeOtherTabs(int keepIndex);
  Q_INVOKABLE void closeTabsToBottom(int index);

signals:
  void currentIndexChanged();
  void countChanged();
  void workspaceIdChanged();
  void closedTabsChanged();

private:
  void loadTabs();
  void persistOrder();
  void persistCurrentTab();

  struct ClosedTab {
    QUrl url;
    QString title;
    bool isPinned = false;
    int index = 0;
  };
  static constexpr int kMaxClosedTabs = 25;
  QList<ClosedTab> m_closedTabs;
  void rememberClosedTab(int index);

  QList<TabItem> m_tabs;
  int m_currentIndex{0};
  QString m_workspaceId;
  bool m_ephemeral = false;
  bool m_private = false;
};
