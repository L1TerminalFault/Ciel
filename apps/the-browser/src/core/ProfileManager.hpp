#pragma once

#include "Database.hpp"

#include <QDateTime>
#include <QJSEngine>
#include <QObject>
#include <QQmlEngine>
#include <QString>
#include <QVariantList>
#include <QtQml/qqmlregistration.h>

class ProfileManager : public QObject {
  Q_OBJECT
  QML_ELEMENT
  QML_SINGLETON

  Q_PROPERTY(
      QString activeProfileId READ activeProfileId NOTIFY activeProfileChanged)
  Q_PROPERTY(QString currentSessionId READ currentSessionId CONSTANT)
  Q_PROPERTY(QString profilePath READ profilePath NOTIFY activeProfileChanged)

public:
  static ProfileManager *create(QQmlEngine *qmlEngine, QJSEngine *jsEngine);
  static ProfileManager *instance();

  explicit ProfileManager(QObject *parent = nullptr);
  ~ProfileManager() override;

  QString activeProfileId() const;
  QString currentSessionId() const;
  QString profilePath() const;
  Database *database();

  Q_INVOKABLE void switchProfile(const QString &profileId);
  Q_INVOKABLE void recordVisit(const QString &url, const QString &title,
                               const QString &workspaceId);
  Q_INVOKABLE QVariantList fetchHistory(const QString &search = QString());
  Q_INVOKABLE void deleteHistoryItem(qint64 id);
  Q_INVOKABLE void clearHistory(int rangeIndex);

  Q_INVOKABLE bool isBookmarked(const QString &url);
  Q_INVOKABLE bool toggleBookmark(const QString &url, const QString &title);

  Q_PROPERTY(QString profilePath READ profilePath NOTIFY activeProfileChanged)
  Q_PROPERTY(QString webEngineStoragePath READ webEngineStoragePath NOTIFY activeProfileChanged)

  Q_INVOKABLE QString profilePathFor(const QString &profileId) const;
  Q_INVOKABLE QString webEngineStoragePathFor(const QString &profileId) const;

  QString webEngineStoragePath() const;   // convenience for current profile

signals:
  void activeProfileChanged();
  void visitRecorded(const QString &url, const QString &title,
                     const QString &workspaceId);
  void bookmarksChanged();

private:
  void initializeProfile(const QString &profileId);
  void startSession();
  void endSession();

  static ProfileManager *s_instance;
  QString m_activeProfileId;
  QString m_currentSessionId;
  QString m_profilePath;
  Database m_database;
};
