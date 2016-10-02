#pragma once

#include "WorldObjects/Actor.h"
#include "WorldObjects/Transform.h"
#include "Types/Matrix4x4.h"

namespace Vanguard
{
	class CORE_API Camera : public Actor
	{
		TYPE_DECLARATION(Camera, Actor);

	public:
		Camera();

	protected:
		float fov;
		bool orthographic;
		float zNear;
		float zFar;

	public:

		float GetFov() { return fov; }
		void SetFov(float aFov) { fov = aFov; }
		bool GetOrthographic() { return orthographic; }
		void SetOrthographic(bool aOrthographic) { orthographic = aOrthographic; }
		float GetZNear() { return zNear; }
		void SetZNear(float aZNear) { zNear = aZNear; }
		float GetZFar() { return zFar; }
		void SetZFar(float aZFar) { zFar = aZFar; }

		Matrix4x4 GetProjectionMatrix();

	};
}
