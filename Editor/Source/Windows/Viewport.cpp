#include "Viewport.h"
#include "EditorCore.h"

#include <QOpenGLWidget>
#include <QWindow>
#include <QLayout>
#include <QResizeEvent>

#ifdef VANGUARD_WINDOWS
#include "Windows.h"
#endif // VANGUARD_WINDOWS

namespace Vanguard
{
	TYPE_DEFINITION(ViewportCamera, Camera)

		ViewportCamera::ViewportCamera() : Camera()
		, input(0.0f, 0.0f, 0.0f)
		, velocity(0.0f, 0.0f, 0.0f)
		, drag(4.0f)
		, brakingFriction(500.0f)
		, movementForce(800.0f)
		, zoomSpeed(1.0f)
	{
		EnableTick();
	}

	void ViewportCamera::Tick(Frame* aFrame)
	{
		//const Vector2 normalizedInput = input.Normalize() * Math::Min(1.0f, input.Length());
		const Vector3 normalizedInput = input;
		const float deltaSeconds = (float)aFrame->GetDeltaTime().InSeconds();

		// Apply movement force
		Vector3 acceleration = (normalizedInput * movementForce);
		velocity += acceleration * aFrame->GetDeltaTime().InSeconds();

		// Apply drag
		velocity *= Math::Clamp(1.0f - (drag * deltaSeconds), 0.0f, 1.0f);

		// Apply braking friction
		if (input == Vector3::Zero && velocity != Vector3::Zero)
		{
			velocity = velocity.Normalize() * (Math::Max(0.0f, velocity.Length() - brakingFriction * deltaSeconds));
		}

		// Move by velocity
		GetTransform()->Move(velocity * deltaSeconds * (fov / 100));
		fov -= velocity.z * aFrame->GetDeltaTime().InSeconds() * (fov / 100) * zoomSpeed;
	}

	Viewport::Viewport(QWidget* parent) : QWidget(parent)
	{
		Application::RegisterNativeEventHandler(this);

		// Temporary for now, just use the already created renderview.
		World* gameWorld = Core::GetInstance()->CreateProjectWorld();
		renderView = gameWorld->GetPrimaryRenderView();

		if (renderView)
		{
			//viewportCamera = gameWorld->SpawnEntity<ViewportCamera>();
			//viewportCamera->SetFov(100);
			//viewportCamera->SetOrthographic(true);
			//renderView = Core::GetInstance()->GetPrimaryRenderer()->CreateRenderView(viewportCamera);

			renderWindow = QWindow::fromWinId((WId)renderView->GetWindowHandle().handle);
			containerWidget = QWidget::createWindowContainer(renderWindow, this);
			containerWidget->setFocusPolicy(Qt::FocusPolicy::ClickFocus);

			setFocusProxy(containerWidget);

			setLayout(new QVBoxLayout());
			layout()->addWidget(containerWidget);
		}
	}

	Viewport::~Viewport()
	{
		Application::UnregisterNativeEventHandler(this);
	}

	void Viewport::HandleNativeEvent(NativeEvent aEvent)
	{
#ifdef VANGUARD_WINDOWS
		// Manually handle giving the container focus on windows.
		MSG* msg = (MSG*)aEvent;
		if (msg->message == WM_LBUTTONDOWN || msg->message == WM_RBUTTONDOWN)
		{
			HWND windowHandle = (HWND)renderWindow->winId();
			if (msg->hwnd == windowHandle)
			{
				if (!(GetFocus() == windowHandle))
					SetFocus(windowHandle);
			}
		}
#endif // VANGUARD_WINDOWS
	}
}