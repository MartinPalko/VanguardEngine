#include "juce_core.h"
#include "DynamicLibrary.h"

#ifdef VANGUARD_LINUX
#include <dlfcn.h>
#endif

namespace Vanguard
{
	DynamicLibrary::DynamicLibrary()
		: data(new juce::DynamicLibrary())
		, lastError()
	{
	}

	DynamicLibrary::~DynamicLibrary()
	{
		delete data;
	}

	void DynamicLibrary::RecordError()
	{
		#if VANGUARD_LINUX
		lastError = dlerror();
		#endif
		// TODO: Other platforms
	}

	bool DynamicLibrary::Open(const FilePath& aFilePath)
	{
		return Open(aFilePath.GetFullPathName(), true);
	}

	bool DynamicLibrary::Open(const String& aLib, bool aExact)
	{
		if (aExact)
		{
			if (data->open(aLib.GetCharPointer()))
			{
				return true;
			}
			else
			{
				RecordError();
				return false;
			}
		}

		String filePath = aLib;

		// Add the correct file extension if none specified.
		if (!filePath.Contains('.'))
		{
			filePath = filePath.Append(Platform::DynamicLibExtension());
		}

		if (data->open(filePath.GetCharPointer()))
		{
			return true;
		}
		
		// Try adding the "lib" prefix.
		if (!filePath.BeginsWith("lib"))
		{
			filePath = "lib" + filePath;
		}

		return data->open(filePath.GetCharPointer());

	}

	void DynamicLibrary::Close()
	{
		data->close();
	}

	void* DynamicLibrary::GetFunction(const String& aFunctionName)
	{
		return data->getFunction(aFunctionName.GetCharPointer());
	}
}