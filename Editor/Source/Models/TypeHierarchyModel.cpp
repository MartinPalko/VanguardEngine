#include "TypeHierarchyModel.h"

#pragma warning(disable : 4267)

TypeHierarchyModel::TypeHierarchyModel(Vanguard::Type* aRoot, QObject *parent) : QAbstractItemModel(parent)
, root(aRoot)
{
}

TypeHierarchyModel::~TypeHierarchyModel()
{
}

QModelIndex TypeHierarchyModel::index(int row, int column, const QModelIndex &parent) const
{
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	if (!parent.isValid()) // Root item
	{
		if (root == nullptr)
			return createIndex(row, column, Vanguard::Type::GetAllBaseTypes()[row]);
		else
			return createIndex(row, column, root->GetDerivedClasses()[row]);
	}
	else
	{
		Vanguard::Type* parentItem = static_cast<Vanguard::Type*>(parent.internalPointer());

		Vanguard::Type* childItem = parentItem->GetDerivedClasses()[row];
		if (childItem)
			return createIndex(row, column, childItem);
		else
			return QModelIndex();
	}
}

QModelIndex TypeHierarchyModel::parent(const QModelIndex &index) const
{
	if (!index.isValid())
		return QModelIndex();

	Vanguard::Type *childItem = static_cast<Vanguard::Type*>(index.internalPointer());
	Vanguard::Type *parentItem = childItem->GetBaseClass();

	if (parentItem == root)
		return QModelIndex();

	int row = 0;
	if (parentItem->GetBaseClass())
	{
		row = parentItem->GetBaseClass()->GetDerivedClasses().IndexOf(parentItem);
	}
	else
	{
		row = Vanguard::Type::GetAllBaseTypes().IndexOf(parentItem);
	}

	return createIndex(row, 0, parentItem);
}

int TypeHierarchyModel::rowCount(const QModelIndex &parent) const
{
	if (parent.column() > 0)
		return 0;

	if (!parent.isValid())
	{
		if (root == nullptr)
			return Vanguard::Type::GetAllBaseTypes().Count();
		else
			return root->GetDerivedClasses().Count();
	}
	else
	{
		Vanguard::Type* parentItem = static_cast<Vanguard::Type*>(parent.internalPointer());
		return parentItem->GetDerivedClasses().Count();
	}
}

int TypeHierarchyModel::columnCount(const QModelIndex &parent) const
{
	return 1;
}

QVariant TypeHierarchyModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (role != Qt::DisplayRole)
		return QVariant();

	Vanguard::Type *item = static_cast<Vanguard::Type*>(index.internalPointer());

	return item->GetTypeName().GetCharPointer();
}

Qt::ItemFlags TypeHierarchyModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return 0;

	return QAbstractItemModel::flags(index);
}

QVariant TypeHierarchyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
	{
		if (section == 0)
			return "Type";
	}

	return QVariant();
}