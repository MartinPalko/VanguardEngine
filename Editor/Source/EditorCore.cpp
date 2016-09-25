#include "EditorCore.h"

#include "EditorWorld.h"
#include "Widgets/EditorMainWindow.h"

#include <QApplication>
#include <QMainWindow>

namespace Vanguard
{
	void EditorCore::Initialize(int aArgC, char** aArgV, const char* aProjectName)
	{
		Application::RegisterNativeEventProcessor(this);

		Core::Initialize(aArgC, aArgV, aProjectName);

		editorWorld = new EditorWorld();
		AddWorld(editorWorld);

		mainWindow = new EditorMainWindow();
	}

	void EditorCore::Run()
	{
		mainWindow->show();
		while (mainWindow->isVisible())
		{
			if (qApplication->hasPendingEvents())
				qApplication->processEvents();
		}

		//Core::Run();
	}

	void EditorCore::ShutDown()
	{
		Core::ShutDown();

		mainWindow->close();
		delete mainWindow;
	}

	bool EditorCore::GetNextEvent(NativeEvent & aOutNextEvent)
	{
		return false;
	}
}