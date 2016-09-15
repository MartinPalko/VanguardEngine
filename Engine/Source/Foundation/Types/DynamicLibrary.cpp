#include "DynamicLibrary.h"

#if defined(VANGUARD_WINDOWS)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
// Undefine stuff that windows defines...
#undef GetFullPathName
#elif defined(VANGUARD_LINUX)
#include <dlfcn.h>
#endif

namespace Vanguard
{
	DynamicLibrary::DynamicLibrary()
		: handle(nullptr)
		, lastError()
	{
	}

	DynamicLibrary::~DynamicLibrary()
	{
	}

	void DynamicLibrary::RecordError()
	{
		#if VANGUARD_LINUX
		lastError = dlerror();
		#endif
		// TODO: Other platforms
	}

	bool DynamicLibrary::DoOpen(const String& aLib)
	{
		Close();

#if defined(VANGUARD_WINDOWS)
			handle = LoadLibrary(aLib.GetCharPointer());
#else
			handle = dlopen (aLib.GetCharPointer(), RTLD_LOCAL | RTLD_NOW);
#endif

		if (!handle)
			RecordError();

		return handle != nullptr;
	}

	bool DynamicLibrary::Open(const FilePath& aFilePath)
	{
		return Open(aFilePath.GetFullPathName(), true);
	}

	bool DynamicLibrary::Open(const String& aLib, bool aExact)
	{
		if (aExact)
		{
			return DoOpen(aLib);
		}

		String filePath = aLib;

		// Add the correct file extension if none specified.
		if (!filePath.Contains('.'))
		{
			filePath = filePath.Append(Platform::DynamicLibExtension());
		}

		if (DoOpen(filePath.GetCharPointer()))
		{
			return true;
		}
		
		// Try adding the "lib" prefix.
		if (!filePath.BeginsWith("lib"))
		{
			filePath = "lib" + filePath;
			return DoOpen(filePath.GetCharPointer());
		}
		else
			return false;
	}

	void DynamicLibrary::Close()
	{
		if (handle)
		{
#if defined(VANGUARD_WINDOWS)
			FreeLibrary ((HMODULE)handle);
#else
			dlclose(handle);
#endif
			handle = nullptr;
		}
	}

	void* DynamicLibrary::GetFunction(const String& aFunctionName)
	{
		if (handle)
		{
			void* funcPtr = nullptr;
#if defined(VANGUARD_WINDOWS)
			funcPtr = GetProcAddress((HMODULE)handle, aFunctionName.GetCharPointer());
#else
			funcPtr = dlsym(handle, aFunctionName.GetCharPointer());
#endif
			if (!funcPtr)
				RecordError();
			return funcPtr;
		}
		return nullptr;
	}
}