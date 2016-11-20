#include "WorldHierarchy.h"
#include "EditorCore.h"
#include "Models/WorldHierarchyModel.h"

#include <QTreeView>
#include <QItemSelectionModel>

namespace Vanguard
{
	WorldHierarchy::WorldHierarchy(World* aWorld, QWidget* parent) : QDockWidget("World Hierarchy", parent)
		, rootWorld(aWorld)
	{
		treeView = new QTreeView(this);
		treeView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		treeView->setModel(new WorldHierarchyModel(aWorld, treeView));
		treeView->setHeaderHidden(true);
		setWidget(treeView);

		connect(treeView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &WorldHierarchy::OnSelectionChanged);
	}

	void WorldHierarchy::OnSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected)
	{
		QModelIndex current = treeView->currentIndex();
		WorldHierarchyNode* node = static_cast<WorldHierarchyNode*>(current.internalPointer());

		if (node->itemType == WorldHierarchyNode::eEntity)
			EditorCore::GetInstance()->SelectEntity(node->item.entity);
		else
			EditorCore::GetInstance()->ClearSelection();
	}
}