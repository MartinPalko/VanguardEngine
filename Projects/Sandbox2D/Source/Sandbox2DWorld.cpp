#include "gainput/gainput.h"

#include "Foundation_Common.h"

#include "Jobs/Frame.h"
#include "Sandbox2D.h"
#include "WorldObjects/Transform.h"
#include "SpriteRenderer.h"
#include "Sandbox2DWorld.h"
#include "Sandbox2DCamera.h"

namespace eGameButton
{
	enum eType
	{
		Exit = 0,
		ProfileFrame,
		CameraMoveLeft,
		CameraMoveRight,
		CameraMoveUp,
		CameraMoveDown,
		CameraZoomIn,
		CameraZoomOut
	};
}

namespace Sandbox2D
{
	Sandbox2DWorld::Sandbox2DWorld() : World("Brick Breaker")
	{
		Application::RegisterNativeEventHandler(this);

		// Create a world and a renderview.
		Core* core = Core::GetInstance();
		playerCamera = SpawnEntity<Sandbox2DCamera>();
		playerCamera->SetFov(100);
		playerCamera->SetOrthographic(true);
		playerCamera->GetTransform()->position = Vector3(12, 5, 0);

		IRenderer* primaryRenderer = core->GetPrimaryRenderer();
		playerView = core->GetPrimaryRenderer()->CreateRenderView(playerCamera);

		// Background sprite
		Actor* backgroundEntity = SpawnEntity<Actor>();
		SpriteRenderer* backgroundSprite = backgroundEntity->AddComponent<SpriteRenderer>();
		backgroundSprite->SetDimensions(Vector2(1000, 1000));
		backgroundSprite->SetColor(Color(100, 149, 237));


		// TestSprites
		Vector3 testSpriteLocations[] = {
			Vector3(0,0,0),
			Vector3(120,120,0),
			Vector3(60,40,0),
			Vector3(200,30,0),
			Vector3(120,300,0),
			Vector3(60,120,0),
			Vector3(40,160,0),
			Vector3(200,200,0)
		};

		for (int i = 0; i < sizeof(testSpriteLocations) / sizeof(Vector3); i++)
		{
			Actor* testSpriteActor = SpawnEntity<Actor>();
			testSpriteActor->GetTransform()->position = testSpriteLocations[i];
			SpriteRenderer* testSprite = testSpriteActor->AddComponent<SpriteRenderer>();
			testSprite->SetDimensions(Vector2(50, 50));
			testSprite->SetImage("Sprite.png");
			testSprite->SetBlendMode(Vanguard::eSpriteBlendMode::Blend);
		}

		// Setup Gainput
		inputManager = new gainput::InputManager(false);
		gainput::DeviceId keyboardId = inputManager->CreateDevice<gainput::InputDeviceKeyboard>();

		inputMap = new gainput::InputMap(*inputManager);
		inputMap->MapBool(eGameButton::Exit, keyboardId, gainput::KeyEscape);
		inputMap->MapBool(eGameButton::ProfileFrame, keyboardId, gainput::KeyP);
		inputMap->MapBool(eGameButton::CameraMoveLeft, keyboardId, gainput::KeyLeft);
		inputMap->MapBool(eGameButton::CameraMoveLeft, keyboardId, gainput::KeyA);
		inputMap->MapBool(eGameButton::CameraMoveRight, keyboardId, gainput::KeyRight);
		inputMap->MapBool(eGameButton::CameraMoveRight, keyboardId, gainput::KeyD);
		inputMap->MapBool(eGameButton::CameraMoveUp, keyboardId, gainput::KeyUp);
		inputMap->MapBool(eGameButton::CameraMoveUp, keyboardId, gainput::KeyW);
		inputMap->MapBool(eGameButton::CameraMoveDown, keyboardId, gainput::KeyDown);
		inputMap->MapBool(eGameButton::CameraMoveDown, keyboardId, gainput::KeyS);
		inputMap->MapBool(eGameButton::CameraZoomIn, keyboardId, gainput::KeyE);
		inputMap->MapBool(eGameButton::CameraZoomOut, keyboardId, gainput::KeyQ);
	}

	Sandbox2DWorld::~Sandbox2DWorld()
	{
		Application::UnregisterNativeEventHandler(this);
		delete inputMap;
		delete inputManager;
	}

	void Sandbox2DWorld::Tick(Vanguard::Frame* aFrame)
	{
		// Update Gainput
		inputManager->Update(aFrame->deltaTime.InMilliseconds());

		LOG_MESSAGE("FPS: " + String::FromFloat(1.0f / aFrame->deltaTime.InSeconds()), "Brick Breaker");

		if (inputMap->GetBool(eGameButton::Exit))
		{
			Core::GetInstance()->Quit();
		}

		if (inputMap->GetBoolIsNew(eGameButton::ProfileFrame))
			Core::GetInstance()->GetProfiler()->ProfileNextFrame();

		Vector3 input(0, 0, 0);
		if (inputMap->GetBool(eGameButton::CameraMoveLeft))
			input.x -= 1;
		if (inputMap->GetBool(eGameButton::CameraMoveRight))
			input.x += 1;
		if (inputMap->GetBool(eGameButton::CameraMoveUp))
			input.y += 1;
		if (inputMap->GetBool(eGameButton::CameraMoveDown))
			input.y -= 1;
		if (inputMap->GetBool(eGameButton::CameraZoomIn))
			input.z += 1;
		if (inputMap->GetBool(eGameButton::CameraZoomOut))
			input.z -= 1;

		playerCamera->SetInput(input);

		// Super's tick AFTER we update input
		World::Tick(aFrame);
	}
	
	void Sandbox2DWorld::HandleNativeEvent(NativeEvent aEvent)
	{
		#if defined(VANGUARD_WINDOWS)
			// Forward any input messages to Gainput
			inputManager->HandleMessage((MSG*)aEvent);
		#elif defined(VANGUARD_LINUX)
			inputManager->HandleEvent((XEvent*)aEvent);
		#endif
	}
}