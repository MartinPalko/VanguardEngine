#pragma once
#include <typeinfo>
#include <unordered_map>
#include "Foundation.h"
#include "Core_Common.h"
#include "Log.h"

namespace Vanguard
{
	class CORE_API Property
	{
		String name;
		StringID id;
		size_t offset;
		size_t typeId;

	public:
		Property() = delete;

		Property(const String& aName, size_t aOffset, size_t aTypeId)
			: name(aName)
			, id(aName)
			, offset(aOffset) 
			, typeId(aTypeId)
		{}

		virtual ~Property() {}

		String GetName() { return name; }
		StringID GetId() { return id; }
		size_t GetOffset() { return offset; }

		template<class T> bool Is()
		{
			return typeid(T).hash_code() == typeID;
		}

		template<class T> T Get(void* aFromInstance)
		{
			return *(T*)((byte*)aFromInstance + offset);
		}

		template<class T> void Set(const T& aValue, void* aOnInstance)
		{
			*(T*)((byte*)aOnInstance + offset) = aValue;
		}
	};

	struct IClassFactory
	{
		virtual void* CreateInstance() const = 0;
	};

	// Type provides runtime information about a class, such as class name, base classes, and derived classes.
	// Type also provides a factory for creating new instances of the class it describes.
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
		std::unordered_map<StringID, Property*> properties;

		Type(IClassFactory* aClassFactory, const String& aClassName, const String& aBaseClassName, size_t aRuntimeHash);
	public:
		~Type();

		static Type* Register(IClassFactory* aClassFactory, size_t aRuntimeHash, const char* aClassName, const char* aBaseClassName = "");
		void RegisterProperty(Property* aProperty);

		bool IsAbstract() const { return classFactory == nullptr; }
		void* CreateInstance() const { return classFactory ? classFactory->CreateInstance() : nullptr; }

		String GetTypeName() const { return className; };
		size_t GetRuntimeHash() const { return runtimeHash; }
		Type* GetBaseClass() const { return baseClass; }
		DynamicArray<Type*> GetDerivedClasses() const;

		Property* GetProperty(StringID aName) const { return properties.at(aName); }

		static DynamicArray<Type*> GetAllTypes();
		static DynamicArray<Type*> GetAllBaseTypes();
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

// Type macros. You must use these macros inside your classes to register them with the type system.

// Declarations
#define ABSTRACT_BASETYPE_DECLARATION(ClassIdentifier)\
friend class Type;\
protected:\
	static std::shared_ptr<Type> ClassIdentifier##_Type;\
public:\
	virtual Type* GetType() const { return &*ClassIdentifier##_Type; }

#define ABSTRACT_TYPE_DECLARATION(ClassIdentifier, BaseIdentifier)\
ABSTRACT_BASETYPE_DECLARATION(ClassIdentifier)

#define BASETYPE_DECLARATION(ClassIdentifier) \
ABSTRACT_BASETYPE_DECLARATION(ClassIdentifier) \
protected:\
	class Factory : public Vanguard::IClassFactory { virtual void* CreateInstance() const override;};\
public:\
	static Factory ClassIdentifier##_Factory;\

#define TYPE_DECLARATION(ClassIdentifier, BaseIdentifier)\
	BASETYPE_DECLARATION(ClassIdentifier)

// Definitions
#define DEFINE_TYPE_FACTORY(ClassIdentifier)\
void* ClassIdentifier::Factory::CreateInstance() const { return new ClassIdentifier();}\
ClassIdentifier::Factory ClassIdentifier::ClassIdentifier##_Factory;\

#define BASETYPE_DEFINITION(ClassIdentifier)\
	DEFINE_TYPE_FACTORY(ClassIdentifier)\
	std::shared_ptr<Type> ClassIdentifier::ClassIdentifier##_Type (Type::Register(&ClassIdentifier::ClassIdentifier##_Factory, typeid(ClassIdentifier).hash_code(), #ClassIdentifier));

#define TYPE_DEFINITION(ClassIdentifier, BaseIdentifier)\
	DEFINE_TYPE_FACTORY(ClassIdentifier)\
	std::shared_ptr<Type> ClassIdentifier::ClassIdentifier##_Type (Type::Register(&ClassIdentifier::ClassIdentifier##_Factory, typeid(ClassIdentifier).hash_code(), #ClassIdentifier, #BaseIdentifier));

#define ABSTRACT_BASETYPE_DEFINITION(ClassIdentifier)\
	std::shared_ptr<Type> ClassIdentifier::ClassIdentifier##_Type (Type::Register(nullptr, typeid(ClassIdentifier).hash_code(), #ClassIdentifier));

#define ABSTRACT_TYPE_DEFINITION(ClassIdentifier, BaseIdentifier)\
	std::shared_ptr<Type> ClassIdentifier::ClassIdentifier##_Type (Type::Register(nullptr, typeid(ClassIdentifier).hash_code(), #ClassIdentifier, #BaseIdentifier));

// Properties
#define START_REGISTER_PROPERTIES(ClassIdentifier)\
struct ClassIdentifier##_propertyRegistrar\
{\
public:\
	ClassIdentifier##_propertyRegistrar()\
	{\
		Type* t = Type::GetType<ClassIdentifier>();

#define REGISTER_PROPERTY(ClassIdentifier, PropertyName) t->RegisterProperty(new Property(#PropertyName, sizeof(ClassIdentifier::PropertyName), typeid(ClassIdentifier::PropertyName).hash_code()));

#define FINISH_REGISTER_PROPERTIES(ClassIdentifier)\
	}\
};\
auto Transform_properties = ClassIdentifier##_propertyRegistrar();
