#pragma once
#include "Platforms/Platform.h"

// Enable memory leak detection on windows debug builds
#if VANGUARD_DEBUG
#define MEMORY_LEAK_DEBUG_ENABLED 1
#endif

#if VANGUARD_WINDOWS && MEMORY_LEAK_DEBUG_ENABLED
#define VANGUARD_DUMP_MEMORY_LEAK_DEBUG _CrtDumpMemoryLeaks
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

// Override new operator
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW

#endif

#define STRINGIFY(x) #x
#define TO_STRING(x) STRINGIFY(x)

#ifdef Foundation_EXPORTS
#define FOUNDATION_API API_EXPORT
#else
#define FOUNDATION_API API_IMPORT
#endif