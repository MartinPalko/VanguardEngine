#pragma once
#include "Renderer2D_Common.h"
#include "SDLImageResource.h"

namespace Vanguard
{
	class RENDERER2D_API SpriteRenderer : public Component
	{
		TYPE_DECLARATION(SpriteRenderer, Component)
	private:
		Vector2 dimensions;
		Color color;
		SDLImageResource* image;

	public:
		SpriteRenderer() : Component()
			, dimensions(1, 1)
			, color(1, 1, 1, 1)
			, image(nullptr)
		{
		}

		inline Vector2 GetDimensions() const { return dimensions; }
		void SetDimensions(Vector2 aDimensions);

		inline Color GetColor() const { return color; }
		void SetColor(Color aColor) { color = aColor; }

		virtual Box GetBounds() override { return Box(Vector3(dimensions.x, dimensions.y, 0)); }

		void SetImage(const String& aImageName);
		SDLImageResource* GetImage() { return image; }

	};
}