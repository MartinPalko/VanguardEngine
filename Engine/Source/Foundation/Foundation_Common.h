#pragma once
#pragma warning(disable : 4244) // Conversion double to float

#include "Platforms/Platform.h"

#if VANGUARD_DEBUG
// Uncomment to enable memory leak detection on debug builds
//#define MEMORY_LEAK_DEBUG_ENABLED
#endif

#if VANGUARD_WINDOWS && defined(MEMORY_LEAK_DEBUG_ENABLED)
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
