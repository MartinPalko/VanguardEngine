#pragma once
#include "Renderer2D_Common.h"

namespace Vanguard
{
	class RENDERER2D_API SpriteRenderer : public Component
	{
		TYPE_DECLARATION(SpriteRenderer, Component)
	private:
		Vector2 dimensions;
		Color color;

	public:
		SpriteRenderer() : Component()
			, dimensions(1, 1)
			, color(1, 1, 1, 1)
		{
		}

		inline Vector2 GetDimensions() const { return dimensions; }
		void SetDimensions(Vector2 aDimensions);

		inline Color GetColor() const { return color; }
		void SetColor(Color aColor) { color = aColor; }

		virtual Box GetBounds() override { return Box(Vector3(dimensions.x, dimensions.y, 0)); }

	};
}