#include "EditorCore.h"

#include "EditorWorld.h"
#include "Widgets/EditorMainWindow.h"

#include <QApplication>
#include <QMainWindow>

namespace Vanguard
{
	bool EditorCore::nativeEventFilter(const QByteArray& eventType, void* message, long* result)
	{
		Application::DispatchNativeEvent((NativeEvent)message);
		return false;
	}

	void EditorCore::Initialize(int aArgC, char** aArgV, const char* aProjectName)
	{
		Core::Initialize(aArgC, aArgV, aProjectName);

		editorWorld = new EditorWorld();
		AddWorld(editorWorld);

		mainWindow = new EditorMainWindow();
		mainWindow->show();

		QApplication::instance()->installNativeEventFilter(this);
	}

	void EditorCore::ShutDown()
	{
		Core::ShutDown();
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
}