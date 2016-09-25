#pragma once
#include "Editor_Common.h"
#include "Interfaces/IModule.h"

class QApplication;

namespace Vanguard
{
	class EditorMainWindow;
	class EditorWorld;

	class EditorCore : public Core , public INativeEventProcessor
	{
		QApplication* qApplication;
		EditorMainWindow* mainWindow;

		EditorWorld* editorWorld;

	public:
		EditorCore(QApplication* aQApplication) : Core()
			, qApplication(aQApplication)
		{}

		// Override Core
		virtual void Initialize(int aArgC, char** aArgV, const char* aProjectName) override;
		virtual void Run() override;
		virtual void ShutDown() override;

		// Implement INativeEventProcessor
		virtual bool GetNextEvent(NativeEvent& aOutNextEvent) override;
	};
}