#pragma once

#include "juce_core.h"
#include "Types.h"
#include "StringUtils.h"

namespace Vanguard
{
	// Represents either a file or directory on the machines filesystem.
	// Is actually just a Wrapper for juce::File
	class FilePath
	{
		friend struct FileSystem;

	private:
		juce::File file; // The actual data of the class we're wrapping

	public:
		// ----------- Constructors -----------

		FilePath (const string& aAbsolutePath)
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

		operator juce::File() const { return file; }
		operator string() const { return GetFullPathName(); }

		bool operator== (const FilePath& other) const { return other.file == file; }
		bool operator!= (const FilePath& other) const { return other.file != file; }
		
		FilePath operator+ (const FilePath& other) const { return GetRelative(other.GetFullPathName()); }
		FilePath operator+ (const string& str) const { return GetRelative(str); }

		FilePath operator-- () const { return GetParentDirectory(); }

		// ----------- Methods -----------

		// Gets the entire path name, including drive, filename, and extension if applicable.		
		string GetFullPathName() const
		{
			return StringUtils::FromJuceString(file.getFullPathName());
		}

		// Gets the filename including extension. If it's a directory, will get the name of the final folder.
		string GetFilename() const
		{
			return StringUtils::FromJuceString(file.getFileName());
		}

		// Returns filename, excluding the extension
		string GetFilenameWithoutExtension() const
		{
			return StringUtils::FromJuceString(file.getFileNameWithoutExtension());
		}

		// Gets the extension of the file, including the period.
		string GetFileExtension() const
		{
			return StringUtils::FromJuceString(file.getFileExtension());
		}

		// Check if the file has the given extension.
		bool HasFileExtension(const string& aExtension) const
		{
			return file.hasFileExtension(juce::StringRef(aExtension));
		}

		// Get the directory this filepath resides in.
		FilePath GetParentDirectory() const
		{
			return file.getParentDirectory();
		}
		
		// Make a new filepath that is relative to this one based on the provided relative path
		FilePath GetRelative(string aRelativePath) const
		{
			return file.getChildFile(juce::StringRef(aRelativePath));
		}
	};
}