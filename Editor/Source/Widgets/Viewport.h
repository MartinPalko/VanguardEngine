#pragma once
#include "Editor_Common.h"

#include <QWidget>

namespace Vanguard
{
	class ViewportCamera : public Camera
	{
		TYPE_DECLARATION(ViewportCamera, Camera)
	protected:
		Vector3 input;
		Vector3 velocity;
		float drag;
		float brakingFriction;
		float movementForce;
		float zoomSpeed;

	public:
		ViewportCamera();
		virtual void Tick(Frame* aFrame) override;
		void SetInput(Vector3 aInput) { input = aInput; }
	};

	class Viewport : public QWidget
	{
		Q_OBJECT;

		QWidget* containerWidget;

		Camera* viewportCamera;
		RenderView* renderView;
		QWindow* renderWindow;

	public:
		Viewport(QWidget* parent);
	};
}