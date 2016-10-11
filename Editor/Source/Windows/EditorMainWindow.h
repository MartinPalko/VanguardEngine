#pragma once
#include "Editor_Common.h"

#include <QMainWindow>

class QMenu;

namespace Vanguard
{
	class EditorMainWindow : public QMainWindow
	{
		Q_OBJECT;

	private:
		class ClassBrowser* classBrowser;
		class Inspector* inspector;
		class LogViewer* logViewer;
		class Viewport* viewport;
		class WorldHierarchy* worldHierarchy;

		QMenu* fileMenu;
		QMenu* editMenu;
		QMenu* viewMenu;
		QMenu* helpMenu;

	public:
		EditorMainWindow();

	public:
		// QWidget Overrides
		virtual QSize sizeHint() const override;

	private:
		void OnReloadStyle();
	};
}