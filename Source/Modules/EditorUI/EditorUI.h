#pragma once
#include "EditorUI_Common.h"
#include "IModule.h"

namespace Vanguard_EditorUI
{
	class EditorUI : public IModule
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