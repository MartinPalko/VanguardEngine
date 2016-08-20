#include "Jobs/Frame.h"
#include "SimpleGame.h"

namespace SimpleGame
{
	VANGUARD_DECLARE_MODULE(SimpleGame)

	void SimpleGame::SetupGame()
	{
		Core* core = Core::GetInstance();

		World* gameWorld = core->CreateWorld("SimpleGame");

		Camera* playerCamera = gameWorld->SpawnEntity<Camera>();

		IRenderer* primaryRenderer = core->GetPrimaryRenderer();
		playerView = primaryRenderer->CreateRenderView(playerCamera, 1280, 720);

		Entity* spriteEntity = gameWorld->SpawnEntity<Entity>();
		spriteEntity->AddComponent<SpriteComponent>();
	}

	void SimpleGame::UpdateGame(Vanguard::Frame* aFrame)
	{
		DEBUG_LOG("UPDATE GAME");
		DEBUG_LOG("Seconds Elapsed: " + String::FromFloat(aFrame->deltaTime.InSeconds()));
	}

	void SimpleGame::CleanupGame()
	{
	}
}