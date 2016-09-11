#include "gainput/gainput.h"

#include "Foundation_Common.h"

#include "Jobs/Frame.h"
#include "BrickBreaker.h"
#include "Entity/Transform.h"
#include "SpriteRenderer.h"
#include "BrickBreakerWorld.h"

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
	const Vector2 BrickBreakerWorld::PlayAreaSize = Vector2(100, 150);

	BrickBreakerWorld::BrickBreakerWorld() : World("Brick Breaker")
	{
		Application::RegisterNativeEventHandler(this);

		// Create a world and a renderview.
		Core* core = Core::GetInstance();
		Camera* playerCamera = SpawnEntity<Camera>();
		playerCamera->SetFov(100);
		playerCamera->SetOrthographic(true);

		IRenderer* primaryRenderer = core->GetPrimaryRenderer();
		playerView = core->GetPrimaryRenderer()->CreateRenderView(playerCamera);

		// Background sprite
		Actor* backgroundEntity = SpawnEntity<Actor>();
		SpriteRenderer* backgroundSprite = backgroundEntity->AddComponent<SpriteRenderer>();
		backgroundSprite->SetDimensions(PlayAreaSize);
		backgroundSprite->SetColor(Color(0x33, 0x33, 0x33));

		// Spawn the paddle
		paddle = SpawnEntity<Paddle>();
		paddle->GetTransform()->position = Vector3(0, -65.0f, 0);

		// Spawn the ball
		ball = SpawnEntity<Ball>();
		ball->GetTransform()->position = Vector3(0, paddle->GetTransform()->position.y, 0);
		ball->GetTransform()->position.y += paddle->GetLocalBounds().GetSize().y / 2;
		ball->GetTransform()->position.y += ball->GetLocalBounds().GetSize().y / 2;

		// Spawn the bricks
		const int bricksX = 12;
		const int bricksY = 6;
		const Vector2 brickSpacing(PlayAreaSize.x / bricksX, -4.f);
		const Vector2 brickOffset((-PlayAreaSize.x + brickSpacing.x) / 2, PlayAreaSize.y / 2 + brickSpacing.y);

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
				Brick* newBrick = SpawnEntity<Brick>();
				newBrick->GetTransform()->position = Vector3((float)x * brickSpacing.x + brickOffset.x, (float)y * brickSpacing.y + brickOffset.y, 0);
				newBrick->GetComponent<SpriteRenderer>()->SetColor(brickColors[y % (sizeof(brickColors) / sizeof(Color))]);
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

	BrickBreakerWorld::~BrickBreakerWorld()
	{
		Application::UnregisterNativeEventHandler(this);
		delete inputMap;
		delete inputManager;
	}

	void BrickBreakerWorld::Tick(Vanguard::Frame* aFrame)
	{
		// Update Gainput
		inputManager->Update(aFrame->deltaTime.InMilliseconds());

		LOG_MESSAGE("FPS: " + String::FromFloat(1.0f / aFrame->deltaTime.InSeconds()), "Brick Breaker");

		if (inputMap->GetBool(eGameButton::Exit))
		{
			Core::GetInstance()->ShutDown();
		}

		float input = 0;
		if (inputMap->GetBool(eGameButton::Left))
			input -= 1;
		if (inputMap->GetBool(eGameButton::Right))
			input += 1;

		if (!ball->TickEnabled() && input)
		{
			ball->EnableTick();
		}

		paddle->SetInput(input);

		// Super's tick AFTER we update input
		World::Tick(aFrame);
	}
	
	void BrickBreakerWorld::HandleNativeEvent(NativeEvent aEvent)
	{
		#if defined(VANGUARD_WINDOWS)
			// Forward any input messages to Gainput
			inputManager->HandleMessage((MSG*)aEvent);
		#elif defined(VANGUARD_LINUX)
			inputManager->HandleEvent((XEvent*)aEvent);
		#endif
	}
}