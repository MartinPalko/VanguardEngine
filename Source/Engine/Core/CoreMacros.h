#pragma once
#include "Platform.h"

#ifdef Core_EXPORTS
#define CORE_API API_EXPORT
#else
#define CORE_API API_IMPORT
#endif