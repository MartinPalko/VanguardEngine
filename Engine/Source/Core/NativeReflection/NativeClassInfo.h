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
		//static DynamicArray<INativeClassInfo*> allClassInfos;
		static DynamicArray<INativeClassInfo*>& GetAllClassInfosList();

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
			size_t h = typeid(T).hash_code();
			DynamicArray<INativeClassInfo*>& allClassInfos = GetAllClassInfosList();
			for (uint32 i = 0; i < allClassInfos.Count(); i++)
			{
				if (allClassInfos[i]->runtimeHash == h)
					return allClassInfos[i];
			}
			return nullptr;
		}

		bool IsA(INativeClassInfo* otherClass) const;

		static void UpdateHierarchyReferences()
		{
			DynamicArray<INativeClassInfo*>& allClassInfos = GetAllClassInfosList();

			// First clear all references.
			for (uint32 i = 0; i < allClassInfos.Count(); i++)
			{
				allClassInfos[i]->baseClass = nullptr;
				allClassInfos[i]->derivedClasses.Clear();
			}
			// Now rebuild
			for (uint32 i = 0; i < allClassInfos.Count(); i++)
			{
				allClassInfos[i]->baseClass = GetType(allClassInfos[i]->baseClassName);
				if (allClassInfos[i]->baseClass)
					allClassInfos[i]->baseClass->derivedClasses.PushBack(allClassInfos[i]);
			}
		}
	};

	template <class T> class NativeClassInfo : public INativeClassInfo
	{

	public:
		static INativeClassInfo* Create(const char* aClassName, const char* aBaseClassName = "")
		{
			String ClassName = typeid(T).name();
				
			DynamicArray<INativeClassInfo*>& allClassInfos = GetAllClassInfosList();

			for (uint32 i = 0; i < allClassInfos.Count(); i++)
			{
				if (allClassInfos[i]->GetTypeName() == ClassName)
				{
					return allClassInfos[i];
				}
			}
			NativeClassInfo<T>* newClassInfo;
			{
				String cname = aClassName;
				String bname = aBaseClassName;
				size_t hash = typeid(T).hash_code();
				newClassInfo = new NativeClassInfo<T>(cname, bname, hash);
			}
			allClassInfos.PushBack(newClassInfo);

			// TODO: Do this initially, and then again whenever a dll is loaded or unloaded.
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