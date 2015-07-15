#pragma once
#include "Editor_Common.h"
#include "Modules/IModule.h"

namespace Vanguard_Editor
{
	class Editor : public IModule
	{
		virtual String GetModuleName() { return "EditorUI"; }
		virtual String GetModuleType() { return "Editor"; }

		World* editorWorld;
		
		virtual void LoadModule()
		{
			editorWorld = Core::GetInstance()->CreateWorld("Editor World");
		}

		virtual void UnloadModule()
		{
			Core::GetInstance()->DestroyWorld(editorWorld);
		}
	};
}