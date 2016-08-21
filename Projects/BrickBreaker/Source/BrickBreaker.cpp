#include "gainput/gainput.h"

#include "Foundation_Common.h"
#if VANGUARD_WINDOWS
#include <Windows.h>
#endif

#include "Jobs/Frame.h"
#include "BrickBreaker.h"
#include "Entity/Transform.h"
#include "SpriteComponent.h"

namespace eGameButton
{
	enum eType
	{
		Exit = 0,
		Left,
		Right
	};
}

namespace BrickBreaker
{
	VANGUARD_DECLARE_MODULE(BrickBreaker)

	void BrickBreaker::SetupGame()
	{
		gameWindow = Application::CreateNativeWindow();

		Application::RegisterNativeEventHandler(this);

		// Create a world and a renderview.
		Core* core = Core::GetInstance();
		World* gameWorld = core->CreateWorld("BrickBreaker");
		Camera* playerCamera = gameWorld->SpawnEntity<Camera>();
		playerCamera->SetFov(100);
		playerCamera->SetOrthographic(true);

		IRenderer* primaryRenderer = core->GetPrimaryRenderer();
		playerView = core->GetPrimaryRenderer()->CreateRenderView(gameWindow, playerCamera);


		// Background sprite
		Actor* backgroundEntity = gameWorld->SpawnEntity<Actor>();
		SpriteComponent* backgroundSprite = backgroundEntity->AddComponent<SpriteComponent>();
		backgroundSprite->SetDimensions(Vector2(100, 150));
		backgroundSprite->SetColor(Color(0x33, 0x33, 0x33));

		// Spawn the paddle
		paddle = gameWorld->SpawnEntity<Paddle>();
		paddle->GetTransform()->position = Vector3(0, -65.0f, 1);

		// Spawn the ball

		// Spawn the bricks
		const int bricksX = 11;
		const int bricksY = 6;
		const Vector2 brickOffset(-40.0f, 65.0f);
		const Vector2 brickSpacing(8.0f, -4.0f);
		const Color brickColors[] = {
			Color(230, 120, 230, 230),
			Color(120, 120, 230, 230),
			Color(120, 230, 230, 230),
			Color(120, 230, 120, 230),
			Color(230, 230, 120, 230),
			Color(230, 120, 120, 230),
		};

		for (int x = 0; x < bricksX; x++)
		{
			for (int y = 0; y < bricksY; y++)
			{
				Brick* newBrick = gameWorld->SpawnEntity<Brick>();
				newBrick->GetTransform()->position = Vector3((float)x * brickSpacing.x + brickOffset.x, (float)y * brickSpacing.y + brickOffset.y, 0);
				newBrick->GetComponent<SpriteComponent>()->SetColor(brickColors[y % (sizeof(brickColors) / sizeof(Color))]);
			}
		}

		// Setup Gainput
		inputManager = new gainput::InputManager(false);
		gainput::DeviceId keyboardId = inputManager->CreateDevice<gainput::InputDeviceKeyboard>();
		
		inputMap = new gainput::InputMap(*inputManager);
		inputMap->MapBool(eGameButton::Exit, keyboardId, gainput::KeyEscape);
		inputMap->MapBool(eGameButton::Left, keyboardId, gainput::KeyLeft);
		inputMap->MapBool(eGameButton::Left, keyboardId, gainput::KeyA);
		inputMap->MapBool(eGameButton::Right, keyboardId, gainput::KeyRight);
		inputMap->MapBool(eGameButton::Right, keyboardId, gainput::KeyD);
	}

	void BrickBreaker::UpdateGame(Vanguard::Frame* aFrame)
	{
		// Update Gainput
		int displayX, displayY;
		Application::GetWindowSize(gameWindow, displayX, displayY);
		inputManager->SetDisplaySize(displayX, displayY);
		inputManager->Update(aFrame->deltaTime.InMilliseconds());

		DEBUG_LOG("UPDATE GAME");
		DEBUG_LOG("Seconds Elapsed: " + String::FromFloat(aFrame->deltaTime.InSeconds()));

		if (inputMap->GetBool(eGameButton::Exit))
		{
			Core::GetInstance()->ShutDown();
		}

		float input = 0;
		if (inputMap->GetBool(eGameButton::Left))
			input -= 1;
		if (inputMap->GetBool(eGameButton::Right))
			input += 1;

		paddle->SetInput(input);
	}

	void BrickBreaker::CleanupGame()
	{
		Application::UnregisterNativeEventHandler(this);
		delete inputMap;
		delete inputManager;
	}

	void BrickBreaker::HandleNativeEvent(NativeEvent aEvent)
	{
		// Forward any input messages to Gainput
		inputManager->HandleMessage(aEvent.message);
	}
}