#include "ExtensionManager.hpp"
#include "ProfileManager.hpp"

#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRegularExpression>
#include <QSet>
#include <QUrl>
#include <QUuid>

namespace {

constexpr qint64 kMaxFile = 5 * 1024 * 1024;
constexpr quint64 kMaxExtensionSize = 20 * 1024 * 1024;

Database *db() { return ProfileManager::instance()->database(); }

bool validId(const QString &id) { return !QUuid::fromString(id).isNull(); }

QJsonObject readJsonFile(const QString &path) {
  QFile f(path);
  if (!f.open(QIODevice::ReadOnly) || f.size() > 1024 * 1024)
    return {};
  QByteArray data = f.readAll();
  if (data.startsWith("\xEF\xBB\xBF"))
    data.remove(0, 3);
  QJsonParseError pe;
  const QJsonDocument doc = QJsonDocument::fromJson(data, &pe);
  return pe.error == QJsonParseError::NoError ? doc.object() : QJsonObject();
}

bool readInside(const QString &root, const QString &rel, QString *out) {
  const QString clean = QDir::cleanPath(root + QLatin1Char('/') + rel);
  if (!clean.startsWith(root + QLatin1Char('/')))
    return false;
  const QFileInfo fi(clean);
  if (!fi.isFile() || fi.size() > kMaxFile)
    return false;
  QFile f(clean);
  if (!f.open(QIODevice::ReadOnly))
    return false;
  *out = QString::fromUtf8(f.readAll());
  return true;
}

QString localized(const QString &dir, const QJsonObject &manifest,
                  const QString &text) {
  if (!text.startsWith(QLatin1String("__MSG_")) ||
      !text.endsWith(QLatin1String("__")))
    return text;
  const QString key = text.mid(6, text.size() - 8).toLower();
  const QString loc =
      manifest.value(QStringLiteral("default_locale")).toString();
  if (loc.isEmpty())
    return text;
  const QJsonObject msgs = readJsonFile(dir + QStringLiteral("/_locales/") +
                                        loc + QStringLiteral("/messages.json"));
  for (auto it = msgs.begin(); it != msgs.end(); ++it)
    if (it.key().toLower() == key)
      return it.value()
          .toObject()
          .value(QStringLiteral("message"))
          .toString(text);
  return text;
}

QString matchToRegex(const QString &pattern) {
  if (pattern == QLatin1String("<all_urls>"))
    return QStringLiteral("^(https?|ftp|file)://.*$");

  static const QRegularExpression re(
      QStringLiteral("^(\\*|https?|file|ftp)://([^/]*)(/.*)$"));
  const QRegularExpressionMatch m = re.match(pattern);
  if (!m.hasMatch())
    return {};

  const QString scheme = m.captured(1), host = m.captured(2),
                path = m.captured(3);
  QString out = QStringLiteral("^");
  out += scheme == QLatin1String("*") ? QStringLiteral("https?")
                                      : QRegularExpression::escape(scheme);
  out += QStringLiteral("://");

  if (scheme == QLatin1String("file")) {
    if (!host.isEmpty())
      return {};
  } else {
    if (host.isEmpty())
      return {};
    if (host == QLatin1String("*"))
      out += QStringLiteral("[^/]+");
    else if (host.startsWith(QLatin1String("*.")))
      out += QStringLiteral("([^/]+\\.)?") +
             QRegularExpression::escape(host.mid(2));
    else if (host.contains(QLatin1Char('*')))
      return {};
    else
      out += QRegularExpression::escape(host);
    out += QStringLiteral("(:\\d+)?");
  }

  QString p = QRegularExpression::escape(path);
  p.replace(QStringLiteral("\\*"), QStringLiteral(".*"));
  return out + p + QStringLiteral("$");
}

QStringList regexes(const QJsonArray &patterns) {
  QStringList out;
  for (const QJsonValue &v : patterns) {
    const QString r = matchToRegex(v.toString());
    if (!r.isEmpty())
      out << r;
  }
  return out;
}

QString jsonArray(const QStringList &l) {
  return QString::fromUtf8(QJsonDocument(QJsonArray::fromStringList(l))
                               .toJson(QJsonDocument::Compact));
}

QString jsonString(const QString &s) {
  const QString arr = QString::fromUtf8(
      QJsonDocument(QJsonArray{s}).toJson(QJsonDocument::Compact));
  return arr.mid(1, arr.size() - 2);
}

bool copyTree(const QString &src, const QString &dst, quint64 &budget) {
  const QFileInfoList entries =
      QDir(src).entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries |
                              QDir::NoSymLinks | QDir::Hidden);
  for (const QFileInfo &fi : entries) {
    const QString target = dst + QLatin1Char('/') + fi.fileName();
    if (fi.isDir()) {
      if (!QDir().mkpath(target) ||
          !copyTree(fi.absoluteFilePath(), target, budget))
        return false;
    } else if (fi.isFile()) {
      if (quint64(fi.size()) > budget)
        return false;
      budget -= quint64(fi.size());
      if (!QFile::copy(fi.absoluteFilePath(), target))
        return false;
    }
  }
  return true;
}

bool hasUsableContentScripts(const QJsonObject &m) {
  for (const QJsonValue &v :
       m.value(QStringLiteral("content_scripts")).toArray()) {
    const QJsonObject o = v.toObject();
    if (!o.value(QStringLiteral("matches")).toArray().isEmpty() &&
        (!o.value(QStringLiteral("js")).toArray().isEmpty() ||
         !o.value(QStringLiteral("css")).toArray().isEmpty()))
      return true;
  }
  return false;
}

} // namespace

ExtensionManager *ExtensionManager::instance() {
  static ExtensionManager *s =
      new ExtensionManager(QCoreApplication::instance());
  return s;
}

ExtensionManager *ExtensionManager::create(QQmlEngine *, QJSEngine *) {
  ExtensionManager *m = instance();
  QJSEngine::setObjectOwnership(m, QJSEngine::CppOwnership);
  return m;
}

ExtensionManager::ExtensionManager(QObject *parent) : QObject(parent) {
  connect(ProfileManager::instance(), &ProfileManager::activeProfileChanged,
          this, [this]() {
            rebuild();
            emit extensionsChanged();
          });
  rebuild();
}

QString ExtensionManager::rootDir() const {
  return QDir::cleanPath(ProfileManager::instance()->profilePath() +
                         QStringLiteral("/extensions"));
}

void ExtensionManager::rebuild() {
  QVariantList out;
  const QVariantList rows =
      db()->query(QStringLiteral("SELECT id FROM extensions WHERE enabled = 1 "
                                 "ORDER BY installed_at ASC;"));

  for (const QVariant &r : rows) {
    const QString id = r.toMap().value(QStringLiteral("id")).toString();
    if (!validId(id))
      continue;
    const QString dir = rootDir() + QLatin1Char('/') + id;
    const QJsonObject manifest =
        readJsonFile(dir + QStringLiteral("/manifest.json"));
    if (manifest.isEmpty())
      continue;

    const QString manifestJs = QString::fromUtf8(
        QJsonDocument(manifest).toJson(QJsonDocument::Compact));
    const QJsonArray entries =
        manifest.value(QStringLiteral("content_scripts")).toArray();

    for (int i = 0; i < entries.size(); ++i) {
      const QJsonObject e = entries.at(i).toObject();
      const QStringList inc =
          regexes(e.value(QStringLiteral("matches")).toArray());
      const QStringList exc =
          regexes(e.value(QStringLiteral("exclude_matches")).toArray());
      if (inc.isEmpty())
        continue;

      QStringList jsParts, cssParts;
      for (const QJsonValue &f : e.value(QStringLiteral("js")).toArray()) {
        QString code;
        if (readInside(dir, f.toString(), &code))
          jsParts << code;
      }
      for (const QJsonValue &f : e.value(QStringLiteral("css")).toArray()) {
        QString css;
        if (readInside(dir, f.toString(), &css))
          cssParts << css;
      }
      if (jsParts.isEmpty() && cssParts.isEmpty())
        continue;

      QString src;
      src += QStringLiteral("(function(){\n");
      src += QStringLiteral("var inc=") + jsonArray(inc) +
             QStringLiteral(",exc=") + jsonArray(exc) + QStringLiteral(";\n");
      src += QStringLiteral("var href=location.href.split('#')[0];\n");
      src += QStringLiteral(
          "function t(l){for(var i=0;i<l.length;i++){try{"
          "if(new RegExp(l[i]).test(href))return true;}catch(e){}}"
          "return false;}\n");
      src += QStringLiteral("if(!t(inc)||t(exc))return;\n");
      src += QStringLiteral("var chrome={runtime:{id:") + jsonString(id) +
             QStringLiteral(",getManifest:function(){return ") + manifestJs +
             QStringLiteral(";}}};var browser=chrome;\n");
      if (!cssParts.isEmpty()) {
        src += QStringLiteral(
            "(function(css){var "
            "s=document.createElement('style');s.textContent=css;"
            "function add(){var r=document.head||document.documentElement;"
            "if(!r)return false;r.appendChild(s);return true;}"
            "if(!add()){var mo=new "
            "MutationObserver(function(){if(add())mo.disconnect();});"
            "mo.observe(document,{childList:true});}})(");
        src += jsonString(cssParts.join(QLatin1Char('\n')));
        src += QStringLiteral(");\n");
      }
      src += jsParts.join(QStringLiteral("\n;\n"));
      src += QStringLiteral("\n})();\n");

      const QString runAt = e.value(QStringLiteral("run_at")).toString();
      QVariantMap script;
      script.insert(QStringLiteral("name"),
                    QStringLiteral("ext:%1:%2").arg(id).arg(i));
      script.insert(QStringLiteral("sourceCode"), src);
      script.insert(QStringLiteral("runAt"),
                    runAt == QLatin1String("document_start")
                        ? QStringLiteral("start")
                        : (runAt == QLatin1String("document_end")
                               ? QStringLiteral("end")
                               : QStringLiteral("idle")));
      script.insert(QStringLiteral("allFrames"),
                    e.value(QStringLiteral("all_frames")).toBool(false));
      script.insert(QStringLiteral("world"),
                    e.value(QStringLiteral("world")).toString() ==
                            QLatin1String("MAIN")
                        ? QStringLiteral("main")
                        : QStringLiteral("isolated"));
      out.append(script);
    }
  }

  m_scripts = out;
  emit scriptsChanged();
}

QVariantList ExtensionManager::list() {
  QVariantList result;
  const QVariantList rows = db()->query(QStringLiteral(
      "SELECT id, enabled FROM extensions ORDER BY installed_at ASC;"));

  for (const QVariant &r : rows) {
    const QVariantMap row = r.toMap();
    const QString id = row.value(QStringLiteral("id")).toString();
    const QString dir = rootDir() + QLatin1Char('/') + id;
    const QJsonObject m = readJsonFile(dir + QStringLiteral("/manifest.json"));

    QVariantMap e;
    e.insert(QStringLiteral("id"), id);
    e.insert(QStringLiteral("enabled"),
             row.value(QStringLiteral("enabled")).toInt() == 1);

    if (m.isEmpty()) {
      e.insert(QStringLiteral("name"), QStringLiteral("Unreadable extension"));
      e.insert(QStringLiteral("version"), QString());
      e.insert(QStringLiteral("description"), QString());
      e.insert(QStringLiteral("runsOn"), QString());
      e.insert(QStringLiteral("iconUrl"), QString());
      e.insert(QStringLiteral("error"),
               QStringLiteral("manifest.json is missing or invalid."));
      result.append(e);
      continue;
    }

    QStringList hosts;
    QSet<QString> seen;
    for (const QJsonValue &v :
         m.value(QStringLiteral("content_scripts")).toArray())
      for (const QJsonValue &p :
           v.toObject().value(QStringLiteral("matches")).toArray())
        if (!seen.contains(p.toString())) {
          seen.insert(p.toString());
          hosts << p.toString();
        }
    QString runsOn = hosts.mid(0, 4).join(QStringLiteral(", "));
    if (hosts.size() > 4)
      runsOn += QStringLiteral(" +%1 more").arg(hosts.size() - 4);

    QString iconUrl;
    int best = -1;
    const QJsonObject icons = m.value(QStringLiteral("icons")).toObject();
    for (auto it = icons.begin(); it != icons.end(); ++it) {
      const int size = it.key().toInt();
      const QString clean =
          QDir::cleanPath(dir + QLatin1Char('/') + it.value().toString());
      if (size > best && clean.startsWith(dir + QLatin1Char('/')) &&
          QFileInfo(clean).isFile()) {
        best = size;
        iconUrl = QUrl::fromLocalFile(clean).toString();
      }
    }

    e.insert(QStringLiteral("name"),
             localized(dir, m,
                       m.value(QStringLiteral("name"))
                           .toString(QStringLiteral("Unnamed"))));
    e.insert(QStringLiteral("version"),
             m.value(QStringLiteral("version")).toString());
    e.insert(
        QStringLiteral("description"),
        localized(dir, m, m.value(QStringLiteral("description")).toString()));
    e.insert(QStringLiteral("runsOn"), runsOn);
    e.insert(QStringLiteral("iconUrl"), iconUrl);
    e.insert(QStringLiteral("error"), QString());
    result.append(e);
  }
  return result;
}

QString ExtensionManager::installFromFolder(const QString &folder) {
  const QUrl u(folder);
  const QString src =
      QDir::cleanPath(u.isLocalFile() ? u.toLocalFile() : folder);
  if (!QFileInfo(src).isDir())
    return QStringLiteral("That is not a folder.");

  const QJsonObject m = readJsonFile(src + QStringLiteral("/manifest.json"));
  if (m.isEmpty())
    return QStringLiteral("No valid manifest.json found in that folder.");
  const int mv = m.value(QStringLiteral("manifest_version")).toInt();
  if (mv != 2 && mv != 3)
    return QStringLiteral("Unsupported manifest_version (expected 2 or 3).");
  if (!hasUsableContentScripts(m))
    return QStringLiteral(
        "This extension has no content_scripts. Only content-script "
        "extensions are supported.");

  const QString id = QUuid::createUuid().toString(QUuid::WithoutBraces);
  const QString dst = rootDir() + QLatin1Char('/') + id;
  if (!QDir().mkpath(dst))
    return QStringLiteral("Could not create the extension folder.");

  quint64 budget = kMaxExtensionSize;
  if (!copyTree(src, dst, budget)) {
    QDir(dst).removeRecursively();
    return QStringLiteral(
        "The extension is larger than 20 MB or could not be copied.");
  }

  db()->execute(
      QStringLiteral("INSERT INTO extensions (id, enabled, installed_at) "
                     "VALUES (:id, 1, :t);"),
      {{QStringLiteral(":id"), id},
       {QStringLiteral(":t"), qint64(QDateTime::currentSecsSinceEpoch())}});
  rebuild();
  emit extensionsChanged();
  return {};
}

void ExtensionManager::setEnabled(const QString &id, bool enabled) {
  if (!validId(id))
    return;
  db()->execute(
      QStringLiteral("UPDATE extensions SET enabled = :e WHERE id = :id;"),
      {{QStringLiteral(":e"), enabled ? 1 : 0}, {QStringLiteral(":id"), id}});
  rebuild();
  emit extensionsChanged();
}

void ExtensionManager::remove(const QString &id) {
  if (!validId(id))
    return;
  db()->execute(QStringLiteral("DELETE FROM extensions WHERE id = :id;"),
                {{QStringLiteral(":id"), id}});
  QDir(rootDir() + QLatin1Char('/') + id).removeRecursively();
  rebuild();
  emit extensionsChanged();
}
