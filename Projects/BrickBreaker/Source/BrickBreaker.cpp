#include "gainput/gainput.h"

#include "Foundation_Common.h"
#if VANGUARD_WINDOWS
#include <Windows.h>
#endif

#include "Jobs/Frame.h"
#include "BrickBreaker.h"
#include "Entity/Transform.h"

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
		IRenderer* primaryRenderer = core->GetPrimaryRenderer();
		playerView = core->GetPrimaryRenderer()->CreateRenderView(gameWindow, playerCamera);

		// Spawn the paddle
		paddle = gameWorld->SpawnEntity<Paddle>();

		// Spawn the ball

		// Spawn the bricks
		int bricksX = 10;
		int bricksY = 10;
		for (int x = 0; x < bricksX; x++)
		{
			for (int y = 0; y < bricksY; y++)
			{
				Brick* newBrick = gameWorld->SpawnEntity<Brick>();
				newBrick->GetTransform()->position = Vector3(x, y, 0);
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