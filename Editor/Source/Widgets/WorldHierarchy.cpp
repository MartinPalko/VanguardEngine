#include "WorldHierarchy.h"

#include "Models/WorldHierarchyModel.h"

#include <QTreeView>

namespace Vanguard
{
	WorldHierarchy::WorldHierarchy(World* aWorld, QWidget* parent) : QDockWidget("World Hierarchy", parent)
		, rootWorld(aWorld)
	{
		treeView = new QTreeView(this);
		treeView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		treeView->setModel(new WorldHierarchyModel(aWorld, treeView));
		setWidget(treeView);
	}
}