#pragma once
#include "Core.h"
#include "VanguardSDL.h"
using namespace Vanguard;

#ifdef Renderer2D_EXPORTS
#define RENDERER2D_API API_EXPORT
#else
#define RENDERER2D_API API_IMPORT
#endif