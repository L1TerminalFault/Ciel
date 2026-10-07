#pragma once
#include <QString>
#include <qstringview.h>
#include <qtypes.h>
#include <qvariant.h>

struct ItemEntery {
  QByteArray name = "";
  bool isDir = false;
  qint64 size = 0;
  qint64 modified = 0;
  bool isInaccessible = false;
  bool isSymLink = false;
  bool isSelected = false;
  int metadataError = -1;
};

Q_DECLARE_METATYPE(ItemEntery)
