#include "ClassBrowser.h"

#include "Models/TypeHierarchyModel.h"

#include <QBoxLayout>
#include <QTreeView>

namespace Vanguard
{
	ClassBrowser::ClassBrowser(QWidget* parent) : QDockWidget("Class Browser", parent)
	{
		treeView = new QTreeView(this);
		treeView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		treeView->setModel(new TypeHierarchyModel(nullptr, treeView));
		treeView->expandAll();
		setWidget(treeView);
	}
}