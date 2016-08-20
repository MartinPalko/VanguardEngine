#include "Jobs/Frame.h"
#include "SimpleGame.h"
#include "Entity/Transform.h"

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

		paddle = gameWorld->SpawnEntity<Entity>();
		
		Transform* transformComponent = paddle->AddComponent<Transform>();
		transformComponent->position = Vector3(0, 0, 0);

		SpriteComponent* spriteComponent = paddle->AddComponent<SpriteComponent>();
		spriteComponent->SetDimensions(Vector2(0.1f, 0.1f));
		spriteComponent->SetColor(Color(255, 120, 120, 255));
	}

	void SimpleGame::UpdateGame(Vanguard::Frame* aFrame)
	{
		DEBUG_LOG("UPDATE GAME");
		DEBUG_LOG("Seconds Elapsed: " + String::FromFloat(aFrame->deltaTime.InSeconds()));

		float movement = aFrame->deltaTime.InSeconds() * 0.01f;
		Transform* transform = paddle->GetComponent<Transform>();
		transform->position.x += movement;
	}

	void SimpleGame::CleanupGame()
	{
	}
}