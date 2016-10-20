#include "RenderableComponent2D.h"

namespace Vanguard
{
	ABSTRACT_TYPE_DEFINITION(RenderableComponent2D, Component);

	START_REGISTER_PROPERTIES(RenderableComponent2D);
		REGISTER_PROPERTY(RenderableComponent2D, dimensions);
		REGISTER_PROPERTY(RenderableComponent2D, color);
		REGISTER_PROPERTY(RenderableComponent2D, blendMode);
		REGISTER_PROPERTY(RenderableComponent2D, flipped);
	FINISH_REGISTER_PROPERTIES(RenderableComponent2D);

	RenderableComponent2D::RenderableComponent2D() : Component()
		, dimensions(1, 1)
		, color(255, 255, 255, 255)
		, blendMode(eSpriteBlendMode::None)
		, flipped(eSpriteFlip::None)
	{

	}

	void RenderableComponent2D::SetDimensions(Vector2 aDimensions)
	{
		dimensions = aDimensions;
		if (GetEntity()->GetType()->IsA(Type::GetType<Actor>()))
			static_cast<Actor*>(GetEntity())->UpdateBounds();
	}
}