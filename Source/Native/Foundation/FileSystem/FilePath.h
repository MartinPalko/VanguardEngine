#pragma once

#include "juce_core.h"
#include "VanguardTypes.h"

namespace Vanguard
{
	// Represents either a file or directory on the machines filesystem.
	// It's basically just a string, but with functionality to querey with the assumption of it being a file path (like getting file extension, parent directory, etc)
	// Is actually just a Wrapper for juce::File
	class FilePath
	{
		friend struct FileSystem;

	private:
		juce::File file; // The actual data of the class we're wrapping

	public:
		// ----------- Constructors -----------

		FilePath(const String& aAbsolutePath)
		{
			file = juce::File(aAbsolutePath);
		}

		FilePath(juce::File aJuceFile)
		{
			file = aJuceFile;
		}

		FilePath()
		{
			file = juce::File();
		}

		// ----------- Operators -----------

		inline operator const juce::File&() const { return file; }
		inline operator const String() const { return GetFullPathName(); }
		inline operator const char*() const
		{
			// NOTE: Returning a const char* will only work with "getFullPathName" because it returns a reference to the string stored inside the file class, not a value. Therefore it's still valid outside the scope of this function.
			return file.getFullPathName().getCharPointer();
		}

		inline bool operator== (const FilePath& other) const { return other.file == file; }
		inline bool operator!= (const FilePath& other) const { return other.file != file; }
		
		inline FilePath operator+ (const FilePath& other) const { return Append(other.GetFullPathName()); }
		inline FilePath operator+ (const String& str) const { return Append(str); }
		inline FilePath operator+ (const char* charPointer) const { return Append(charPointer); }

		inline FilePath operator-- () const { return GetParentDirectory(); }

		// ----------- Methods -----------

		// Gets the entire path name, including drive, filename, and extension if applicable.		
		inline const String GetFullPathName() const
		{
			return String(file.getFullPathName());
		}

		// Gets the filename including extension. If it's a directory, will get the name of the final folder.
		inline String GetFilename() const
		{
			return file.getFileName();
		}

		// Returns filename, excluding the extension
		inline String GetFilenameWithoutExtension() const
		{
			return file.getFileNameWithoutExtension();
		}

		// Gets the extension of the file, including the period.
		inline String GetFileExtension() const
		{
			return file.getFileExtension();
		}

		// Check if the file has the given extension.
		inline bool HasFileExtension(const String& aExtension) const
		{
			return file.hasFileExtension(aExtension);
		}

		// Get the directory this filepath resides in.
		inline FilePath GetParentDirectory() const
		{
			return file.getParentDirectory();
		}
		
		// Make a new filepath that is relative to this one based on the provided relative path
		inline FilePath GetRelative(const String& aRelativePath) const
		{
			return file.getChildFile(aRelativePath);
		}

		// Return a new FilePah with text appended directly to the full path name. Useful for adding file extensions.
		inline FilePath Append(const String& aRelativePath) const
		{
			return FilePath(GetFullPathName() + aRelativePath);
		}


	};
}