#include "WorldHierarchyModel.h"
#include "EditorCore.h"

WorldHierarchyNode::WorldHierarchyNode(WorldHierarchyModel* aModel)
{
	parent = nullptr;
	itemType = eNull;
	model = aModel;

	for (auto world : Vanguard::EditorCore::GetInstance()->GetWorlds())
		children.PushBack(new WorldHierarchyNode(world, this, model));
}

WorldHierarchyNode::WorldHierarchyNode(Vanguard::World* aWorld, WorldHierarchyNode* aParent, WorldHierarchyModel* aModel)
{
	parent = aParent;
	item.world = aWorld;
	itemType = eWorld;
	model = aModel;

	aWorld->RegisterEventListener(model);

	for (auto entity : aWorld->GetInstances<Vanguard::Entity>(true))
		children.PushBack(new WorldHierarchyNode(entity, this, model));
}

WorldHierarchyNode::WorldHierarchyNode(Vanguard::Entity* aEntity, WorldHierarchyNode* aParent, WorldHierarchyModel* aModel)
{
	parent = aParent;
	item.entity = aEntity;
	itemType = eEntity;
	model = aModel;

	for (int i = 0; i < aEntity->GetNumChildren(); i++)
		children.PushBack(new WorldHierarchyNode(aEntity->GetChild(i), this, model));
}

WorldHierarchyNode::~WorldHierarchyNode()
{
	if (itemType == eWorld)
		item.world->UnregisterEventListener(model);

	for (auto child : children)
	{
		delete child;
	}
}

QString WorldHierarchyNode::GetDisplayName()
{
	switch (itemType)
	{
	case eWorld:
		return ToQString(item.world->GetWorldName());
	case eEntity:
		return ToQString(item.entity->GetName());
	default:
		return "Invalid";
	}
}

int WorldHierarchyNode::GetRow()
{
	if (!parent)
		return 0;

	return parent->children.IndexOf(this);
}

WorldHierarchyModel::WorldHierarchyModel(Vanguard::World* aWorld, QObject *parent) : QAbstractItemModel(parent)
{
	if (aWorld)
		rootNode = new WorldHierarchyNode(aWorld, nullptr, this);
	else
	{
		rootNode = new WorldHierarchyNode(this);

		// Need to listen for worlds added and removed
		Vanguard::Core::GetInstance()->RegisterEventListener(this);
	}
}

WorldHierarchyModel::~WorldHierarchyModel()
{
	Vanguard::Core::GetInstance()->UnregisterEventListener(this);
	delete rootNode;
}

WorldHierarchyNode* WorldHierarchyModel::GetNode(void* aPtr, WorldHierarchyNode* aParent)
{
	if (!aParent)
		aParent = rootNode;

	// Recursively find the node who's item matches aPtr
	const int numChildren = aParent->children.Count();
	for (int i = 0; i < numChildren; i++)
	{
		WorldHierarchyNode* child = aParent->children[i];
		if (child->item.entity == aPtr)
		{
			return child;
		}
		else
		{
			WorldHierarchyNode* foundInChildren = GetNode(aPtr, child);
			if (foundInChildren)
			{
				return foundInChildren;
			}
		}
	}
	return false;
}

bool WorldHierarchyModel::GetNodeIndex(WorldHierarchyNode* aNode, QModelIndex& outIndex, QModelIndex aParent)
{
	// Recursively search for a QModelIndex pointing to our node
	const int numChildren = rowCount(aParent);
	for (int i = 0; i < numChildren; i++)
	{
		QModelIndex child = index(i, 0, aParent);
		if (child.internalPointer() == aNode)
		{
			outIndex = outIndex;
			return true;
		}
		else if (GetNodeIndex(aNode, outIndex, child))
		{
			return true;
		}
	}
	return false;
}

bool WorldHierarchyModel::GetNodeLocation(WorldHierarchyNode* aNode, QModelIndex& outParent, int& outIndex, QModelIndex aParent)
{
	// Recursively search for a QModelIndex pointing to our node
	const int numChildren = rowCount(aParent);
	for (int i = 0; i < numChildren; i++)
	{
		QModelIndex child = index(i, 0, aParent);
		if (child.internalPointer() == aNode)
		{
			outParent = aParent;
			outIndex = i;
			return true;
		}
		else if (GetNodeLocation(aNode, outParent, outIndex, child))
		{
			return true;
		}
	}
	return false;
}

QModelIndex WorldHierarchyModel::index(int row, int column, const QModelIndex &parent) const
{
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	if (!parent.isValid()) // Root item
		return createIndex(row, column, rootNode->children.At(row));

	WorldHierarchyNode* node = static_cast<WorldHierarchyNode*>(parent.internalPointer());
	return createIndex(row, column, node->children.At(row));
}

QModelIndex WorldHierarchyModel::parent(const QModelIndex &index) const
{
	if (!index.isValid())
		return QModelIndex();

	WorldHierarchyNode* node = static_cast<WorldHierarchyNode*>(index.internalPointer());

	if (node->parent == rootNode)
		return QModelIndex();

	return createIndex(node->parent->GetRow(), 0, node->parent);
}

int WorldHierarchyModel::rowCount(const QModelIndex &parent) const
{
	if (parent.column() > 0)
		return 0;

	if (!parent.isValid()) // Root
		return rootNode->children.Count();

	WorldHierarchyNode* node = static_cast<WorldHierarchyNode*>(parent.internalPointer());
	return node->children.Count();
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

	WorldHierarchyNode* node = static_cast<WorldHierarchyNode*>(index.internalPointer());
	return node->GetDisplayName();
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

void WorldHierarchyModel::OnCoreEvent(Vanguard::Event* aEvent)
{
	// TODO: Handle worlds added and removed.
}

void WorldHierarchyModel::OnWorldEvent(Vanguard::WorldEvent* aEvent)
{
	if (auto addedEvent = Vanguard::Type::SafeCast<Vanguard::ObjectAddedEvent>(aEvent))
	{
		// Handle entity added to world
		if (auto entity = Vanguard::Type::SafeCast<Vanguard::Entity>(addedEvent->GetObject()))
		{
			WorldHierarchyNode* parentNode;

			if (entity->HasParent())
				parentNode = GetNode(entity->GetParent());
			else
				parentNode = GetNode(entity->GetWorld());

			QModelIndex parentIndex;
			GetNodeIndex(parentNode, parentIndex);

			if (parentNode && GetNodeIndex(parentNode, parentIndex))
			{
				const int numChildren = parentNode->children.Count();
				beginInsertRows(parentIndex, numChildren, numChildren);
				parentNode->children.PushBack(new WorldHierarchyNode(entity, parentNode, this));
				endInsertRows();
			}
		}
	}
	else if (auto removedEvent = Vanguard::Type::SafeCast<Vanguard::ObjectRemovedEvent>(aEvent))
	{
		// Handle entity removed from world
		Vanguard::World* world = aEvent->GetWorld();

		WorldHierarchyNode* node = GetNode(removedEvent->GetObject());
		QModelIndex parent;
		int i;
		
		if (node && GetNodeLocation(node, parent, i))
		{
			beginRemoveRows(parent, i, i);
			node->parent->children.Remove(node);
			delete node;
			endRemoveRows();
		}
	}
}