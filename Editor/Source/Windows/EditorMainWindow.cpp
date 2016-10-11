#include "EditorMainWindow.h"

#include "EditorCore.h"
#include "ClassBrowser.h"
#include "Inspector.h"
#include "LogViewer.h"
#include "Viewport.h"
#include "WorldHierarchy.h"
#include "StyleManager.h"

#include <QSplitter>
#include <QMenuBar>
#include <QMenu>

namespace Vanguard
{
	EditorMainWindow::EditorMainWindow()
	{
		// Menu bar
		fileMenu = menuBar()->addMenu("&File");
		editMenu = menuBar()->addMenu("&Edit");
		viewMenu = menuBar()->addMenu("&View");
		helpMenu = menuBar()->addMenu("&Help");

		QAction* exitAction = new QAction("E&xit", this);
		connect(exitAction, &QAction::triggered, this, &QWidget::close);
		fileMenu->addAction(exitAction);

		QAction* reloadStyleAction = new QAction("&Reload Style", this);
		connect(reloadStyleAction, &QAction::triggered, this, &EditorMainWindow::OnReloadStyle);
		editMenu->addAction(reloadStyleAction);

		// Viewport
		viewport = new Viewport(this);
		setCentralWidget(viewport);

		setDockOptions(dockOptions() & ~AnimatedDocks);

		// Dockable Tool Windows
		classBrowser = new ClassBrowser(this);
		addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, classBrowser);

		inspector = new Inspector(this);
		addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, inspector);

		logViewer = new LogViewer(this);
		addDockWidget(Qt::DockWidgetArea::BottomDockWidgetArea, logViewer);

		worldHierarchy = new WorldHierarchy(nullptr, this);
		addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, worldHierarchy);
	}

	QSize EditorMainWindow::sizeHint() const
	{
		return QSize(1280, 720);
	}

	void EditorMainWindow::OnReloadStyle()
	{
		StyleManager::GetInstance()->LoadStyle();
	}
}
