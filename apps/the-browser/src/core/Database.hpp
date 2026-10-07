#pragma once

#include <QObject>
#include <QSqlDatabase>
#include <QString>
#include <QVariantList>
#include <QVariantMap>

class Database : public QObject {
  Q_OBJECT

public:
  explicit Database(QObject *parent = nullptr);
  ~Database() override;

  bool initialize(const QString &dbPath);
  void close();

  bool execute(const QString &queryStr, const QVariantMap &bindings = {});
  QVariantList query(const QString &queryStr, const QVariantMap &bindings = {});

private:
  QString m_connectionName;
  bool m_open{false};
  bool runMigrations();
};
