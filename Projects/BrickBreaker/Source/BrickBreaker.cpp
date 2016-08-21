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
		int displayX = 1280;
		int displayY = 720;

		gameWindow = Application::CreateNativeWindow();

		Application::RegisterNativeEventHandler(this);

		Core* core = Core::GetInstance();

		World* gameWorld = core->CreateWorld("BrickBreaker");

		Camera* playerCamera = gameWorld->SpawnEntity<Camera>();

		IRenderer* primaryRenderer = core->GetPrimaryRenderer();
		playerView = primaryRenderer->CreateRenderView(gameWindow, playerCamera);

		paddle = gameWorld->SpawnEntity<Entity>();
		
		Transform* transformComponent = paddle->AddComponent<Transform>();
		transformComponent->position = Vector3(0, 0, 0);

		SpriteComponent* spriteComponent = paddle->AddComponent<SpriteComponent>();
		spriteComponent->SetDimensions(Vector2(0.1f, 0.1f));
		spriteComponent->SetColor(Color(255, 120, 120, 255));
		
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

		float movement = 0;
		if (inputMap->GetBool(eGameButton::Left))
		{
			movement -= aFrame->deltaTime.InSeconds() * 0.1f;
		}

		if (inputMap->GetBool(eGameButton::Right))
		{
			movement += aFrame->deltaTime.InSeconds() * 0.1f;
		}
		Transform* transform = paddle->GetComponent<Transform>();
		transform->position.x += movement;
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