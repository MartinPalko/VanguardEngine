#include "Core.h"
#include "IModule.h"
#include "juce_gui_basics.h"

#include "EditorMainWindow.h"

namespace Vanguard
{
	class Editor : public IModule
	{
		virtual String GetModuleName() { return "Editor"; }
		virtual String GetModuleType() { return "Editor"; }

		EditorMainWindow* editorWindow;

		virtual void LoadModule()
		{
			editorWindow = new EditorMainWindow("Vanguard Editor");
		}

		virtual void UnloadModule()
		{
			delete editorWindow;
			editorWindow = nullptr;
		}

	};
}