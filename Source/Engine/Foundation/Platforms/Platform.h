#pragma once

#if _WIN32
#include "WindowsPlatform.h"
#elif TARGET_OS_MAC
#include "OSXPlatform.h"
#elif __linux
#include "LinuxPlatform.h"
#else
#error "Trying to compile for unsupported platform!"
#endif