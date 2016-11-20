#pragma once
#include "Editor_Common.h"

#include <QAbstractItemModel>

class WorldHierarchyModel;

struct WorldHierarchyNode
{
	union
	{
		Vanguard::World* world;
		Vanguard::Entity* entity;
	} item;

	enum
	{
		eNull = 0,
		eWorld = 1,
		eEntity = 2
	} itemType;

	WorldHierarchyNode* parent;
	Vanguard::DynamicArray<WorldHierarchyNode*> children;
	WorldHierarchyModel* model;

	WorldHierarchyNode(WorldHierarchyModel* aModel);
	WorldHierarchyNode(Vanguard::World* aWorld, WorldHierarchyNode* aParent, WorldHierarchyModel* aModel);
	WorldHierarchyNode(Vanguard::Entity* aEntity, WorldHierarchyNode* aParent, WorldHierarchyModel* aModel);
	~WorldHierarchyNode();

	QString GetDisplayName();
	int GetRow();
};

class WorldHierarchyModel : public QAbstractItemModel, public Vanguard::ICoreEventListener, public Vanguard::IWorldEventListener
{
	Q_OBJECT;

	WorldHierarchyNode* rootNode;
	std::unordered_map<void*, WorldHierarchyNode*> nodemap;

public:
	explicit WorldHierarchyModel(Vanguard::World* aWorld, QObject *parent = 0);
	~WorldHierarchyModel();

	WorldHierarchyNode* GetNode(void* aPtr, WorldHierarchyNode* aParent = nullptr);
	bool GetNodeIndex(WorldHierarchyNode* aNode, QModelIndex& outIndex, QModelIndex aParent = QModelIndex());
	bool GetNodeLocation(WorldHierarchyNode* aNode, QModelIndex& outParent, int& outIndex, QModelIndex aParent = QModelIndex());

	// Implement QAbstractItemModel
	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
	QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;
	int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
	int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
	QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
	Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

	// Implement Vanguard::ICoreEventListener
	virtual void OnCoreEvent(Vanguard::Event* aEvent) override;

	// Implement Vanguard::IWorldEventListener
	virtual void OnWorldEvent(Vanguard::WorldEvent* aEvent) override;
};