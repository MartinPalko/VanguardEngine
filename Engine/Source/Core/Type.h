#pragma once
#include <typeinfo>
#include "Foundation.h"
#include "Core_Common.h"
#include "Log.h"

namespace Vanguard
{
	struct IClassFactory
	{
		virtual void* CreateInstance() const = 0;
	};

	class CORE_API Type
	{
	protected:
		static std::unordered_map<size_t, Type* >& GetClassinfoHashMap();
		static std::unordered_map<size_t, Type* >& GetClassinfoNameMap();

		IClassFactory* classFactory;
		String className;
		String baseClassName;
		size_t runtimeHash; // This hash is ONLY useful for runtime comparisons. It may differ from build to build, or from compiler to compiler.
		Type* baseClass = nullptr;
		DynamicArray<Type*> derivedClasses;

		Type(IClassFactory* aClassFactory, const String& aClassName, const String& aBaseClassName, size_t aRuntimeHash);
	public:
		~Type();

		static Type* Register(IClassFactory* aClassFactory, size_t aRuntimeHash, const char* aClassName, const char* aBaseClassName = "");

		void* CreateInstance() const{ return classFactory->CreateInstance(); }

		String GetTypeName() const{ return className; };
		size_t GetRuntimeHash() const { return runtimeHash; }
		Type* GetBaseClass() const { return baseClass; }
		DynamicArray<Type*> GetDerivedClasses() const;

		static DynamicArray<Type*> GetAllTypes();
		static Type* GetType(const StringID& aTypeName);
		template<class T>static Type* GetType()
		{
			const size_t typeHash = typeid(T).hash_code();
#if VANGUARD_DEBUG
			if (!GetClassinfoHashMap().count(typeHash))
			{
				LOG_EXCEPTION("Trying to get class info for unregistered type: " + String(typeid(T).raw_name()), "Type");
			}
#endif
			return GetClassinfoHashMap()[typeHash];
		}

		bool IsA(Type* otherClass) const;

		static void UpdateHierarchyReferences();
	};
}

#define BASETYPE_DECLARATION(ClassIdentifier) \
friend class Type;\
protected:\
	static Vanguard::IClassFactory* ClassIdentifier##_ClassFactory;\
	static std::shared_ptr<Type> ClassIdentifier##_ClassInfo;\
public:\
	virtual Type* GetClassInfo() const { return &*ClassIdentifier##_ClassInfo; }

#define TYPE_DECLARATION(ClassIdentifier, BaseIdentifier)\
	BASETYPE_DECLARATION(ClassIdentifier)

#define DEFINE_TYPE_FACTORY(ClassIdentifier)\
class ClassIdentifier##_Factory : public Vanguard::IClassFactory { virtual void* CreateInstance() const override { return new ClassIdentifier(); } };\
Vanguard::IClassFactory* ClassIdentifier::ClassIdentifier##_ClassFactory = new ClassIdentifier##_Factory();\

#define BASETYPE_DEFINITION(ClassIdentifier)\
	DEFINE_TYPE_FACTORY(ClassIdentifier)\
	std::shared_ptr<Type> ClassIdentifier::ClassIdentifier##_ClassInfo (Type::Register(ClassIdentifier::ClassIdentifier##_ClassFactory, typeid(ClassIdentifier).hash_code(), #ClassIdentifier));

#define TYPE_DEFINITION(ClassIdentifier, BaseIdentifier)\
	DEFINE_TYPE_FACTORY(ClassIdentifier)\
	std::shared_ptr<Type> ClassIdentifier::ClassIdentifier##_ClassInfo (Type::Register(ClassIdentifier::ClassIdentifier##_ClassFactory, typeid(ClassIdentifier).hash_code(), #ClassIdentifier, #BaseIdentifier));
