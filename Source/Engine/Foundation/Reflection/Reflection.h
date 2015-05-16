#pragma once

#include "TypeInfo.h"

#define DllExport   __declspec( dllexport )

// Place in .h
#define V_REFLECTED_CLASS(Class)\
class Class;\
DllExport extern const TTypeInfo<Class>* Class##_type;\
//
//// Place in .h, inside class body
//#define V_REFLECTED_CLASS_BODY(Class)//\
//public:\
//	virtual TypeInfo* GetType()\
//	{\
//		return Class##_type;\
//	}

// Place in .cpp
#define V_REFLECTED_CLASS_DEFINITION(Class)\
	const TTypeInfo<Class>* Class##_type = new TTypeInfo<Class>(#Class);