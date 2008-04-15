/*
 * This file is part of the Geoid and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#include "treemodel.h"
#include "treeitem.h"
#include "glwidget3d.h"
#include "mfolder.h"
#include "renderer.h"
#include "mabstractview.h"

TreeModel::TreeModel(QObject *parent)
    : QAbstractItemModel(parent)
{
	rootItem = new MFolder();
}

TreeModel::~TreeModel()
{
	delete rootItem;
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
	return 1;
}

bool TreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (!index.isValid())
		return false;
	TreeItem *item = itemFromIndex(index);
	if (item)
	{
		item->setData(index.column(), role, value);
		if (role == Qt::CheckStateRole)
		{
			m_currentView->calculateBorders();
			m_currentView->draw();
		}
		emit dataChanged(QModelIndex(), QModelIndex());
		return true;
	}
	
	return false;
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	TreeItem *item = itemFromIndex(index);
	return item->data(role);
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
	return Qt::ItemIsDragEnabled | Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable | Qt::ItemIsDropEnabled;
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		if (section == 0)
			return tr("Objects list");
	return QVariant();
}


QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
	TreeItem *parentItem;

	if (!parent.isValid())
		parentItem = rootItem;
	else
		parentItem = static_cast<TreeItem*>(parent.internalPointer());

	TreeItem *childItem = parentItem->child(row);
	if (childItem)
		return createIndex(row, column, childItem);
	else
		return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
	if (!index.isValid())
		return QModelIndex();

	TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
	TreeItem *parentItem = childItem->parent();

	if (parentItem == rootItem)
		return QModelIndex();

	return createIndex(parentItem->row(), 0, parentItem);
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
	TreeItem *parentItem;

	if (!parent.isValid())
		parentItem = rootItem;
	else
		parentItem = static_cast<TreeItem*>(parent.internalPointer());

	return parentItem->childCount();
}

TreeItem *TreeModel::itemFromIndex(const QModelIndex &index) const
{
	if (index.isValid())
		return static_cast<TreeItem *>(index.internalPointer());
	else
		return rootItem;
}

bool TreeModel::appendTopLevelItem(TreeItem *item) 
{
	beginInsertRows(QModelIndex(), 0, 0);
	rootItem->appendChild(item);
	endInsertRows();
	return true;
}
void TreeModel::add(MObject *object)
{
	appendTopLevelItem(object);
	object->render(m_currentView->renderer());
}

void TreeModel::draw(Renderer *renderer)
{
	rootItem->draw(renderer);
}

void TreeModel::calculateBorders(BorderStruct &borders)
{
	rootItem->calculateBorders(borders);
}

void TreeModel::setCurrentView(MAbstractView *currentView)
{
	m_currentView = currentView;
}
