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

	class CORE_API Type
	{
	protected:
		static std::unordered_map<size_t, Type* >& GetClassinfoNameMap();
		static std::unordered_map<size_t, Type* >& GetClassinfoHashMap();

		IClassFactory* classFactory;
		String className;
		String baseClassName;
		size_t runtimeHash; // This hash is ONLY useful for runtime comparisons. It may differ from build to build, or from compiler to compiler.
		Type* baseClass = nullptr;
		DynamicArray<Type*> derivedClasses;

		Type(IClassFactory* aClassFactory, const String& aClassName, const String& aBaseClassName, size_t aRuntimeHash)
		{
			classFactory = aClassFactory;
			className = aClassName;
			baseClassName = aBaseClassName;
			runtimeHash = aRuntimeHash;
		}
	public:
		static Type* Register(IClassFactory* aClassFactory, size_t aRuntimeHash, const char* aClassName, const char* aBaseClassName = "");

		void* CreateInstance() const{ return classFactory->CreateInstance(); }

		String GetTypeName() const{ return className; };

		Type* GetBaseClass() const { return baseClass; }
		DynamicArray<Type*> GetDerivedClasses() const { return derivedClasses; }

		static DynamicArray<Type*> GetAllTypes();
		static Type* GetType(const StringID& aTypeName);
		template<class T>static Type* GetType()
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

		bool IsA(Type* otherClass) const;

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

#define BASETYPE_DECLARATION(ClassIdentifier) \
friend class Type;\
protected:\
	static IClassFactory* ClassIdentifier##_ClassFactory;\
	static Type* ClassIdentifier##_ClassInfo;\
public:\
	virtual Type* GetClassInfo() const { return ClassIdentifier##_ClassInfo; }

#define TYPE_DECLARATION(ClassIdentifier, BaseIdentifier)\
	BASETYPE_DECLARATION(ClassIdentifier)

#define DEFINE_TYPE_FACTORY(ClassIdentifier)\
class ClassIdentifier##_Factory : public IClassFactory { public: virtual void* CreateInstance() override { return new ClassIdentifier(); } };\
IClassFactory* ClassIdentifier::ClassIdentifier##_ClassFactory = new ClassIdentifier##_Factory();\

#define BASETYPE_DEFINITION(ClassIdentifier)\
	DEFINE_TYPE_FACTORY(ClassIdentifier)\
	Type* ClassIdentifier::ClassIdentifier##_ClassInfo = Type::Register(ClassIdentifier::ClassIdentifier##_ClassFactory, typeid(ClassIdentifier).hash_code(), #ClassIdentifier);

#define TYPE_DEFINITION(ClassIdentifier, BaseIdentifier)\
	DEFINE_TYPE_FACTORY(ClassIdentifier)\
	Type* ClassIdentifier::ClassIdentifier##_ClassInfo = Type::Register(ClassIdentifier::ClassIdentifier##_ClassFactory, typeid(ClassIdentifier).hash_code(), #ClassIdentifier, #BaseIdentifier);