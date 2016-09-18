#pragma once
#include "VanguardString.h"
#include "Foundation_Common.h"

namespace juce
{
	class File;
}

namespace Vanguard
{
	// Represents either a file or directory on the machines filesystem.
	// It's basically just a string, but with functionality to querey with the assumption of it being a file path (like getting file extension, parent directory, etc)
	// Is actually just a Wrapper for juce::File
	class FOUNDATION_API FilePath
	{
		friend struct FileSystem;

	private:
		juce::File* file; // The actual data of the class we're wrapping

	public:
		// ----------- Constructors -----------

		FilePath(const String& aAbsolutePath);
		FilePath();
		FilePath(const FilePath& aOther);

		FilePath& operator = (const FilePath& aOther);
		FilePath& operator = (FilePath&& aOther);

		~FilePath();



		// ----------- Operators -----------

		inline operator const String() const { return GetFullPathName(); }

		inline bool operator== (const FilePath& other) const { return other.GetFullPathName() == GetFullPathName(); }
		inline bool operator!= (const FilePath& other) const { return other.GetFullPathName() != GetFullPathName(); }

		inline FilePath operator+ (const FilePath& other) const { return Append(other.GetFullPathName()); }
		inline FilePath operator+ (const String& str) const { return Append(str); }
		inline FilePath operator+ (const char* charPointer) const { return Append(charPointer); }

		inline FilePath operator-- () const { return GetParentDirectory(); }

		operator bool() const;

		// ----------- Methods -----------

		// Gets the entire path name, including drive, filename, and extension if applicable.
		const String GetFullPathName() const;

		// Gets the filename including extension. If it's a directory, will get the name of the final folder.
		String GetFilename() const;

		// Returns filename, excluding the extension
		String GetFilenameWithoutExtension() const;

		// Gets the extension of the file, including the period.
		String GetFileExtension() const;

		// Check if the file has the given extension.
		bool HasFileExtension(const String& aExtension) const;

		// Get the directory this filepath resides in.
		FilePath GetParentDirectory() const;

		// Make a new filepath that is relative to this one based on the provided relative path
		FilePath GetRelative(const String& aRelativePath) const;

		// Return a new FilePah with text appended directly to the full path name. Useful for adding file extensions.
		FilePath Append(const String& aRelativePath) const;
	};
}
