#pragma once
#include "Platforms/Platform.h"

#define STRINGIFY(x) #x
#define TO_STRING(x) STRINGIFY(x)

#ifdef Foundation_EXPORTS
#define FOUNDATION_API API_EXPORT
#else
#define FOUNDATION_API API_IMPORT
#endif