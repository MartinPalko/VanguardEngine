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
		treeView->setModel(new TypeHierarchyModel());
		treeView->expandAll();
		setWidget(treeView);
	}
}