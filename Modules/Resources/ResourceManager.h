#pragma once
#include "Resources_Common.h"
#include "Type.h"
#include "Resource.h"

namespace Vanguard
{
	class RESOURCES_API ResourceManager
	{
		//std::unordered_map<ResourceID, Resource*> resources;
		//std::unordered_map<StringID, std::map<Type*, ResourceID>> resourceDirectory;

	public:
		ResourceManager();
		virtual ~ResourceManager();

		void RefreshResourceDatabase();

		Resource* GetResource(ResourceID aID);

		FilePath FindAbsoluteResourcePath(const String& aResourceName);

		Resource* LoadResource(const String& aResourceName, Type* aResourceType);
		template<class T> T* LoadResource(const String& aResourceName)
		{
			Resource* resource = LoadResource(aResourceName, Type::GetType<T>());
			return static_cast<T*>(resource);
		}
	};
}