#pragma once

#include "files/type.hpp"
#include <QByteArray>
#include <QObject>
#include <QString>
#include <QVector>
#include <atomic>

class DirectoryLoader : public QObject {
  Q_OBJECT

public:
  explicit DirectoryLoader(QObject *parent = nullptr);

  Q_INVOKABLE void loadDirectory(const QString &path, bool batchLoading = true);
  Q_INVOKABLE void revalidateInBackground(const QString &path,
                                          qint64 cachedMtime);
  void cancel();

signals:
  void loadStarted(const QString &path);
  void entriesReady(const QVector<ItemEntery> &entries);
  void loadFinished(const QString &path, qint64 modifiedTime);
  void loadError(const QString &path, int errorCode,
                 const QString &errorMessage);

private:
  std::atomic<bool> m_cancelRequested{false};
};
