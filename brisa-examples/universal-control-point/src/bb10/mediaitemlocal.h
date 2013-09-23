#ifndef MEDIAITEMLOCAL_H
#define MEDIAITEMLOCAL_H

#include "listmodel.h"

class MediaItemLocal : public ListItem
{
   Q_OBJECT
   public:
     enum Roles {
       NameRole = Qt::UserRole+1,
       UUIDRole,
       IconUrlRole
     };

   public:
     explicit MediaItemLocal(const QString name, QObject *parent = 0): ListItem(parent) { this->m_name = name; }
     QVariant data(int role) const;
     QHash<int, QByteArray> roleNames() const;
     inline QString id() const {
         return "";
     }
     inline QString name() const {
         return this->m_name;
     }
     inline QString uuid() const {
         return "local";
     }
     inline QString iconUrl() const {
         return "";
     }

   private:
     QString m_name;
   };

#endif // MEDIAITEMLOCAL_H
