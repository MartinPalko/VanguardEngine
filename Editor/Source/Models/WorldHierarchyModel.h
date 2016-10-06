#pragma once
#include "Editor_Common.h"

#include <QAbstractItemModel>

class WorldHierarchyModel : public QAbstractItemModel, public Vanguard::IWorldEventListener
{
	Q_OBJECT;

	Vanguard::World* rootWorld;

	bool IsWorld(void* ptr) const;
	std::unordered_map<void*, QModelIndex> indexMap;

public:
	explicit WorldHierarchyModel(Vanguard::World* aWorld, QObject *parent = 0);
	~WorldHierarchyModel();

	// Impement IWorldEventListener
	virtual void OnWorldEvent(Vanguard::WorldEvent* aEvent) override;

	// Implement QAbstractItemModel
	QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
	Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
	QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;
	int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
	int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
};