#pragma once
#include "Resources_Common.h"
#include "Type.h"
#include "Resource.h"

namespace Vanguard
{
	class RESOURCES_API ResourceManager
	{
		class ResourceDirectory
		{
			std::unordered_map<StringID, std::map<Type*, ResourceID>*> data;
		public:
			~ResourceDirectory();

			void Add(StringID aName, Type* aType, ResourceID& outID);
			bool Lookup(StringID aName, Type* aType, ResourceID& outID);
		};

		std::unordered_map<ResourceID, Resource*> resources;
		ResourceDirectory directory;

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