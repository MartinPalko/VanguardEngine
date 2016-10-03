#include "ResourceManager.h"
#include "Directories.h"
#include "Type.h"

namespace Vanguard
{
	ResourceManager::ResourceDirectory::~ResourceDirectory()
	{
		for (auto d : data)
			delete d.second;
	}

	void ResourceManager::ResourceDirectory::Add(StringID aName, Type * aType, ResourceID & outID)
	{
		auto typemap = data[aName];
		if (!typemap)
		{
			typemap = new std::map<Type*, ResourceID>();
			data[aName] = typemap;
		}

		typemap->emplace(aType, outID);
	}

	bool ResourceManager::ResourceDirectory::Lookup(StringID aName, Type* aType, ResourceID& outID)
	{
		auto typemap = data[aName];
		if (typemap)
		{
			auto existingResource = typemap->find(aType);
			if (existingResource != typemap->end())
			{
				outID = existingResource->second;
				return true;
			}
		}
		return false;
	}

	ResourceManager::ResourceManager()
	{
	}

	ResourceManager::~ResourceManager()
	{
		for (auto item : resources)
		{
			delete item.second;
		}
		resources.clear();
	}

	void ResourceManager::RefreshResourceDatabase()
	{
	}

	Resource* ResourceManager::GetResource(ResourceID aID)
	{
		return resources.at(aID);
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

		ResourceID id;
		if (directory.Lookup(aResourceName, aResourceType, id))
			return resources[id];

		// Create a new resource instance, register it, and return.
		Resource* resource = static_cast<Resource*>(aResourceType->CreateInstance());
		resource->resourcePath = resourcePath;

		resources[resource->id] = resource;
		directory.Add(aResourceName, aResourceType, resource->id);

		resource->LoadResource();
		return resource;
	}
}