#pragma once

#include <QImageReader>
#include <QQuickImageResponse>
#include <QRunnable>
#include <QThreadPool>
#include <qcoreapplication.h>
#include <qimage.h>
#include <qobject.h>
#include <qpixmap.h>
#include <qquickimageprovider.h>
#include <qrunnable.h>
#include <qsize.h>
#include <qthread.h>
#include <qthreadpool.h>
#include <qtmetamacros.h>
#include <qtypes.h>

class ThumbnailResponseRunnable : public QObject, public QRunnable {
  Q_OBJECT
signals:
  void done(QImage image);

public:
  ThumbnailResponseRunnable(const QString &filePath, const QSize &requestedSize)
      : m_filePath(filePath), m_requestedSize(requestedSize) {}

  void run() override {
    QImageReader reader(m_filePath);

    if (m_requestedSize.isValid()) {
      reader.setScaledSize(m_requestedSize);
    }
    if (!reader.canRead()) {
      emit done(QImage());
      return;
    }
    QImage image = reader.read();
    emit done(image);
  }

private:
  QString m_filePath = "";
  QSize m_requestedSize;
};

class ThumbnailImageResponse : public QQuickImageResponse {
public:
  ThumbnailImageResponse(const QString &filePath, const QSize &requestedSize,
                         QThreadPool *pool) {
    auto runnable = new ThumbnailResponseRunnable(filePath, requestedSize);
    connect(runnable, &ThumbnailResponseRunnable::done, this,
            &ThumbnailImageResponse::handleDone);
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
};

class AsyncThumbnailProvider : public QQuickAsyncImageProvider {
public:
  QQuickImageResponse *
  requestImageResponse(const QString &filePath,
                       const QSize &requestedSize) override {

    return new ThumbnailImageResponse(filePath, requestedSize, &pool);
  }

private:
  QThreadPool pool;
};
