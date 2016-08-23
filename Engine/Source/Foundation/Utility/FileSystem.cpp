#include "juce_core.h"
#include "FileSystem.h"

namespace Vanguard
{
	FilePath FileSystem::GetSystemTempDirectory()
	{
		return FilePath(String(juce::File::getSpecialLocation(juce::File::tempDirectory).getFullPathName().getCharPointer()));
	}

	FilePath FileSystem::GetSystemProgramsDirectory()
	{
		return FilePath(String(juce::File::getSpecialLocation(juce::File::globalApplicationsDirectory).getFullPathName().getCharPointer()));
	}

	void FileSystem::SetCurrentWorkingDir(const FilePath& aFilePath)
	{
		aFilePath.file->setAsCurrentWorkingDirectory();
	}

	FilePath FileSystem::GetCurrentWorkingDir()
	{
		return FilePath(String(juce::File::getCurrentWorkingDirectory().getFullPathName().getCharPointer()));
	}

	FilePath FileSystem::GetHostApplicationPath()
	{
		return FilePath(String(juce::File::getSpecialLocation(juce::File::hostApplicationPath).getFullPathName().getCharPointer()));
	}

	// If a file or directory already exists at this path, this will return a path that does not exist by adding a unique suffix to the file (eg _1 or _2, until a filepath is found that does not already exist.
	FilePath FileSystem::MakeUniqueFileName(const FilePath& aFilePath)
	{
		FilePath newPath = aFilePath;

		int32 uniqueSuffix = 1;
		while (FileExists(newPath))
		{
			uniqueSuffix++;
			String newFilename = aFilePath.GetFilenameWithoutExtension() + "_" + String::FromInt(uniqueSuffix) + aFilePath.GetFileExtension();
			newPath = aFilePath.GetParentDirectory().GetRelative(newFilename);
		}
		return FilePath(newPath);
	}

	// Returns the time when the file was last modified. Returns invalid time if file does not exist.
	Time FileSystem::GetTimeLastModified(const FilePath& aFilePath)
	{
		return aFilePath.file->getLastModificationTime().toMilliseconds();
	}

	// Returns the time when the file was created. Returns invalid time if file does not exist.
	Time FileSystem::GetTimeCreated(const FilePath& aFilePath)
	{
		return aFilePath.file->getCreationTime().toMilliseconds();
	}

	// True if filepath exists as either a file or directory.
	bool FileSystem::Exists(const FilePath& aFilePath)
	{
		return FileExists(aFilePath) || DirectoryExists(aFilePath);
	}

	// True if path points to a file that exists. False if it points to a directory, or a file that doesn't exist.
	bool FileSystem::FileExists(const FilePath& aFilePath)
	{
		return aFilePath.file->existsAsFile();
	}

	// True if path points to a directory that exists. False if it points to a file, or directory that doesn't exist.
	bool FileSystem::DirectoryExists(const FilePath& aFilePath)
	{
		return aFilePath.file->isDirectory();
	}

	// Gets filesize in bytes. Returns 0 if it doesn't exist.
	uint64 FileSystem::GetFileSizeInBytes(const FilePath& aFilePath)
	{
		return aFilePath.file->getSize();
	}

	// Gets a human readable string of the file size, for example "100 bytes", "2KB", or "2 MB".
	String FileSystem::GetHumanReadableSize(const FilePath& aFilePath)
	{
		return GetHumanReadableSizeFromBytes(GetFileSizeInBytes(aFilePath));
	}

	// Gets a human readable string of the file size, for example "100 bytes", "2KB", or "2 MB".
	String FileSystem::GetHumanReadableSizeFromBytes(uint64 aNumBytes)
	{
		return String(juce::File::descriptionOfSizeInBytes(aNumBytes).getCharPointer());
	}

	// Checks to see if we have write access to a file or directory.
	bool FileSystem::HasWriteAccess(const FilePath& aFilePath)
	{
		return aFilePath.file->hasWriteAccess();
	}

	// Checks to see if we have write access to a file or directory.
	bool FileSystem::IsHidden(const FilePath& aFilePath)
	{
		return aFilePath.file->isHidden();
	}

	// Set file's readonly status, and optionally apply recursively to children. Returns false if not able to modify readonly property.
	bool FileSystem::SetReadOnly(const FilePath& aFilePath, bool aShouldBeReadOnly, bool aApplyRecursively)
	{
		return aFilePath.file->setReadOnly(aShouldBeReadOnly, aApplyRecursively);
	}

	// Create an empty file on the disk if it doesn't already exist. Returns the result (error message if failed)
	String FileSystem::CreateFile(const FilePath& aFilePath)
	{
		return String(aFilePath.file->create().getErrorMessage().getCharPointer());
	}

	// Write a plain text file containing the current string. If the file exists, it will be completely overwritten.
	bool FileSystem::WriteToFile(const FilePath& aFilePath, const String& aStringContents)
	{
		if (FileExists(aFilePath))
			if (!Delete(aFilePath))
				return false;

		if (!(CreateFile(aFilePath) == ""))
			return false;

		if (!HasWriteAccess(aFilePath))
			return false;

		juce::FileOutputStream* outStream = aFilePath.file->createOutputStream();
		outStream->setPosition(0);
		const bool result = outStream->writeText(aStringContents.GetCharPointer(), false, false);
		delete outStream;
		return result;
	}

	// Append text to an existing file-> If the file does not exist, will create it and fill it with the supplied text.
	bool FileSystem::AppendToFile(const FilePath& aFilePath, const String& aStringContents)
	{
		if (!FileExists(aFilePath))
			return WriteToFile(aFilePath, aStringContents);

		if (!HasWriteAccess(aFilePath))
			return false;

		juce::FileOutputStream* outStream = aFilePath.file->createOutputStream();
		const bool result = outStream->writeText(aStringContents.GetCharPointer(), false, false);
		delete outStream;
		return result;
	}

	String FileSystem::ReadFileAsText(const FilePath& aFilePath)
	{
		if (!FileExists(aFilePath))
			return "";

		juce::FileInputStream* inStream = aFilePath.file->createInputStream();
		String fileText = String(inStream->readEntireStreamAsString().getCharPointer());
		delete inStream;
		return fileText;
	}

	DynamicArray<String> FileSystem::ReadFileLinesAsText(const FilePath& aFilePath)
	{
		if (!FileExists(aFilePath))
			return DynamicArray <String>();

		DynamicArray<String> fileLines = DynamicArray <String>();

		juce::FileInputStream* inStream = aFilePath.file->createInputStream();
		inStream->setPosition(0);

		while (!inStream->isExhausted())
		{
			fileLines.PushBack(String(inStream->readNextLine().getCharPointer()));
		}

		delete inStream;
		return fileLines;
	}

	// Create an empty directory on the disk if it doesn't already exist. Returns the result (error message if failed)
	String FileSystem::CreateDirectory(const FilePath& aFilePath)
	{
		return String(aFilePath.file->createDirectory().getErrorMessage().getCharPointer());
	}

	// Deletes a file or directory from the disk. Returns false if delete fails.
	bool FileSystem::Delete(const FilePath& aFilePath)
	{
		if (FileExists(aFilePath))
			return aFilePath.file->deleteFile();
		else
			return aFilePath.file->deleteRecursively();
	}

	// Alternative to delete that moves the file to the trash (or recycle bin on win) and allows the user to recover it if needed. Returns false if file can't be moved.
	bool FileSystem::MoveToTrash(const FilePath& aFilePath)
	{
		return aFilePath.file->moveToTrash();
	}

	// Moves file or directory to a specific path. Note: This should not be the directory you want to put it in but the actual filename.
	bool FileSystem::MoveTo(const FilePath& aFilePath, const FilePath& aDestinationPath)
	{
		if (aFilePath.file->isDirectory())
		{
			// Juce doesn't contain a move directory, so copy it, and then delete the old one.
			if (CopyTo(aFilePath, aDestinationPath))
				return Delete(aFilePath);
			else return false;
		}
		else
			return aFilePath.file->moveFileTo(juce::File(*aDestinationPath.file));
	}

	// Copies file or directory to a specific path. Note: This should not be the directory you want to put it in but the actual filename.
	bool FileSystem::CopyTo(const FilePath& aFilePath, const FilePath& aTargetLocation)
	{
		if (aFilePath.file->isDirectory())
			return aFilePath.file->copyDirectoryTo(juce::File(aTargetLocation.GetFullPathName().GetCharPointer()));
		else
			return aFilePath.file->copyFileTo(juce::File(*aTargetLocation.file));
	}

	// Moves file or directory into a specified directory, retaining it's same name an extension
	bool FileSystem::MoveInto(const FilePath& aFilePath, const FilePath& aDestinationDirectory)
	{
		if (FileExists(aDestinationDirectory))
			return false; // Can't move into a file

		return MoveTo(aFilePath, aDestinationDirectory.GetRelative(aFilePath.GetFilename()));
	}

	// Copies file or directory into a specified directory, retaining it's same name an extension
	bool FileSystem::CopyInto(const FilePath& aFilePath, const FilePath& aDestinationDirectory)
	{
		if (FileExists(aDestinationDirectory))
			return false; // Can't copy into a file

		return CopyTo(aFilePath, aDestinationDirectory.GetRelative(aFilePath.GetFilename()));
	}

	// Searches directory for files, or other directories matching the wildcard criteria.
	DynamicArray<FilePath> FileSystem::Find(const FilePath& aDirectoryToSearch, const String& aWildCardPattern, bool aSearchRecursively, bool aFindFiles, bool aFindDirectories, bool aIgnoreHiddenFiles)
	{
		if (!DirectoryExists(aDirectoryToSearch))
			return DynamicArray<FilePath>(); // Directory needs to exist, otherwise there's no sense.

		juce::Array<juce::File> result;
		int whatToLookFor = 0 + (aFindFiles ? 2 : 0) + (aFindDirectories ? 1 : 0) + (aIgnoreHiddenFiles ? 4 : 0);
		aDirectoryToSearch.file->findChildFiles(result, whatToLookFor, aSearchRecursively, aWildCardPattern.GetCharPointer());

		DynamicArray<FilePath> returnDynamicArray = DynamicArray<FilePath>();

		for (int32 i = 0; i < result.size(); i++)
		{
			returnDynamicArray.PushBack(FilePath(String(result[i].getFullPathName().getCharPointer())));
		}

		return returnDynamicArray;
	}

	// Returns true if file path has any subdirectories
	bool FileSystem::HasSubdirectories(const FilePath& aFilePath)
	{
		return aFilePath.file->containsSubDirectories();
	}

	// Reads the contents of the file as if it were plain text.
	String FileSystem::LoadFileAsString(const FilePath& aFilePath)
	{
		return String(aFilePath.file->loadFileAsString().getCharPointer());
	}

	// Reads the contents of the file as if it were plain text. Creates a new string at every line break.
	DynamicArray<String> FileSystem::LoadFileAsStringArray(const FilePath& aFilePath)
	{
		juce::StringArray juceDestLines;
		aFilePath.file->readLines(juceDestLines);

		DynamicArray<String> returnDynamicArray = DynamicArray<String>();
		returnDynamicArray.Resize(juceDestLines.size());

		for (int32 i = 0; i < juceDestLines.size(); i++)
		{
			returnDynamicArray[i] = String(juceDestLines[i].getCharPointer());
		}

		return returnDynamicArray;
	}

	// Checks the file byte-by-byte and returns true if both are identical. False if not identical, or couldn't be compared.
	bool FileSystem::CompareFiles(const FilePath& aFilePath, const FilePath& aOtherFile)
	{
		return aFilePath.file->hasIdenticalContentTo(*aOtherFile.file);
	}

	// Will open a finder (mac), explorer (windows), or equivalent window displaying this file path.
	void FileSystem::OpenExplorer(const FilePath& aAtFilePath)
	{
		return aAtFilePath.file->revealToUser();
	}

	// True if the file path is located on a CD or DVD drive
	bool FileSystem::IsOnCDRomDrive(const FilePath& aFilePath)
	{
		return aFilePath.file->isOnCDRomDrive();
	}

	// True is located on a hard disk. False if a network drive, but will still be true for removable hard drives.
	bool FileSystem::IsOnHardDisk(const FilePath& aFilePath)
	{
		return aFilePath.file->isOnHardDisk();
	}

	// Checks if it's on a drive deemed "removable" such as a USB, CD-rom, or network drive.
	const bool FileSystem::IsOnRemovableDrive(const FilePath& aFilePath)
	{
		return aFilePath.file->isOnRemovableDrive();
	}
}
