#pragma once
#include "Core.h"
using namespace Vanguard;

#ifdef VanguardSDL_EXPORTS
#define VANGUARDSDL_API API_EXPORT
#else
#define VANGUARDSDL_API API_IMPORT
#endif