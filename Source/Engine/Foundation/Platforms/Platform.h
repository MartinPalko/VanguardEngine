#pragma once

#if DEBUG || _DEBUG
#define VANGUARD_DEBUG 1
#else
#define VANGUARD_RELEASE 1
#endif

#define MODULE_NAME_STRING MACRO_VALUE_TO_STRING(LIBRARY_NAME)
#define MACRO_VALUE_TO_STRING(s) SYMBOL_AS_STRING(s)
#define SYMBOL_AS_STRING(s) #s

#if _WIN32
#include "WindowsPlatform.h"
#define VANGUARD_WINDOWS 1
#elif TARGET_OS_MAC
#include "OSXPlatform.h"
#define VANGUARD_MAC 1
#elif __linux || __GNUC__
#include "LinuxPlatform.h"
#define VANGUARD_LINUX 1
#else
#error "Trying to compile for unsupported platform!"
#endif

#if VANGUARD_WINDOWS
#define API_EXPORT __declspec(dllexport)
#define API_IMPORT __declspec(dllimport)
#else
#define API_EXPORT
#define API_IMPORT
#endif
