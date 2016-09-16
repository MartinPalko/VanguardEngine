#pragma once
#include "Editor_Common.h"
#include "Interfaces/IModule.h"

class QApplication;
class QMainWindow;

namespace Vanguard
{
	class EditorCore : public Core
	{
		QApplication* qApplication;
		QMainWindow* qMainWindow;

	public:
		virtual void Initialize(int aArgC, char** aArgV, const char* aProjectName = "") override;
		virtual void Run() override;
		virtual void ShutDown() override;
	};
}