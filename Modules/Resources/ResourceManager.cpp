#include "ResourceManager.h"
#include "Directories.h"
#include "Type.h"

namespace Vanguard
{
	ResourceManager::ResourceManager()
	{
	}

	ResourceManager::~ResourceManager()
	{
		//for (auto item : resources)
		//{
		//	delete item.second;
		//}
		//resources.clear();
	}

	void ResourceManager::RefreshResourceDatabase()
	{
	}

	Resource* ResourceManager::GetResource(ResourceID aID)
	{
		//return resources.at(aID);
		return nullptr;
	}

	FilePath ResourceManager::FindAbsoluteResourcePath(const String& aResourceName)
	{
		DynamicArray<FilePath> resourceDirectories = Directories::GetResourceDirectories();
		for (FilePath resourceDirectory : resourceDirectories)
		{
			FilePath resourcePath = resourceDirectory.GetRelative(aResourceName);
			if (FileSystem::FileExists(resourcePath))
				return resourcePath;
		}
		return FilePath();
	}

	Resource* ResourceManager::LoadResource(const String& aResourceName, Type* aResourceType)
	{
		if (aResourceType->IsAbstract())
			return nullptr;

		if (!aResourceType->IsA(Type::GetType<Resource>()))
			return nullptr;
		
		FilePath resourcePath = FindAbsoluteResourcePath(aResourceName);

		if (!resourcePath)
			return nullptr;

		const StringID resourceNameID = aResourceName;

		//// Check with resource directory to see if this resource already exists.
		//auto& existingResources = resourceDirectory.find(resourceNameID);
		//if (existingResources != resourceDirectory.end())
		//{
		//	auto& existingResourceType = existingResources->second.find(aResourceType);
		//	if (existingResourceType != existingResources->second.end())
		//	{
		//		return resources[existingResourceType->second];
		//	}
		//}

		// Create a new resource instance, register it, and return.
		Resource* resource = static_cast<Resource*>(aResourceType->CreateInstance());
		resource->resourcePath = resourcePath;

		//resources[resource->id] = resource;
		//resourceDirectory[resourceNameID][aResourceType] = resource->id;

		resource->LoadResource();
		return resource;
	}
}