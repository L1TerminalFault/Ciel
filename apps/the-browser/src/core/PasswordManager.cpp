#include "PasswordManager.hpp"
#include "ProfileManager.hpp"

#include <QClipboard>
#include <QCoreApplication>
#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QGuiApplication>
#include <QPointer>
#include <QTimer>
#include <QUrl>
#include <QUuid>

#include <qt6keychain/keychain.h>

namespace {

QString service() { return QStringLiteral("ciel-browser"); }

const char *kPageScript = R"JS(
(function () {
  'use strict';
  if (window.top !== window) return;
  if (typeof QWebChannel === 'undefined' || !window.qt || !qt.webChannelTransport) return;
  if (location.protocol !== 'https:' && location.protocol !== 'http:') return;

  new QWebChannel(qt.webChannelTransport, function (channel) {
    var bridge = channel.objects.cielPasswords;
    if (!bridge) return;

    var valueSetter = Object.getOwnPropertyDescriptor(HTMLInputElement.prototype, 'value').set;

    function visible(el) {
      var r = el.getBoundingClientRect();
      var s = getComputedStyle(el);
      return r.width > 0 && r.height > 0 && s.visibility !== 'hidden' && s.display !== 'none';
    }
    function passwordFields(scope) {
      return Array.prototype.filter.call(scope.querySelectorAll('input[type=password]'), visible);
    }
    function usernameFor(pw) {
      var inputs = (pw.form || document).querySelectorAll('input');
      var found = null;
      for (var i = 0; i < inputs.length; i++) {
        var el = inputs[i];
        if (el === pw) break;
        var t = (el.getAttribute('type') || 'text').toLowerCase();
        if ((t === 'text' || t === 'email' || t === 'tel') && visible(el)) found = el;
      }
      return found;
    }
    function setValue(el, v) {
      valueSetter.call(el, v);
      el.dispatchEvent(new Event('input', { bubbles: true }));
      el.dispatchEvent(new Event('change', { bubbles: true }));
    }

    var requested = false;
    function check() {
      var has = passwordFields(document).length > 0;
      if (has && !requested) { requested = true; bridge.requestFill(location.origin); }
      else if (!has) { requested = false; }
    }
    bridge.fill.connect(function (origin, user, pass) {
      if (origin !== location.origin) return;          
      var pws = passwordFields(document);
      if (!pws.length) return;
      var pw = pws[0];
      if (pw.value) return;
      if ((pw.getAttribute('autocomplete') || '') === 'new-password') return;
      var field = usernameFor(pw);
      if (field && field.value && field.value !== user) return;
      if (field && !field.value && user) setValue(field, user);
      setValue(pw, pass);
    });

    var timer = null;
    new MutationObserver(function () {
      if (timer) return;
      timer = setTimeout(function () { timer = null; check(); }, 300);
    }).observe(document.documentElement, { childList: true, subtree: true });
    check();

    var lastKey = '', lastAt = 0;
    function capture(scope) {
      var pws = passwordFields(scope).filter(function (e) { return e.value; });
      if (!pws.length) return;
      var pw = pws[pws.length - 1];     
      var field = usernameFor(pw);
      var user = field ? field.value : '';
      var key = user + '\u0000' + pw.value;
      var now = Date.now();
      if (key === lastKey && now - lastAt < 2000) return;
      lastKey = key; lastAt = now;
      bridge.submitted(location.origin, user, pw.value);
    }
    document.addEventListener('submit', function (e) { capture(e.target); }, true);
    document.addEventListener('keydown', function (e) {
      if (e.key === 'Enter' && e.target && e.target.type === 'password')
        capture(e.target.form || document);
    }, true);
    document.addEventListener('click', function (e) {
      var b = e.target && e.target.closest ? e.target.closest('button, input') : null;
      if (b && b.type === 'submit') capture(b.form || document);
    }, true);
  });
})();
)JS";

} // namespace

PasswordManager *PasswordManager::instance() {
  static PasswordManager *s = new PasswordManager(QCoreApplication::instance());
  return s;
}

PasswordManager *PasswordManager::create(QQmlEngine *, QJSEngine *) {
  PasswordManager *pm = instance();
  QJSEngine::setObjectOwnership(pm, QJSEngine::CppOwnership);
  return pm;
}

PasswordManager::PasswordManager(QObject *parent) : QObject(parent) {
  connect(ProfileManager::instance(), &ProfileManager::activeProfileChanged,
          this, &PasswordManager::passwordsChanged);
}

QString PasswordManager::keyFor(const QString &id) const {
  return ProfileManager::instance()->activeProfileId() + QLatin1Char('/') + id;
}

QString PasswordManager::normalizeOrigin(const QString &raw) {
  const QUrl u(raw);
  const QString scheme = u.scheme().toLower();
  if ((scheme != QLatin1String("http") && scheme != QLatin1String("https")) ||
      u.host().isEmpty())
    return {};

  QString host = u.host();
  if (host.contains(QLatin1Char(':')))
    host = QLatin1Char('[') + host + QLatin1Char(']');

  QString out = scheme + QStringLiteral("://") + host;
  if (u.port() != -1)
    out += QLatin1Char(':') + QString::number(u.port());
  return out;
}

bool PasswordManager::autofillAllowed(const QString &origin) {
  const QUrl u(origin);
  if (u.scheme() == QLatin1String("https"))
    return true;
  const QString h = u.host();
  return h == QLatin1String("localhost") || h == QLatin1String("127.0.0.1") ||
         h == QLatin1String("::1");
}

QString PasswordManager::injectedScript() {
  static QString cached;
  if (cached.isEmpty()) {
    QFile f(QStringLiteral(":/qtwebchannel/qwebchannel.js"));
    if (f.open(QIODevice::ReadOnly))
      cached = QString::fromUtf8(f.readAll()) + QLatin1Char('\n');
    else
      qWarning() << "[passwords] qwebchannel.js not found; autofill disabled";
    cached += QString::fromUtf8(kPageScript);
  }
  return cached;
}

/* ---- keyring helpers (all async) ---- */

void PasswordManager::readSecret(const QString &id, QObject *ctx, SecretDone done) {
  auto *job = new QKeychain::ReadPasswordJob(service(), this);
  job->setAutoDelete(true);
  job->setKey(keyFor(id));
  QPointer<QObject> guard(ctx);
  connect(job, &QKeychain::Job::finished, this,
          [job, guard, done](QKeychain::Job *) {
            if (!guard)
              return;
            const bool ok = job->error() == QKeychain::NoError;
            done(ok, ok ? job->textData() : job->errorString());
          });
  job->start();
}

void PasswordManager::writeSecret(const QString &id, const QString &password,
                                  QObject *ctx, SecretDone done) {
  auto *job = new QKeychain::WritePasswordJob(service(), this);
  job->setAutoDelete(true);
  job->setKey(keyFor(id));
  job->setTextData(password);
  QPointer<QObject> guard(ctx);
  connect(job, &QKeychain::Job::finished, this,
          [job, guard, done](QKeychain::Job *) {
            if (!guard)
              return;
            done(job->error() == QKeychain::NoError, job->errorString());
          });
  job->start();
}

void PasswordManager::deleteSecret(const QString &id) {
  auto *job = new QKeychain::DeletePasswordJob(service(), this);
  job->setAutoDelete(true);
  job->setKey(keyFor(id));
  connect(job, &QKeychain::Job::finished, this, [job](QKeychain::Job *) {
    if (job->error() != QKeychain::NoError &&
        job->error() != QKeychain::EntryNotFound)
      qWarning() << "[passwords] keyring delete failed:" << job->errorString();
  });
  job->start();
}

/* ---- public API ---- */

QVariantList PasswordManager::list(const QString &search) {
  QString sql = QStringLiteral(
      "SELECT id, origin, username, updated_at FROM passwords ");
  QVariantMap bindings;
  if (!search.trimmed().isEmpty()) {
    sql += QStringLiteral("WHERE origin LIKE :s OR username LIKE :s ");
    bindings.insert(QStringLiteral(":s"),
                    QStringLiteral("%") + search.trimmed() + QStringLiteral("%"));
  }
  sql += QStringLiteral(
      "ORDER BY origin COLLATE NOCASE ASC, username COLLATE NOCASE ASC;");
  return ProfileManager::instance()->database()->query(sql, bindings);
}

void PasswordManager::save(const QString &rawOrigin, const QString &username,
                           const QString &password) {
  const QString origin = normalizeOrigin(rawOrigin);
  if (origin.isEmpty() || password.isEmpty())
    return;

  const QVariantList rows = ProfileManager::instance()->database()->query(
      QStringLiteral(
          "SELECT id FROM passwords WHERE origin = :o AND username = :u LIMIT 1;"),
      {{QStringLiteral(":o"), origin}, {QStringLiteral(":u"), username}});

  const bool exists = !rows.isEmpty();
  const QString id =
      exists ? rows.first().toMap().value(QStringLiteral("id")).toString()
             : QUuid::createUuid().toString(QUuid::WithoutBraces);

  writeSecret(id, password, this,
              [this, exists, id, origin, username](bool ok, const QString &err) {
                if (!ok) {
                  emit keychainError(err);
                  return;
                }
                Database *d = ProfileManager::instance()->database();
                const qint64 now = QDateTime::currentSecsSinceEpoch();
                if (exists) {
                  d->execute(QStringLiteral("UPDATE passwords SET updated_at = :t "
                                            "WHERE id = :id;"),
                             {{QStringLiteral(":t"), now},
                              {QStringLiteral(":id"), id}});
                } else {
                  d->execute(
                      QStringLiteral(
                          "INSERT INTO passwords (id, origin, username, "
                          "created_at, updated_at) "
                          "VALUES (:id, :o, :u, :t, :t);"),
                      {{QStringLiteral(":id"), id},
                       {QStringLiteral(":o"), origin},
                       {QStringLiteral(":u"), username},
                       {QStringLiteral(":t"), now}});
                }
                emit passwordsChanged();
              });
}

void PasswordManager::remove(const QString &id) {
  deleteSecret(id);
  ProfileManager::instance()->database()->execute(
      QStringLiteral("DELETE FROM passwords WHERE id = :id;"),
      {{QStringLiteral(":id"), id}});
  emit passwordsChanged();
}

void PasswordManager::reveal(const QString &id) {
  readSecret(id, this, [this, id](bool ok, const QString &v) {
    if (ok)
      emit passwordRevealed(id, v);
    else
      emit keychainError(v);
  });
}

void PasswordManager::copyPassword(const QString &id) {
  readSecret(id, this, [this](bool ok, const QString &v) {
    if (!ok) {
      emit keychainError(v);
      return;
    }
    QGuiApplication::clipboard()->setText(v);
    QTimer::singleShot(30000, this, [v]() {
      QClipboard *c = QGuiApplication::clipboard();
      if (c->text() == v)
        c->clear();
    });
  });
}

void PasswordManager::classifyCapture(const QString &origin,
                                      const QString &username,
                                      const QString &password, QObject *ctx,
                                      std::function<void(CaptureKind)> done) {
  const QVariantList rows = ProfileManager::instance()->database()->query(
      QStringLiteral(
          "SELECT id FROM passwords WHERE origin = :o AND username = :u LIMIT 1;"),
      {{QStringLiteral(":o"), origin}, {QStringLiteral(":u"), username}});

  if (rows.isEmpty()) {
    done(CaptureKind::New);
    return;
  }

  const QString id = rows.first().toMap().value(QStringLiteral("id")).toString();
  readSecret(id, ctx, [done, password](bool ok, const QString &stored) {
    done(ok && stored == password ? CaptureKind::Ignore : CaptureKind::Update);
  });
}

void PasswordManager::credentialsFor(
    const QString &origin, QObject *ctx,
    std::function<void(const QString &, const QString &)> done) {
  const QVariantList rows = ProfileManager::instance()->database()->query(
      QStringLiteral("SELECT id, username FROM passwords WHERE origin = :o "
                     "ORDER BY COALESCE(last_used_at, updated_at) DESC LIMIT 1;"),
      {{QStringLiteral(":o"), origin}});
  if (rows.isEmpty())
    return;

  const QVariantMap row = rows.first().toMap();
  const QString id = row.value(QStringLiteral("id")).toString();
  const QString user = row.value(QStringLiteral("username")).toString();

  readSecret(id, ctx, [id, user, done](bool ok, const QString &pw) {
    if (!ok)
      return;
    ProfileManager::instance()->database()->execute(
        QStringLiteral("UPDATE passwords SET last_used_at = :t WHERE id = :id;"),
        {{QStringLiteral(":t"), qint64(QDateTime::currentSecsSinceEpoch())},
         {QStringLiteral(":id"), id}});
    done(user, pw);
  });
}

/* ======================= PasswordBridge ======================= */

PasswordBridge::PasswordBridge(QObject *parent) : QObject(parent) {}

void PasswordBridge::submitted(const QString &rawOrigin, const QString &username,
                               const QString &password) {
  const QString origin = PasswordManager::normalizeOrigin(rawOrigin);
  if (origin.isEmpty() || password.isEmpty() || password.size() > 4096 ||
      username.size() > 512)
    return;

  PasswordManager::instance()->classifyCapture(
      origin, username, password, this,
      [this, origin, username, password](PasswordManager::CaptureKind kind) {
        if (kind == PasswordManager::CaptureKind::Ignore)
          return;
        m_pending.origin = origin;
        m_pending.username = username;
        m_pending.password = password;
        m_pending.active = true;
        emit saveOffered(QUrl(origin).authority(), username,
                         kind == PasswordManager::CaptureKind::Update);
      });
}

void PasswordBridge::requestFill(const QString &rawOrigin) {
  const QString origin = PasswordManager::normalizeOrigin(rawOrigin);
  if (origin.isEmpty() || !PasswordManager::autofillAllowed(origin))
    return;

  PasswordManager::instance()->credentialsFor(
      origin, this, [this, origin](const QString &user, const QString &pass) {
        emit fill(origin, user, pass);
      });
}

void PasswordBridge::acceptPending() {
  if (!m_pending.active)
    return;
  PasswordManager::instance()->save(m_pending.origin, m_pending.username,
                                    m_pending.password);
  discardPending();
}

void PasswordBridge::discardPending() { m_pending = {}; }
