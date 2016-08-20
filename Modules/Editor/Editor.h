#pragma once
#include "Editor_Common.h"
#include "Interfaces/IModule.h"

namespace Vanguard_Editor
{
	class Editor : public IModule
	{
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