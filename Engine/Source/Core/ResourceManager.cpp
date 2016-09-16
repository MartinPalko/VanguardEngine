#include "ResourceManager.h"

namespace Vanguard
{
	ResourceManager::ResourceManager()
	{
	}

	ResourceManager::~ResourceManager()
	{
		for (Resource* resource : resources)
		{
			delete resource;
		}
		resources.Clear();
	}

	FilePath* ResourceManager::FindResource(const String & aResourceName, const DynamicArray<String>& aAcceptableExtensions)
	{
		return nullptr;
	}
}