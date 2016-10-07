#pragma once
#include "Core.h"
#include "Conversions.h"

#ifdef Renderer2D_EXPORTS
#define EDITOR_API API_EXPORT
#else
#define EDITOR_API API_IMPORT
#endif