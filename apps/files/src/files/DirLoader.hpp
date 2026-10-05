#pragma once

#include "type.hpp"
#include <QObject>
#include <QString>
#include <QVector>
#include <atomic>
#include <qtypes.h>

class DirectoryLoader : public QObject {
  Q_OBJECT

public:
  explicit DirectoryLoader(QObject *parent = nullptr);

public slots:
  void loadDirectory(const QString &path);
  void revalidateInBackground(const QString &path, qint64 cachedMtime);
  void cancel();

signals:
  void entriesReady(const QVector<ItemEntery> &batch);
  void loadFinished(const QString &path, qint64 modifiedTime);
  void loadError(const QString &path, int errorCode,
                 const QString &errorMessage);

private:
  std::atomic<bool> m_cancelRequested{false};
};
