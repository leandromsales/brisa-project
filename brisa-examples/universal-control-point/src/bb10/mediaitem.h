#ifndef MEDIAITEM_H
#define MEDIAITEM_H

#include "listmodel.h"
#include <upnp/controlpoint/device.h>

class MediaItem : public ListItem
{
    Q_OBJECT
   public:
     enum Roles {
       NameRole = Qt::UserRole+1,
       UUIDRole,
       IconUrlRole
     };

   public:
     explicit MediaItem(QObject *parent = 0): ListItem(parent) { m_device = NULL; }
     explicit MediaItem(Device * dev, QObject *parent = 0): ListItem(parent), m_device(dev) {}
     QVariant data(int role) const;
     QHash<int, QByteArray> roleNames() const;
     inline QString id() const {
         if(m_device) {
            return m_device->getAttribute("uuid");
         }
         return "00000000-0000-0000-0000-000000000000";
     }
     inline QString name() const {
         if(m_device) {
            return m_device->getAttribute("friendlyName");
         }
         return "Empty list";
     }
     inline QString uuid() const {
         if(m_device) {
            return m_device->getAttribute("uuid");
         }
         return "00000000-0000-0000-0000-000000000000";
     }
     inline QString iconUrl() const {
         if(m_device) {
             Icon * icon = m_device->getIcon("image/png_48x48");
             if (icon) {
                 return icon->getUrl().toString();
             } else {
                 Icon * icon = m_device->getIcon("image/png_32x32");
                 if (icon) {
                     return icon->getUrl().toString();
                 }
             }
         }
         return "";
     }

     Device * getDevice() {
        return m_device;
     }

   private:
     Device * m_device;
   };
#endif // MEDIAITEM_H
