#pragma once

#include "NativeClassInfo.h"

#define REFLECTED_BASECLASS_DECLARATION(ClassIdentifier) \
friend class INativeClassInfo;\
protected:\
	static INativeClassInfo* ClassIdentifier##_ClassInfo;\
public:\
	virtual INativeClassInfo* GetClassInfo() const { return ClassIdentifier##_ClassInfo; }

#define REFLECTED_SUBCLASS_DECLARATION(ClassIdentifier, BaseIdentifier)\
	REFLECTED_BASECLASS_DECLARATION(ClassIdentifier)

#define REFLECTED_BASECLASS_DEFINITION(ClassIdentifier)\
	INativeClassInfo* ClassIdentifier::ClassIdentifier##_ClassInfo = NativeClassInfo<ClassIdentifier>::Create(#ClassIdentifier);

#define REFLECTED_SUBCLASS_DEFINITION(ClassIdentifier, BaseIdentifier)\
	INativeClassInfo* ClassIdentifier::ClassIdentifier##_ClassInfo = NativeClassInfo<ClassIdentifier>::Create(#ClassIdentifier,#BaseIdentifier);