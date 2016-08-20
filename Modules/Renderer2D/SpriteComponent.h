#include "Renderer2D_Common.h"

namespace Vanguard
{
	class RENDERER2D_API SpriteComponent : public Component
	{
		TYPE_DECLARATION(SpriteComponent, Component)
	private:
		Vector2 dimensions;
		Color color;

	public:
		SpriteComponent() : Component()
			, dimensions(1, 1)
			, color(1, 1, 1, 1)
		{
		}

		Vector2 GetDimensions() { return dimensions; }
		void SetDimensions(Vector2 aDimensions) { dimensions = aDimensions; }
		
		Color GetColor() { return color; }
		void SetColor(Color aColor) { color = aColor; }

	};
}