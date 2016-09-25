#include "EditorWorld.h"

namespace Vanguard
{
	EditorWorld::EditorWorld() : World("Editor")
	{
		
	}

	EditorWorld::~EditorWorld()
	{

	}

	void EditorWorld::Tick(Vanguard::Frame* aFrame)
	{
		// Editor world shouldn't tick
		// Vanguard::World::Tick(aFrame);
	}
	
	void EditorWorld::HandleNativeEvent(Vanguard::NativeEvent aEvent)
	{

	}
}