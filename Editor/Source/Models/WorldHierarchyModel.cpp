#include "WorldHierarchyModel.h"
#include "EditorCore.h"

bool WorldHierarchyModel::IsWorld(void* ptr) const
{
	return Vanguard::EditorCore::GetInstance()->GetWorlds().Contains((Vanguard::World*)ptr);
}

WorldHierarchyModel::WorldHierarchyModel(Vanguard::World* aWorld, QObject *parent) : QAbstractItemModel(parent)
, rootWorld(aWorld)
{
	if (rootWorld)
	{
		rootWorld->RegisterEventListener(this);
	}
	else
	{
		for (Vanguard::World* world : Vanguard::EditorCore::GetInstance()->GetWorlds())
			world->RegisterEventListener(this);
	}
}

WorldHierarchyModel::~WorldHierarchyModel()
{
	if (rootWorld)
	{
		rootWorld->UnregisterEventListener(this);
	}
	else
	{
		for (Vanguard::World* world : Vanguard::EditorCore::GetInstance()->GetWorlds())
			world->UnregisterEventListener(this);
	}
}

void WorldHierarchyModel::OnWorldEvent(Vanguard::WorldEvent* aEvent)
{
	if (aEvent->GetType()->IsA<Vanguard::ObjectAddedEvent>() || aEvent->GetType()->IsA<Vanguard::ObjectRemovedEvent>())
	{
		// TODO: Update only the part of the model that needs to be updated, instead of resetting the whole thing.
		beginResetModel();
		endResetModel();
	}
}

QModelIndex WorldHierarchyModel::index(int row, int column, const QModelIndex &parent) const
{
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	if (!parent.isValid()) // Root item
	{
		if (rootWorld == nullptr)
			return createIndex(row, column, Vanguard::EditorCore::GetInstance()->GetWorlds()[row]);
		else
			return createIndex(row, column, rootWorld->GetInstances<Vanguard::Entity>()[row]);
	}
	else
	{
		if (IsWorld(parent.internalPointer()))
		{
			Vanguard::World* parentItem = static_cast<Vanguard::World*>(parent.internalPointer());
			return createIndex(row, column, parentItem->GetInstances<Vanguard::Entity>()[row]);
		}

		Vanguard::Entity* parentItem = static_cast<Vanguard::Entity*>(parent.internalPointer());

		Vanguard::Entity* childItem = parentItem->GetChild(row);
		if (childItem)
			return createIndex(row, column, childItem);
		else
			return QModelIndex();
	}
}

QModelIndex WorldHierarchyModel::parent(const QModelIndex &index) const
{
	if (!index.isValid())
		return QModelIndex();

	if (index.internalPointer() == rootWorld)
		return QModelIndex();

	if (IsWorld(index.internalPointer()))
	{
		// Index is a world
		return QModelIndex();
	}

	Vanguard::Entity* entity = static_cast<Vanguard::Entity*>(index.internalPointer());
	Vanguard::Entity* parentEntity = entity->GetParent();

	if (!parentEntity) // Parent is world
	{
		Vanguard::World* entityWorld = entity->GetWorld();
		int row = Vanguard::EditorCore::GetInstance()->GetWorlds().IndexOf(entityWorld); // Get which world index
		return createIndex(row, 0, entityWorld);
	}

	Vanguard::Entity* parentsParentEntity = parentEntity->GetParent();
	
	if (!parentsParentEntity) // Parent is top-level, get index from world
	{
		Vanguard::World* entityWorld = entity->GetWorld();
		int row = entityWorld->GetInstances<Vanguard::Entity>().IndexOf(parentEntity);
		return createIndex(row, 0, parentEntity);
	}

	// Parent is another entity
	for (int i = 0; i < parentsParentEntity->GetNumChildren(); i++)
	{
		if (parentsParentEntity->GetChild(i) == parentEntity)
			return createIndex(i, 0, parentEntity);
	}

	// Shouldn't happen
	return QModelIndex();
}

int WorldHierarchyModel::rowCount(const QModelIndex &parent) const
{
	if (parent.column() > 0)
		return 0;

	if (!parent.isValid()) // Root
	{
		if (rootWorld == nullptr)
			return Vanguard::EditorCore::GetInstance()->GetWorlds().Count();
		else
			return rootWorld->GetInstances<Vanguard::Entity>().Count();
	}
	else if (IsWorld(parent.internalPointer()))
	{
		Vanguard::World* parentItem = static_cast<Vanguard::World*>(parent.internalPointer());
		return parentItem->GetInstances<Vanguard::Entity>().Count();
	}
	else
	{
		Vanguard::Entity* parentItem = static_cast<Vanguard::Entity*>(parent.internalPointer());
		return parentItem->GetNumChildren();
	}
}

int WorldHierarchyModel::columnCount(const QModelIndex &parent) const
{
	return 1;
}

QVariant WorldHierarchyModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (role != Qt::DisplayRole)
		return QVariant();

	if (IsWorld(index.internalPointer()))
	{
		Vanguard::World *item = static_cast<Vanguard::World*>(index.internalPointer());
		return item->GetWorldName().GetCharPointer();
	}

	Vanguard::Entity *item = static_cast<Vanguard::Entity*>(index.internalPointer());

	return item->GetType()->GetTypeName().GetCharPointer();
}

Qt::ItemFlags WorldHierarchyModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return 0;

	return QAbstractItemModel::flags(index);
}

QVariant WorldHierarchyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
	{
		if (section == 0)
			return "World Hierarchy";
	}

	return QVariant();
}