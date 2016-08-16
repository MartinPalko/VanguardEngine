#pragma once
#include "Foundation.h"
#include "Core_Common.h"
#include "Log.h"
#include <typeinfo>

namespace Vanguard
{
	class IClassFactory
	{
	public:
		virtual void* CreateInstance() = 0;
	};

	class CORE_API NativeClassInfo
	{
	protected:
		static std::unordered_map<size_t, NativeClassInfo* >& GetClassinfoNameMap();
		static std::unordered_map<size_t, NativeClassInfo* >& GetClassinfoHashMap();

		IClassFactory* classFactory;
		String className;
		String baseClassName;
		size_t runtimeHash; // This hash is ONLY useful for runtime comparisons. It may differ from build to build, or from compiler to compiler.
		NativeClassInfo* baseClass = nullptr;
		DynamicArray<NativeClassInfo*> derivedClasses;

		NativeClassInfo(IClassFactory* aClassFactory, const String& aClassName, const String& aBaseClassName, size_t aRuntimeHash)
		{
			classFactory = aClassFactory;
			className = aClassName;
			baseClassName = aBaseClassName;
			runtimeHash = aRuntimeHash;
		}
	public:
		static NativeClassInfo* Register(IClassFactory* aClassFactory, size_t aRuntimeHash, const char* aClassName, const char* aBaseClassName = "");

		void* CreateInstance() const{ return classFactory->CreateInstance(); }

		String GetTypeName() const{ return className; };

		NativeClassInfo* GetBaseClass() const { return baseClass; }
		DynamicArray<NativeClassInfo*> GetDerivedClasses() const { return derivedClasses; }

		static DynamicArray<NativeClassInfo*> GetAllTypes();
		static NativeClassInfo* GetType(const StringID& aTypeName);
		template<class T>static NativeClassInfo* GetType()
		{
			const size_t typeHash = typeid(T).hash_code();
#if VANGUARD_DEBUG
			if (!GetClassinfoHashMap().count(typeHash))
			{
				Log::Exception("Trying to get class info for unregistered type: " + String(typeid(T).raw_name()));
			}
#endif
			return GetClassinfoHashMap()[typeHash];
		}

		bool IsA(NativeClassInfo* otherClass) const;

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
}