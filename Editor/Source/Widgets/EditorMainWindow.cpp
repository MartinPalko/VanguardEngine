#include "EditorMainWindow.h"

#include "ClassBrowser.h"
#include "Inspector.h"
#include "LogViewer.h"
#include "Viewport.h"
#include "WorldHierarchy.h"

#include <QSplitter>

namespace Vanguard
{
	EditorMainWindow::EditorMainWindow()
	{
		viewport = new Viewport(this);
		setCentralWidget(viewport);

		classBrowser = new ClassBrowser(this);
		addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, classBrowser);

		inspector = new Inspector(this);
		addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, inspector);

		logViewer = new LogViewer(this);
		addDockWidget(Qt::DockWidgetArea::BottomDockWidgetArea, logViewer);

		worldHierarchy = new WorldHierarchy(this);
		addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, worldHierarchy);
	}
}
