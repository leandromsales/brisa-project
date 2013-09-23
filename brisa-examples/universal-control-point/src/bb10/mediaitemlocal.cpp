#include "mediaitemlocal.h"

QHash<int, QByteArray> MediaItemLocal::roleNames() const
{
  QHash<int, QByteArray> names;
  names[NameRole] = "name";
  names[UUIDRole] = "uuid";
  names[IconUrlRole] = "iconUrl";
  return names;
}

QVariant MediaItemLocal::data(int role) const
{
  switch(role) {
  case NameRole:
    return name();
  case UUIDRole:
    return uuid();
  case IconUrlRole:
    return iconUrl();
  default:
    return QVariant();
  }
}
