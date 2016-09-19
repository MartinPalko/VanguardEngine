#pragma once
#include "Resources_Common.h"
#include "Resource.h"

namespace Vanguard
{
	class RESOURCES_API ImageResource : public Resource
	{
		TYPE_DECLARATION(ImageResource, Resource);

	public:
		ImageResource();
		virtual ~ImageResource();

		// Implement Resource
		virtual bool LoadResource();
		virtual bool UnloadResource();
		virtual bool IsLoaded();
		virtual size_t GetSize();
	};
}