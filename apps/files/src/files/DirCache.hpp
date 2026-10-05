#pragma once
#include "type.hpp"
#include <QVector>
#include <qdatetime.h>
#include <qtypes.h>

struct CachedListing {
  QVector<ItemEntery> entries;
  qint64 dirMtime;
};
