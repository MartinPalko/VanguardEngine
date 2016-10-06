#include "EditorCore.h"

#include "EditorWorld.h"
#include "Widgets/EditorMainWindow.h"

#include <QApplication>
#include <QMainWindow>

namespace Vanguard
{
	bool EditorCore::eventFilter(QObject* watched, QEvent* event)
	{
		if (watched == mainWindow && event->type() == QEvent::Close && GetState() == CoreState::Running)
		{
			Quit();
			return true;
		}
		return false;
	}

	bool EditorCore::nativeEventFilter(const QByteArray& eventType, void* message, long* result)
	{
		// Forward native events processed by Qt to the Core Application
		Application::DispatchNativeEvent((NativeEvent)message);
		return false;
	}

	void EditorCore::Initialize(int aArgC, char** aArgV, const char* aProjectName)
	{
		Core::Initialize(aArgC, aArgV, aProjectName);

		editorWorld = new EditorWorld();
		AddWorld(editorWorld);

		mainWindow = new EditorMainWindow();
		mainWindow->installEventFilter(this);

		QApplication::instance()->installNativeEventFilter(this);
		QApplication::instance()->installEventFilter(this);
	}

	void EditorCore::Run()
	{
		mainWindow->show();

		Core::Run();
	}

	void EditorCore::ShutDown()
	{
		mainWindow->removeEventFilter(this);

		if (mainWindow->isVisible())
			mainWindow->close();

		delete mainWindow;
		mainWindow = nullptr;

		Core::ShutDown();
		
		QApplication::instance()->removeNativeEventFilter(this);
		QApplication::instance()->removeEventFilter(this);
	}

	void EditorCore::ProcessEvents(bool aIncludeNativeEvents)
	{
		if (aIncludeNativeEvents && qApplication->hasPendingEvents())
			qApplication->processEvents();	

		Core::ProcessEvents(aIncludeNativeEvents);
	}

	World* EditorCore::GetEditorWorld()
	{
		return editorWorld;
	}

	DynamicArray<World*> EditorCore::GetWorlds()
	{
		return worlds;
	}

}