#include "juce_core.h"
#include "DynamicLibrary.h"

namespace Vanguard
{
	DynamicLibrary::DynamicLibrary()
	{
		data = new juce::DynamicLibrary();
	}

	DynamicLibrary::~DynamicLibrary()
	{
		delete data;
	}

	bool DynamicLibrary::Open(const FilePath& aFilePath)
	{
		return data->open(aFilePath.GetFullPathName().GetCharPointer());
	}

	bool DynamicLibrary::Open(const String& aFilePath)
	{
		if (!aFilePath.Contains('.'))
		{
			aFilePath.Append(String(".") + Platform::DynamicLibExtension());
		}

		return data->open(aFilePath.GetCharPointer());
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