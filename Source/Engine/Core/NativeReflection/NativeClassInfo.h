#pragma once
#include "Foundation.h"
#include "CoreMacros.h"

namespace Vanguard
{
	template<class T, class U> T base_of(U T::*);

	class CORE_API INativeClassInfo
	{
	protected:
		static List<INativeClassInfo*> allClassInfos;

		String className;
		INativeClassInfo* baseClass;
		List<INativeClassInfo*> derivedClasses;

		INativeClassInfo(const String& aClassName, const String& aBaseClassName)
		{
			className = aClassName;
			baseClass = GetType(aBaseClassName);
			if (baseClass)
				baseClass->derivedClasses.PushBack(this);
		}

		template<class T> static T* CreateInstance()
		{
			return new T();
		}
	public:
		virtual void* CreateInstance() const = 0;

		String GetTypeName() const{ return className; };

		INativeClassInfo* GetBaseClass() const { return baseClass; }
		List<INativeClassInfo*> GetDerivedClasses() const { return derivedClasses; }

		static List<INativeClassInfo*> GetAllTypes();
		static INativeClassInfo* GetType(const String& aTypeName);

		bool IsA(INativeClassInfo* otherClass) const;
	};

	template <class T> class NativeClassInfo : public INativeClassInfo
	{

	public:
		static INativeClassInfo* Create(const String& aBaseClassName = "")
		{
			for (uint32 i = 0; i < allClassInfos.Size(); i++)
			{
				if (allClassInfos[i]->GetTypeName() == typeid(T).name())
				{
					return allClassInfos[i];
				}
			}
			NativeClassInfo<T>* newClassInfo = new NativeClassInfo<T>(aBaseClassName);
			allClassInfos.PushBack(newClassInfo);
			return newClassInfo;
		}

		NativeClassInfo(const String& aBaseClassName) : INativeClassInfo(typeid(T).name(),aBaseClassName){ }

		virtual void* CreateInstance() const override
		{
			return INativeClassInfo::CreateInstance < T >(); // Calle from base, because reflected classes are automaticalled declared friends of the interface.
		};
	};
}