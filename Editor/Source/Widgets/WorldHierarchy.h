#pragma once
#include "Editor_Common.h"

#include <QDockWidget>

class QTreeView;

namespace Vanguard
{
	class WorldHierarchy : public QDockWidget
	{
		Q_OBJECT;

	private:
		World* rootWorld;
		QTreeView* treeView;

	public:
		WorldHierarchy(World* aWorld, QWidget* parent);
	};
}