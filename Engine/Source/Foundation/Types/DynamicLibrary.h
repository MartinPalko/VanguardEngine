#pragma once
#include "Foundation_Common.h"
#include "VanguardString.h"
#include "FilePath.h"

namespace juce
{
	class DynamicLibrary;
}

namespace Vanguard
{
	class FOUNDATION_API DynamicLibrary
	{
	private:
		juce::DynamicLibrary* data = nullptr;

		// Private because they should not be used.
		DynamicLibrary(const DynamicLibrary& aOther){}
		DynamicLibrary& operator = (const DynamicLibrary& aOther){ return *this; }
	public:

		DynamicLibrary();
		~DynamicLibrary();

		bool Open(const FilePath& aFilePath);
		bool Open(const String& aFileName);
		void Close();
		void* GetFunction(const String& aFunctionName);

	};
}