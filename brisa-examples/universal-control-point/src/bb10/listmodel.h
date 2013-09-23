/*
 * Author: Christophe Dumez <dchris@gmail.com>
 * License: Public domain (No attribution required)
 * Website: http://cdumez.blogspot.com/
 * Version: 1.0
 */

#ifndef LISTMODEL_H
#define LISTMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QVariant>
#include <bb/cascades/DataModel>
#include <bb/cascades/ArrayDataModel>


using namespace bb::cascades;

class ListItem: public QObject {
	Q_OBJECT

public:
	ListItem(QObject* parent = 0) : QObject(parent) { }
	virtual ~ListItem() {}
	virtual QString id() const = 0;
	virtual QVariant data(int role) const = 0;
	virtual QHash<int, QByteArray> roleNames() const = 0;

	signals:
	void dataChanged();
};

class ListModel : public DataModel
{
	Q_OBJECT

public:
	explicit ListModel(ListItem* prototype, QObject* parent = 0);
	~ListModel();
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	void appendRow(ListItem* item);
	void appendRows(const QList<ListItem*> &items);
	void insertRow(int row, ListItem* item);
	bool removeRow(int row, const QModelIndex &parent = QModelIndex());
	bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
	ListItem* takeRow(int row);
	ListItem* find(const QString &id) const;
	QModelIndex indexFromItem( const ListItem* item) const;
	void clear();
	ListItem* removeById(const QString &id);
	QHash<int, QByteArray> roleNames() {
		return m_prototype->roleNames();
	}

	/*
	 * Data model override
	 */

	virtual int childCount(const QVariantList &indexPath)
	{
		return mArrayDataModel.childCount(indexPath);
	}
	virtual bool hasChildren(const QVariantList &indexPath)
	{
		return mArrayDataModel.hasChildren(indexPath);
	}
	virtual QString itemType(const QVariantList &indexPath)
	{
		if (indexPath.length() == 1) {
			return QString();
		} else {
			return QString::null;
		}
	}
	virtual QVariant data(const QVariantList &indexPath)
	{
		return mArrayDataModel.data(indexPath);
	}

private slots:
void handleItemChange();

private:
ListItem* m_prototype;
QList<ListItem*> m_list;

ArrayDataModel mArrayDataModel;
};

#endif // LISTMODEL_H
