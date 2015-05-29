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

		// ----------- Methods -----------

		// True if the file exists as either a file or directory
		bool Exists() const
		{
			return file.exists(); 
		}

		// True is path points to a file that exists. False if it points to a directory, or a file that doesn't exist.
		bool ExistsAsFile() const
		{
			return file.existsAsFile();
		}

		// True if path points to a directory that exists. False if it points to a file, or directory that doesn't exist.
		bool ExistsAsDirectory() const
		{
			return file.isDirectory();
		}

		// Gets filesize in bytes. Returns 0 if it doesn't exist.
		uint64 GetFilesizeInBytes() const
		{
			return file.getSize(); 
		}

		// Gets a human readable string of the file size, for example "100 bytes", "2KB", or "2 MB".
		string HumanReadableFileSize() const
		{
			return StringUtils::FromJuceString(file.descriptionOfSizeInBytes(GetFilesizeInBytes()));
		}

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

		// Returns a hashed version of the path, useful for quickly checking if directories are equal.
		int32 GetHash() const
		{
			return file.hashCode();
		}

		// Get the directory this filepath resides in.
		FilePath GetParentDirectory() const
		{
			return file.getParentDirectory();
		}
		
		// Make a new filepath that is relative to this
		FilePath Append(string aRelativePath) const
		{
			return file.getChildFile(juce::StringRef(aRelativePath));
		}

		// Check if we have write accesss to this file.
		bool HasWriteAccess()
		{
			return file.hasWriteAccess();
		}

		// Set file's readonly status, and optionally apply recursively to children. Returns false if not able to modify readonly property.
		bool SetReadonly(bool aShouldBeReadOnly, bool aApplyRecursively = false) const
		{
			return file.setReadOnly(aShouldBeReadOnly, aApplyRecursively);
		}


		// Returns true is the file is set to hidden.
		bool IsHidden() const
		{
			return file.isHidden();
		}
		
		// Create an empty file on the disk if it doesn't already exist. Returns the result (error message if failed)
		string CreateFileOnDisk() const
		{
			return StringUtils::FromJuceString(file.create().getErrorMessage());
		}

		// Create an empty directory on the disk if it doesn't already exist. Returns the result (error message if failed)
		string CreateDirectoryOnDisk() const
		{
			return StringUtils::FromJuceString(file.createDirectory().getErrorMessage());
		}

		// Deletes a file or directory from the disk. Returns false if delete fails.
		bool DeleteFromDisk() const
		{
			if (ExistsAsFile())
				return file.deleteFile();
			else
				return file.deleteRecursively();
		}

		// Alternative to delete that moves the file to the trash (or recycle bin on win) and allows the user to recover it if needed. Returns false if file can't be moved.
		bool MoveToTrash() const
		{
			return file.moveToTrash();
		}

		// Moves file or directory to a specific path. Note: This should not be the directory you want to put it in but the actual filename.
		bool MoveTo(const FilePath& aTargetLocation) const
		{
			if (file.isDirectory())
			{
				// Juce doesn't contain a move directory, so copy it, and then delete the old one.
				if (CopyTo(aTargetLocation))
					return DeleteFromDisk();
				else return false;
			}
			else
				return file.moveFileTo(aTargetLocation);
		}

		// Copies file or directory to a specific path. Note: This should not be the directory you want to put it in but the actual filename.
		bool CopyTo(const FilePath& aTargetLocation) const
		{
			if (file.isDirectory())
				return file.copyDirectoryTo(aTargetLocation);
			else
				return file.copyFileTo(aTargetLocation);
		}

		// Moves file or directory into a specified path, retaining it's same name an extension
		bool MoveInto(const FilePath& aTargetDirectory) const
		{
			return MoveTo(aTargetDirectory.Append(GetFilename()));
		}

		// Copies file or directory into a specified path, retaining it's same name an extension
		bool CopyInto(const FilePath& aTargetDirectory) const
		{
			return CopyTo(aTargetDirectory.Append(GetFilename()));
		}

		// Searches subdirectories for files, or other directories matching the wildcard criteria.
		List<FilePath> FindFilesInSubdirectories(const string& aWildCardPattern = "*", bool aSearchRecursively = true, bool aFindFiles = true, bool aFindDirectories = false, bool aIgnoreHiddenFiles = false) const
		{
			juce::Array<juce::File> result;
			int whatToLookFor = 0 + (aFindFiles ? 1 : 0) + (aFindDirectories ? 2 : 0) + (aIgnoreHiddenFiles ? 4 : 0);
			file.findChildFiles(result, whatToLookFor, aSearchRecursively, aWildCardPattern);
			
			List<FilePath> returnList = List<FilePath>();
			returnList.resize(result.size());

			for (int i = 0; i < result.size(); i++)
			{
				returnList[i] = result[i];
			}

			return returnList;
		}

	};
}