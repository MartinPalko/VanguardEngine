#pragma once
#include "Core.h"
using namespace Vanguard;

#ifdef VanguardSDL_EXPORTS
#define VANGUARDSDL_API API_EXPORT
#else
#define VANGUARDSDL_API API_IMPORT
#endif

// Enables check to make sure we're using the proper config.h
#define USING_GENERATED_CONFIG_H
// Include the generated config (If included by SDL headers, they will prefer the non-generated config)
#include <SDL_config.h>
#include <SDL.h>