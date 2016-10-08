#pragma once
#include "Editor_Common.h"

#include <QDockWidget>
#include <QItemSelection>

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

	private:
		void OnSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);

	};
}