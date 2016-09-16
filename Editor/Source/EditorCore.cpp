#include "EditorCore.h"
#include <QApplication>
#include <QMainWindow>

namespace Vanguard
{
	void EditorCore::Initialize(int aArgC, char** aArgV, const char* aProjectName)
	{
		Core::Initialize(aArgC, aArgV, aProjectName);
	}

	void EditorCore::Run()
	{
		Core::Run();
	}

	void EditorCore::ShutDown()
	{
		Core::ShutDown();
	}
}