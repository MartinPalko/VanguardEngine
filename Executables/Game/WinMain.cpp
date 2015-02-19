#include <iostream>
#include "Core.h"

int main()
{
	Core* core = new Core();
	std::cout << core->DoThing() << "\n";

	system("pause");

	delete core;
	return 0;
}