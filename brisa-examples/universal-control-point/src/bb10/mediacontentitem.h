#ifndef MEDIACONTENTITEM_H
#define MEDIACONTENTITEM_H

#include "listmodel.h"

class MediaContentItem : public ListItem
{Q_OBJECT
public:
    enum Roles {
        ObjectIdRole = Qt::UserRole+1,
        ParentIdRole,
        TitleRole,
        ResourceRole,
        ContainerRole
    };

public:
    MediaContentItem(QObject *parent = 0): ListItem(parent) {}
    explicit MediaContentItem(const QString &objectId, const QString &parentId, const QString &title, const QString &resource, const bool &container, QObject *parent = 0);
    QVariant data(int role) const;
    QHash<int, QByteArray> roleNames() const;
    inline QString objectId() const { return m_objectId; }
    inline QString id() const { return m_objectId; }
    inline QString parentId() const { return m_parentId; }
    inline QString title() const { return m_title; }
    inline QString resource() const { return m_resource; }
    inline bool isContainer() const {return m_container; }

private:
    QString m_objectId;
    QString m_parentId;
    QString m_title;
    QString m_resource;
    bool m_container;
};
#endif // MEDIACONTENTITEM_H
