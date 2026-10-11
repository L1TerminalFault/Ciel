#pragma once

#include <QJSEngine>
#include <QObject>
#include <QQmlEngine>
#include <QString>
#include <QVariantList>
#include <QtQml/qqmlregistration.h>

#include <functional>

class PasswordManager : public QObject {
  Q_OBJECT
  QML_ELEMENT
  QML_SINGLETON

public:
  enum class CaptureKind { Ignore, New, Update };

  static PasswordManager *create(QQmlEngine *, QJSEngine *);
  static PasswordManager *instance();

  static QString normalizeOrigin(const QString &raw);
  static bool autofillAllowed(const QString &origin);

  Q_INVOKABLE QVariantList list(const QString &search = QString());
  Q_INVOKABLE void save(const QString &origin, const QString &username,
                        const QString &password);
  Q_INVOKABLE void remove(const QString &id);
  Q_INVOKABLE void reveal(const QString &id);
  Q_INVOKABLE void copyPassword(const QString &id);
  Q_INVOKABLE QString injectedScript();

  void classifyCapture(const QString &origin, const QString &username,
                       const QString &password, QObject *ctx,
                       std::function<void(CaptureKind)> done);
  void
  credentialsFor(const QString &origin, QObject *ctx,
                 std::function<void(const QString &, const QString &)> done);

signals:
  void passwordsChanged();
  void passwordRevealed(const QString &id, const QString &password);
  void keychainError(const QString &message);

private:
  explicit PasswordManager(QObject *parent = nullptr);

  using SecretDone = std::function<void(bool ok, const QString &valueOrError)>;
  void readSecret(const QString &id, QObject *ctx, SecretDone done);
  void writeSecret(const QString &id, const QString &password, QObject *ctx,
                   SecretDone done);
  void deleteSecret(const QString &id);
  QString keyFor(const QString &id) const;
};

class PasswordBridge : public QObject {
  Q_OBJECT
  QML_ELEMENT

public:
  explicit PasswordBridge(QObject *parent = nullptr);

  Q_INVOKABLE void acceptPending();
  Q_INVOKABLE void discardPending();

public slots:
  void submitted(const QString &origin, const QString &username,
                 const QString &password);
  void requestFill(const QString &origin);

signals:
  void fill(const QString &origin, const QString &username,
            const QString &password);
  void saveOffered(const QString &host, const QString &username, bool isUpdate);

private:
  struct Pending {
    QString origin, username, password;
    bool active = false;
  } m_pending;
};
