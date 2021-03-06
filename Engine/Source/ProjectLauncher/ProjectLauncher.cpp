#include "Types/DynamicLibrary.h"
#include "Interfaces/ICore.h"

using namespace Vanguard;

int main(int argc, char **argv)
{
	int returnVal = 0;

	DynamicLibrary* coreLib = new DynamicLibrary();
	if (coreLib->Open("Core"))
	{
		T_VANGUARD_CREATE_CORE_FUNCTION createCoreFunction = (T_VANGUARD_CREATE_CORE_FUNCTION)coreLib->GetFunction(TO_STRING(VANGUARD_CREATE_CORE_FUNCTION));

		if (createCoreFunction)
		{
			ICore* VanguardCore = createCoreFunction();
			VanguardCore->Initialize(argc, argv, TO_STRING(VANGUARD_PROJECT));
			VanguardCore->CreateProjectWorld();

			VanguardCore->Run();

			delete VanguardCore;
		}
		else
		{
			returnVal = 2;
		}
	}
	else
	{
		returnVal = 1;
	}
	coreLib->Close();
	delete coreLib;

#ifdef MEMORY_LEAK_DEBUG_ENABLED
	VANGUARD_DUMP_MEMORY_LEAK_DEBUG();
#endif
	return returnVal;
}