#pragma once
#include "Foundation.h"
#include "Core_Common.h"
#include "Log.h"
#include <typeinfo>

namespace Vanguard
{
	template<class T, class U> T base_of(U T::*);

	class CORE_API INativeClassInfo
	{
	protected:
		static std::unordered_map<size_t, INativeClassInfo* >& GetClassinfoNameMap();
		static std::unordered_map<size_t, INativeClassInfo* >& GetClassinfoHashMap();

		String className;
		String baseClassName;
		size_t runtimeHash; // This hash is ONLY useful for runtime comparisons. It may differ from build to build, or from compiler to compiler.
		INativeClassInfo* baseClass = nullptr;
		DynamicArray<INativeClassInfo*> derivedClasses;

		INativeClassInfo(const String& aClassName, const String& aBaseClassName, size_t aRuntimeHash)
		{
			className = aClassName;
			baseClassName = aBaseClassName;
			runtimeHash = aRuntimeHash;
		}

		template<class T> static T* CreateInstance()
		{
			return new T();
		}
	public:
		virtual void* CreateInstance() const = 0;

		String GetTypeName() const{ return className; };

		INativeClassInfo* GetBaseClass() const { return baseClass; }
		DynamicArray<INativeClassInfo*> GetDerivedClasses() const { return derivedClasses; }

		static DynamicArray<INativeClassInfo*> GetAllTypes();
		static INativeClassInfo* GetType(const String& aTypeName);
		template<class T>static INativeClassInfo* GetType()
		{
			return GetClassinfoHashMap()[typeid(T).hash_code()];
		}

		bool IsA(INativeClassInfo* otherClass) const;

		static void UpdateHierarchyReferences()
		{
			// First clear all references.
			for (auto pair : GetClassinfoHashMap())
			{
				pair.second->baseClass = nullptr;
				pair.second->derivedClasses.Clear();
			}
			// Now rebuild
			for (auto pair : GetClassinfoHashMap())
			{
				pair.second->baseClass = GetType(pair.second->baseClassName);
				if (pair.second->baseClass)
					pair.second->baseClass->derivedClasses.PushBack(pair.second);
			}
		}
	};

	template <class T> class NativeClassInfo : public INativeClassInfo
	{

	public:
		static INativeClassInfo* Create(const char* aClassName, const char* aBaseClassName = "")
		{
			const size_t hash = typeid(T).hash_code();

			std::unordered_map<size_t, INativeClassInfo*>& nameMap = GetClassinfoNameMap();
			std::unordered_map<size_t, INativeClassInfo*>& hashMap = GetClassinfoHashMap();
			
			if (hashMap.count(hash))
			{
				return hashMap[hash];
			}

			NativeClassInfo<T>* newClassInfo = new NativeClassInfo<T>(aClassName, aBaseClassName, hash);
			nameMap[StringID(aClassName).GetHash()] = newClassInfo;
			hashMap[hash] = newClassInfo;

			// TODO: Do this initially, and then again whenever a dll is loaded or unloaded, instead of every time a class is registered.
			UpdateHierarchyReferences();

			return newClassInfo;
		}

		NativeClassInfo(const String& aClassName, const String& aBaseClassName, size_t aRuntimeHash) : INativeClassInfo(aClassName, aBaseClassName, aRuntimeHash){ }

		virtual void* CreateInstance() const override
		{
			return INativeClassInfo::CreateInstance < T >(); // Called from base, because reflected classes are automaticalled declared friends of the interface.
		};
	};
}