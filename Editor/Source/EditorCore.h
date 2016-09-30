#pragma once
#include "Editor_Common.h"
#include "Interfaces/IModule.h"

class QApplication;

namespace Vanguard
{
	class EditorMainWindow;
	class EditorWorld;

	class EditorCore : public Core
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
		virtual void ShutDown() override;
		virtual void ProcessEvents() override;

	public:
		static EditorCore* GetInstance() { return static_cast<EditorCore*>(Core::GetInstance()); }

		World* GetEditorWorld();
	};
}