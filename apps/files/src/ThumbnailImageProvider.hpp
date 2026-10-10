#pragma once

#include <QCache>
#include <QCryptographicHash>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QImageReader>
#include <QImageWriter>
#include <QQuickImageResponse>
#include <QRunnable>
#include <QStandardPaths>
#include <QThreadPool>
#include <qcache.h>
#include <qcoreapplication.h>
#include <qimage.h>
#include <qmutex.h>
#include <qobject.h>
#include <qpixmap.h>
#include <qquickimageprovider.h>
#include <qrunnable.h>
#include <qsize.h>
#include <qthread.h>
#include <qthreadpool.h>
#include <qtmetamacros.h>
#include <qtypes.h>
#include <qurl.h>

class ThumbnailResponseRunnable : public QObject, public QRunnable {
  Q_OBJECT
signals:
  void done(QImage image);

public:
  ThumbnailResponseRunnable(const QString &filePath, const QSize &requestedSize,
                            QMutex *mutexlock, QCache<QString, QImage> *cache,
                            const QString &cachekey)
      : m_filePath(filePath), m_requestedSize(requestedSize),
        m_mutextLock(mutexlock), m_memCache(cache), m_cacheKey(cachekey) {
    m_mutextLock->lock();
    m_memCache->setMaxCost(200);
    m_mutextLock->unlock();
    setAutoDelete(false);
  }

  void run() override {
    QString fileUri = QUrl::fromLocalFile(m_filePath).toString();
    QByteArray hashBytes =
        QCryptographicHash::hash(fileUri.toUtf8(), QCryptographicHash::Md5);
    QString thumbFileName = QString::fromLatin1(hashBytes.toHex()) + ".png";
    QString genericCache =
        QStandardPaths::writableLocation(QStandardPaths::GenericCacheLocation);
    QString systemThumbDir = genericCache + "/thumbnails/";

    QString sizeSubFolder =
        (m_requestedSize.width() <= 128 && m_requestedSize.height() <= 128)
            ? "normal/"
            : "large/";

    QString finalThumbPath = systemThumbDir + sizeSubFolder + thumbFileName;

    if (QFile::exists(finalThumbPath)) {
      QImageReader reader(finalThumbPath);
      QImage diskImage = reader.read();
      if (!diskImage.isNull()) {
        m_mutextLock->lock();
        if (!m_memCache->contains(m_cacheKey)) {
          m_memCache->insert(m_cacheKey, new QImage(diskImage),
                             diskImage.sizeInBytes());
        }
        m_mutextLock->unlock();
        emit done(diskImage);
        return;
      }
    }

    QImageReader reader(m_filePath);
    if (m_requestedSize.isValid()) {
      reader.setScaledSize(m_requestedSize);
    }
    if (!reader.canRead()) {
      emit done(QImage());
      return;
    }
    QImage newlyGeneratedImage = reader.read();
    if (!newlyGeneratedImage.isNull()) {
      // push to cache
      m_mutextLock->lock();
      if (!m_memCache->contains(m_cacheKey)) {
        m_memCache->insert(m_cacheKey, new QImage(newlyGeneratedImage),
                           newlyGeneratedImage.sizeInBytes());
      }
      m_mutextLock->unlock();
    }
    emit done(newlyGeneratedImage);

    if (!newlyGeneratedImage.isNull()) {
      QDir().mkpath(systemThumbDir + sizeSubFolder);

      QImageWriter writer(finalThumbPath, "png");
      QFileInfo originalInfo(m_filePath);
      writer.setText("Thumb::URI", fileUri);
      writer.setText(
          "Thumb::MTime",
          QString::number(originalInfo.lastModified().toSecsSinceEpoch()));
      writer.write(newlyGeneratedImage);
    }
  }

private:
  QString m_filePath = "";
  QSize m_requestedSize;
  QCache<QString, QImage> *m_memCache;
  QMutex *m_mutextLock;
  QString m_cacheKey;
};

class ThumbnailImageResponse : public QQuickImageResponse {
public:
  ThumbnailImageResponse(const QString &filePath, const QSize &requestedSize,
                         QThreadPool *pool,
                         QCache<QString, QImage> *sharedCache,
                         QMutex *sharedMutex)
      : m_cache(sharedCache), m_mutex(sharedMutex) {

    QString cacheKey = filePath + QString("_%1x%2")
                                      .arg(requestedSize.width())
                                      .arg(requestedSize.height());
    m_mutex->lock();
    if (m_cache->contains(cacheKey)) {
      m_image = *(m_cache->object(cacheKey));
      m_mutex->unlock();
      emit finished();
      return;
    }
    m_mutex->unlock();

    auto runnable = new ThumbnailResponseRunnable(filePath, requestedSize,
                                                  m_mutex, m_cache, cacheKey);
    connect(runnable, &ThumbnailResponseRunnable::done, this,
            &ThumbnailImageResponse::handleDone);
    connect(runnable, &ThumbnailResponseRunnable::done, runnable,
            &QObject::deleteLater);
    pool->start(runnable);
  }

  void handleDone(QImage image) {
    m_image = image;
    m_failed = image.isNull();
    emit finished();
  }
  QString errorString() const override { return m_failed ? "err" : ""; }
  QQuickTextureFactory *textureFactory() const override {
    if (m_image.isNull())
      return nullptr;
    return QQuickTextureFactory::textureFactoryForImage(m_image);
  }

private:
  QImage m_image;
  bool m_failed = false;
  QMutex *m_mutex;
  QCache<QString, QImage> *m_cache;
};

class AsyncThumbnailProvider : public QQuickAsyncImageProvider {
public:
  QQuickImageResponse *
  requestImageResponse(const QString &filePath,
                       const QSize &requestedSize) override {

    return new ThumbnailImageResponse(filePath, requestedSize, &pool,
                                      &m_memCache, &m_cacheMutex);
  }

private:
  QThreadPool pool;
  QCache<QString, QImage> m_memCache;
  QMutex m_cacheMutex;
};
