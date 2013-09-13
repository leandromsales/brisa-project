#include "mediacontentitem.h"

MediaContentItem::MediaContentItem(const QString &objectId, const QString &parentId, const QString &title, const QString &resource, const bool &container, QObject *parent)
    : ListItem(parent), m_objectId(objectId), m_parentId(parentId), m_title(title), m_resource(resource), m_container(container)
{
}

QHash<int, QByteArray> MediaContentItem::roleNames() const
{
    QHash<int, QByteArray> names;
    names[ObjectIdRole] = "objectId";
    names[ParentIdRole] = "parentId";
    names[TitleRole] = "title";
    names[ResourceRole] = "resource";
    names[ContainerRole] = "container";
    return names;
}

QVariant MediaContentItem::data(int role) const
{
    switch(role) {
    case ObjectIdRole:
        return objectId();
    case ParentIdRole:
        return parentId();
    case TitleRole:
        return title();
    case ResourceRole:
        return resource();
    case ContainerRole:
        return isContainer();
    default:
        return QVariant();
    }
}
