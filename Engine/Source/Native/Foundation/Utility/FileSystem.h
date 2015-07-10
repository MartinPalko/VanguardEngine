#pragma once
#include "VanguardTypes.h"
#include "juce_core.h"
#include "FilePath.h"

namespace Vanguard
{
	struct FileSystem
	{
		// Gets a directory inside of the users temp directory, useful for temporary files.
		static FilePath GetEngineTempDirectory()
		{
			FilePath systemTempDirectory = juce::File::getSpecialLocation(juce::File::tempDirectory);
			FilePath engineTempDirectory = systemTempDirectory.GetRelative("Vanguard");

			// Create the directory if it doesn't already exist.
			if (!DirectoryExists(engineTempDirectory))
				CreateDirectory(engineTempDirectory);

			return engineTempDirectory;
		}

		static void ClearEngineTempDirectory()
		{
			Delete(GetEngineTempDirectory());
		}

		static FilePath GetProjectsDirectory()
		{
			return GetEngineRootDirectory().GetRelative("Projects");
		}

		static FilePath GetEngineBinDirectory()
		{
			FilePath engineExecutable = juce::File::getSpecialLocation(juce::File::hostApplicationPath);
			return engineExecutable.GetParentDirectory();
		}

		static FilePath GetEngineModuleDirectory()
		{
			return GetEngineBinDirectory();
		}

		static FilePath GetLogDirectory()
		{
			FilePath logDir = GetEngineRootDirectory().GetRelative("Logs");
			if (!DirectoryExists(logDir))
				CreateDirectory(logDir);
			return logDir;
		}

		static FilePath GetSystemProgramsDirectory()
		{
			return juce::File::getSpecialLocation(juce::File::globalApplicationsDirectory);
		}

		static void SetCurrentWorkingDir(const FilePath& aFilePath)
		{
			aFilePath.file.setAsCurrentWorkingDirectory();
		}

		static FilePath GetCurrentWorkingDir()
		{
			return juce::File::getCurrentWorkingDirectory();
		}

		static FilePath GetEngineRootDirectory()
		{
			return GetEngineBinDirectory().GetParentDirectory();
		}

		static FilePath GetEngineConfigDirectory()
		{
			FilePath configDir = GetEngineRootDirectory().GetRelative("Config");
			if (!DirectoryExists(configDir))
				CreateDirectory(configDir);
			return configDir;
		}

		// If a file or directory already exists at this path, this will return a path that does not exist by adding a unique suffix to the file (eg _1 or _2, until a filepath is found that does not already exist.
		static FilePath MakeUniqueFileName(const FilePath& aFilePath)
		{
			FilePath newPath = aFilePath;

			int32 uniqueSuffix = 1;
			while (FileExists(newPath))
			{
				uniqueSuffix++;
				String newFilename = aFilePath.GetFilenameWithoutExtension() + "_" + String::FromInt32(uniqueSuffix) + aFilePath.GetFileExtension();
				newPath = aFilePath.GetParentDirectory().GetRelative(newFilename);
			}
			return newPath;
		}

		// Returns the time when the file was last modified. Returns invalid time if file does not exist.
		static Time GetTimeLastModified(const FilePath& aFilePath)
		{
			return aFilePath.file.getLastModificationTime();
		}

		// Returns the time when the file was created. Returns invalid time if file does not exist.
		static Time GetTimeCreated(const FilePath& aFilePath)
		{
			return aFilePath.file.getCreationTime();
		}

		// True if filepath exists as either a file or directory.
		static bool Exists(const FilePath& aFilePath)
		{
			return FileExists(aFilePath) || DirectoryExists(aFilePath);
		}

		// True if path points to a file that exists. False if it points to a directory, or a file that doesn't exist.
		static bool FileExists(const FilePath& aFilePath)
		{
			return aFilePath.file.existsAsFile();
		}

		// True if path points to a directory that exists. False if it points to a file, or directory that doesn't exist.
		static bool DirectoryExists(const FilePath& aFilePath)
		{
			return aFilePath.file.isDirectory();
		}

		// Gets filesize in bytes. Returns 0 if it doesn't exist.
		static uint64 GetFileSizeInBytes(const FilePath& aFilePath)
		{
			return aFilePath.file.getSize();
		}

		// Gets a human readable string of the file size, for example "100 bytes", "2KB", or "2 MB".
		static String GetHumanReadableSize(const FilePath& aFilePath)
		{
			return GetHumanReadableSizeFromBytes(GetFileSizeInBytes(aFilePath));
		}

		// Gets a human readable string of the file size, for example "100 bytes", "2KB", or "2 MB".
		static String GetHumanReadableSizeFromBytes(uint64 aNumBytes)
		{
			return juce::File::descriptionOfSizeInBytes(aNumBytes);
		}

		// Checks to see if we have write access to a file or directory.
		static bool HasWriteAccess(const FilePath& aFilePath)
		{
			return aFilePath.file.hasWriteAccess();
		}

		// Checks to see if we have write access to a file or directory.
		static bool IsHidden(const FilePath& aFilePath)
		{
			return aFilePath.file.isHidden();
		}

		// Set file's readonly status, and optionally apply recursively to children. Returns false if not able to modify readonly property.
		static bool SetReadOnly(const FilePath& aFilePath, bool aShouldBeReadOnly, bool aApplyRecursively)
		{
			return aFilePath.file.setReadOnly(aShouldBeReadOnly, aApplyRecursively);
		}

		// Create an empty file on the disk if it doesn't already exist. Returns the result (error message if failed)
		static String CreateFile(const FilePath& aFilePath)
		{
			return aFilePath.file.create().getErrorMessage();
		}

		// Write a plain text file containing the current string. If the file exists, it will be completely overwritten.
		static bool WriteToFile(const FilePath& aFilePath, const String& aStringContents)
		{
			if (FileExists(aFilePath))
				if (!Delete(aFilePath))
					return false;

			if (!(CreateFile(aFilePath) == ""))
				return false;

			juce::FileOutputStream* outStream = aFilePath.file.createOutputStream();
			outStream->setPosition(0);
			outStream->writeText(aStringContents, false, false);
			delete outStream;
			return true;
		}

		// Append text to an existing file. If the file does not exist, will create it and fill it with the supplied text.
		static bool AppendToFile(const FilePath& aFilePath, const String& aStringContents)
		{
			if (!FileExists(aFilePath))
				return WriteToFile(aFilePath, aStringContents);
			
			if (!HasWriteAccess(aFilePath))
				return false;

			juce::FileOutputStream* outStream = aFilePath.file.createOutputStream();
			outStream->writeText(aStringContents, false, false);
			delete outStream;
			return true;
		}

		static String ReadFileAsText(const FilePath& aFilePath)
		{
			if (!FileExists(aFilePath))
				return "";

			juce::FileInputStream* inStream = aFilePath.file.createInputStream();
			String fileText = inStream->readEntireStreamAsString();
			delete inStream;
			return fileText;
		}

		static DynamicArray<String> ReadFileLinesAsText(const FilePath& aFilePath)
		{
			if (!FileExists(aFilePath))
				return DynamicArray <String>();

			DynamicArray<String> fileLines = DynamicArray <String>();

			juce::FileInputStream* inStream = aFilePath.file.createInputStream();
			inStream->setPosition(0);

			while (!inStream->isExhausted())
			{
				fileLines.PushBack(inStream->readNextLine());
			}

			delete inStream;
			return fileLines;
		}

		// Create an empty directory on the disk if it doesn't already exist. Returns the result (error message if failed)
		static String CreateDirectory(const FilePath& aFilePath)
		{
			return aFilePath.file.createDirectory().getErrorMessage();
		}

		// Deletes a file or directory from the disk. Returns false if delete fails.
		static bool Delete(const FilePath& aFilePath)
		{
			if (FileExists(aFilePath))
				return aFilePath.file.deleteFile();
			else
				return aFilePath.file.deleteRecursively();
		}

		// Alternative to delete that moves the file to the trash (or recycle bin on win) and allows the user to recover it if needed. Returns false if file can't be moved.
		static bool MoveToTrash(const FilePath& aFilePath)
		{
			return aFilePath.file.moveToTrash();
		}

		// Moves file or directory to a specific path. Note: This should not be the directory you want to put it in but the actual filename.
		static bool MoveTo(const FilePath& aFilePath, const FilePath& aDestinationPath)
		{
			if (aFilePath.file.isDirectory())
			{
				// Juce doesn't contain a move directory, so copy it, and then delete the old one.
				if (CopyTo(aFilePath, aDestinationPath))
					return Delete(aFilePath);
				else return false;
			}
			else
				return aFilePath.file.moveFileTo(aDestinationPath);
		}

		// Copies file or directory to a specific path. Note: This should not be the directory you want to put it in but the actual filename.
		static bool CopyTo(const FilePath& aFilePath, const FilePath& aTargetLocation)
		{
			if (aFilePath.file.isDirectory())
				return aFilePath.file.copyDirectoryTo(aTargetLocation);
			else
				return aFilePath.file.copyFileTo(aTargetLocation);
		}

		// Moves file or directory into a specified directory, retaining it's same name an extension
		static bool MoveInto(const FilePath& aFilePath, const FilePath& aDestinationDirectory)
		{
			if (FileExists(aDestinationDirectory))
				return false; // Can't move into a file

			return MoveTo(aFilePath, aDestinationDirectory.GetRelative(aFilePath.GetFilename()));
		}

		// Copies file or directory into a specified directory, retaining it's same name an extension
		static bool CopyInto(const FilePath& aFilePath, const FilePath& aDestinationDirectory)
		{
			if (FileExists(aDestinationDirectory))
				return false; // Can't copy into a file

			return CopyTo(aFilePath, aDestinationDirectory.GetRelative(aFilePath.GetFilename()));
		}

		// Searches directory for files, or other directories matching the wildcard criteria.
		static DynamicArray<FilePath> Find(const FilePath& aDirectoryToSearch, const String& aWildCardPattern = "*", bool aSearchRecursively = true, bool aFindFiles = true, bool aFindDirectories = false, bool aIgnoreHiddenFiles = false)
		{
			if (!DirectoryExists(aDirectoryToSearch))
				return DynamicArray<FilePath>(); // Directory needs to exist, otherwise there's no sense.

			juce::Array<juce::File> result;
			int whatToLookFor = 0 + (aFindFiles ? 2 : 0) + (aFindDirectories ? 1 : 0) + (aIgnoreHiddenFiles ? 4 : 0);
			aDirectoryToSearch.file.findChildFiles(result, whatToLookFor, aSearchRecursively, aWildCardPattern);

			DynamicArray<FilePath> returnDynamicArray = DynamicArray<FilePath>();

			for (int32 i = 0; i < result.size(); i++)
			{
				returnDynamicArray.PushBack(result[i]);
			}

			return returnDynamicArray;
		}

		// Returns true if file path has any subdirectories
		static bool HasSubdirectories(const FilePath& aFilePath)
		{
			return aFilePath.file.containsSubDirectories();
		}

		// Reads the contents of the file as if it were plain text.
		static String LoadFileAsString(const FilePath& aFilePath)
		{
			return aFilePath.file.loadFileAsString();
		}

		// Reads the contents of the file as if it were plain text. Creates a new string at every line break.
		static DynamicArray<String> LoadFileAsStringArray(const FilePath& aFilePath)
		{
			juce::StringArray juceDestLines;
			aFilePath.file.readLines(juceDestLines);

			DynamicArray<String> returnDynamicArray = DynamicArray<String>();
			returnDynamicArray.Resize(juceDestLines.size());

			for (int32 i = 0; i < juceDestLines.size(); i++)
			{
				returnDynamicArray[i] = juceDestLines[i];
			}

			return returnDynamicArray;
		}

		// Checks the file byte-by-byte and returns true if both are identical. False if not identical, or couldn't be compared.
		static bool CompareFiles(const FilePath& aFilePath, const FilePath& aOtherFile)
		{
			return aFilePath.file.hasIdenticalContentTo(aOtherFile);
		}

		// Will open a finder (mac), explorer (windows), or equivalent window displaying this file path.
		static void OpenExplorer(const FilePath& aAtFilePath)
		{
			return aAtFilePath.file.revealToUser();
		}

		// True if the file path is located on a CD or DVD drive
		static bool IsOnCDRomDrive(const FilePath& aFilePath)
		{
			return aFilePath.file.isOnCDRomDrive();
		}

		// True is located on a hard disk. False if a network drive, but will still be true for removable hard drives.
		static bool IsOnHardDisk(const FilePath& aFilePath)
		{
			return aFilePath.file.isOnHardDisk();
		}

		// Checks if it's on a drive deemed "removable" such as a USB, CD-rom, or network drive.
		static const bool IsOnRemovableDrive(const FilePath& aFilePath)
		{
			return aFilePath.file.isOnRemovableDrive();
		}

	};
}
