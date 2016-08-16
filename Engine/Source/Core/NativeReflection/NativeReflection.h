#pragma once

#include "NativeClassInfo.h"

#define REFLECTED_BASECLASS_DECLARATION(ClassIdentifier) \
friend class NativeClassInfo;\
protected:\
	static IClassFactory* ClassIdentifier##_ClassFactory;\
	static NativeClassInfo* ClassIdentifier##_ClassInfo;\
public:\
	virtual NativeClassInfo* GetClassInfo() const { return ClassIdentifier##_ClassInfo; }

#define REFLECTED_SUBCLASS_DECLARATION(ClassIdentifier, BaseIdentifier)\
	REFLECTED_BASECLASS_DECLARATION(ClassIdentifier)

#define DEFINE_FACTORY(ClassIdentifier)\
class ClassIdentifier##_Factory : public IClassFactory { public: virtual void* CreateInstance() override { return new ClassIdentifier(); } };\
IClassFactory* ClassIdentifier::ClassIdentifier##_ClassFactory = new ClassIdentifier##_Factory();\

#define REFLECTED_BASECLASS_DEFINITION(ClassIdentifier)\
	DEFINE_FACTORY(ClassIdentifier)\
	NativeClassInfo* ClassIdentifier::ClassIdentifier##_ClassInfo = NativeClassInfo::Register(ClassIdentifier::ClassIdentifier##_ClassFactory, typeid(ClassIdentifier).hash_code(), #ClassIdentifier);

#define REFLECTED_SUBCLASS_DEFINITION(ClassIdentifier, BaseIdentifier)\
	DEFINE_FACTORY(ClassIdentifier)\
	NativeClassInfo* ClassIdentifier::ClassIdentifier##_ClassInfo = NativeClassInfo::Register(ClassIdentifier::ClassIdentifier##_ClassFactory, typeid(ClassIdentifier).hash_code(), #ClassIdentifier, #BaseIdentifier);