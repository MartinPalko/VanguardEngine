#pragma once
#include "Editor_Common.h"

#include <QDockWidget>

class QTreeView;

namespace Vanguard
{
	class ClassBrowser : public QDockWidget
	{
		Q_OBJECT;

	private:
		QTreeView* treeView;

	public:
		ClassBrowser(QWidget* parent);
	};
}