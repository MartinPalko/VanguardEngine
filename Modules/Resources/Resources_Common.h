#pragma once
#include "Core.h"
using namespace Vanguard;

#ifdef Resources_EXPORTS
#define RESOURCES_API API_EXPORT
#else
#define RESOURCES_API API_IMPORT
#endif

typedef UUID ResourceID;