#pragma once
#include "Core_Common.h"

namespace Vanguard
{
	class Resource
	{
		FilePath resourcePath;

	public:
		Resource(const String& aResourceName) {}
		virtual ~Resource() 
		{
			if (IsLoaded())
				UnloadResource();
		}

		FilePath GetResourcePath() { return resourcePath; }

		virtual bool LoadResource() = 0;
		virtual bool UnloadResource() = 0;
		virtual bool IsLoaded() = 0;
		virtual DynamicArray<String> GetValidFileExtensions() = 0;
	};

	class ResourceManager
	{
		DynamicArray<Resource*> resources;

	public:
		ResourceManager();
		virtual ~ResourceManager();

		FilePath* FindResource(const String& aResourceName, const DynamicArray<String>& aAcceptableExtensions);

		template<class T> T* LoadResource(const String& aResourceName)
		{
			return (T*)LoadResource(aResourceName);
		}
	};
}