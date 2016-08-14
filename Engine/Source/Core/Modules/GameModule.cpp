#include "GameModule.h"

namespace Vanguard
{
	GameModule::GameModule()
	{

	}

	void GameModule::LoadModule()
	{
		// Create a game window

		// Create a game world
		gameWorld = Core::GetInstance()->CreateWorld("Game World");

		// Setup the game
		SetupGame();

		// Register the game's tick with the world.
		gameWorld->RegisterTick([this](Frame* aFrame) { this->UpdateGame(aFrame); });
	}

	void GameModule::UnloadModule()
	{
		CleanupGame();

		// Destroy the game world
		Core::GetInstance()->DestroyWorld(gameWorld);
		gameWorld = nullptr;	
	}
}