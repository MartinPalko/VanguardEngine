#include "EditorCore.h"

#include "EditorWorld.h"
#include "Widgets/EditorMainWindow.h"

#include <QApplication>
#include <QMainWindow>

namespace Vanguard
{
	void EditorCore::Initialize(int aArgC, char** aArgV, const char* aProjectName)
	{
		Core::Initialize(aArgC, aArgV, aProjectName);

		editorWorld = new EditorWorld();
		AddWorld(editorWorld);

		mainWindow = new EditorMainWindow();
		mainWindow->show();
	}

	void EditorCore::ShutDown()
	{
		Core::ShutDown();
	}

	void EditorCore::ProcessEvents()
	{
		Core::ProcessEvents();

		if (qApplication->hasPendingEvents())
			qApplication->processEvents();		
	}

	World* EditorCore::GetEditorWorld()
	{
		return editorWorld;
	}
}