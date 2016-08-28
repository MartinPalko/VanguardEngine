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
		juce::DynamicLibrary* data;
		String lastError;

		// Private because they should not be used.
		DynamicLibrary(const DynamicLibrary& aOther){}
		DynamicLibrary& operator = (const DynamicLibrary& aOther){ return *this; }

		void RecordError();
	public:

		DynamicLibrary();
		~DynamicLibrary();

		bool Open(const FilePath& aFilePath);
		bool Open(const String& aLib, bool aExact = false);
		void Close();
		String GetLastError() { return lastError; }
		void* GetFunction(const String& aFunctionName);

	};
}