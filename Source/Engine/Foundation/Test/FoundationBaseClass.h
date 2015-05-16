#pragma once
#include "Reflection.h"

V_REFLECTED_CLASS(FoundationBaseClass)

class DllExport FoundationBaseClass
{
public:
	FoundationBaseClass();

	virtual void DoThing()
	{
		std::cout << "\nI'M DOING A THING\n";
	}
};