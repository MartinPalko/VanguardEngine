#pragma once
#include "Resources_Common.h"
#include "Type.h"

namespace Vanguard
{
	class RESOURCES_API Resource
	{
		ABSTRACT_BASETYPE_DECLARATION(Resource);

		friend class ResourceManager;

	private:
		FilePath resourcePath;
		UUID id;

	public:
		Resource() {}
		virtual ~Resource();

		FilePath GetResourcePath() { return resourcePath; }

		virtual bool LoadResource() = 0;
		virtual bool UnloadResource() = 0;
		virtual bool IsLoaded() = 0;
		virtual size_t GetSize() = 0;
	};
}