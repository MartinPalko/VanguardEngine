#pragma once
#include "Renderer2D_Common.h"
#include "SDLImageResource.h"

namespace Vanguard
{
	namespace eSpriteBlendMode
	{
		enum Type
		{
			None = 0x00000000,
			Blend = 0x00000001,
			Add = 0x00000002,
			Mod = 0x00000004
		};
	}

	namespace eSpriteFlip
	{
		enum Type
		{
			None = 0x00000000,
			Horizontal = 0x00000001,
			Vertical = 0x00000002,
			Both = 0x00000003
		};
	}

	class RENDERER2D_API SpriteRenderer : public Component
	{
	public:

		TYPE_DECLARATION(SpriteRenderer, Component)
	private:
		Vector2 dimensions;
		Color color;
		SDLImageResource* image;
		eSpriteBlendMode::Type blendMode;
		eSpriteFlip::Type flipped;

	public:
		SpriteRenderer() : Component()
			, dimensions(1, 1)
			, color(1, 1, 1, 1)
			, image(nullptr)
			, blendMode(eSpriteBlendMode::None)
			, flipped(eSpriteFlip::None)
		{
		}

		inline Vector2 GetDimensions() const { return dimensions; }
		void SetDimensions(Vector2 aDimensions);

		inline Color GetColor() const { return color; }
		void SetColor(Color aColor) { color = aColor; }

		virtual Box GetBounds() override { return Box(Vector3(dimensions.x, dimensions.y, 0)); }

		void SetImage(const String& aImageName);
		SDLImageResource* GetImage() { return image; }

		void SetBlendMode(eSpriteBlendMode::Type aBlendMode) { blendMode = aBlendMode; }
		eSpriteBlendMode::Type GetBlendMode() { return blendMode; }

		void SetFlipped(eSpriteFlip::Type aFlip) { flipped = aFlip; }
		eSpriteFlip::Type GetFlipped() { return flipped; }

	};
}