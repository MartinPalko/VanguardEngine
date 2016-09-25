#pragma once
#include "Editor_Common.h"

#include <QMainWindow>

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

	public:
		EditorMainWindow();
	};
}