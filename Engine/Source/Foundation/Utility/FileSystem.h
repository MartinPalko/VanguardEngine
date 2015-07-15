#pragma once
#include "Foundation_Common.h"
#include "Types/VanguardTypes.h"

namespace Vanguard
{
	struct FOUNDATION_API FileSystem
	{
		static FilePath GetSystemTempDirectory();

		static FilePath GetSystemProgramsDirectory();

		static void SetCurrentWorkingDir(const FilePath& aFilePath);

		static FilePath GetCurrentWorkingDir();

		static FilePath GetHostApplicationPath();

		// If a file or directory already exists at this path, this will return a path that does not exist by adding a unique suffix to the file (eg _1 or _2, until a filepath is found that does not already exist.
		static FilePath MakeUniqueFileName(const FilePath& aFilePath);

		// Returns the time when the file was last modified. Returns invalid time if file does not exist.
		static Time GetTimeLastModified(const FilePath& aFilePath);

		// Returns the time when the file was created. Returns invalid time if file does not exist.
		static Time GetTimeCreated(const FilePath& aFilePath);

		// True if filepath exists as either a file or directory.
		static bool Exists(const FilePath& aFilePath);

		// True if path points to a file that exists. False if it points to a directory, or a file that doesn't exist.
		static bool FileExists(const FilePath& aFilePath);

		// True if path points to a directory that exists. False if it points to a file, or directory that doesn't exist.
		static bool DirectoryExists(const FilePath& aFilePath);

		// Gets filesize in bytes. Returns 0 if it doesn't exist.
		static uint64 GetFileSizeInBytes(const FilePath& aFilePath);

		// Gets a human readable string of the file size, for example "100 bytes", "2KB", or "2 MB".
		static String GetHumanReadableSize(const FilePath& aFilePath);

		// Gets a human readable string of the file size, for example "100 bytes", "2KB", or "2 MB".
		static String GetHumanReadableSizeFromBytes(uint64 aNumBytes);

		// Checks to see if we have write access to a file or directory.
		static bool HasWriteAccess(const FilePath& aFilePath);

		// Checks to see if we have write access to a file or directory.
		static bool IsHidden(const FilePath& aFilePath);

		// Set file's readonly status, and optionally apply recursively to children. Returns false if not able to modify readonly property.
		static bool SetReadOnly(const FilePath& aFilePath, bool aShouldBeReadOnly, bool aApplyRecursively);

		// Create an empty file on the disk if it doesn't already exist. Returns the result (error message if failed)
		static String CreateFile(const FilePath& aFilePath);

		// Write a plain text file containing the current string. If the file exists, it will be completely overwritten.
		static bool WriteToFile(const FilePath& aFilePath, const String& aStringContents);

		// Append text to an existing file. If the file does not exist, will create it and fill it with the supplied text.
		static bool AppendToFile(const FilePath& aFilePath, const String& aStringContents);

		static String ReadFileAsText(const FilePath& aFilePath);

		static DynamicArray<String> ReadFileLinesAsText(const FilePath& aFilePath);

		// Create an empty directory on the disk if it doesn't already exist. Returns the result (error message if failed)
		static String CreateDirectory(const FilePath& aFilePath);

		// Deletes a file or directory from the disk. Returns false if delete fails.
		static bool Delete(const FilePath& aFilePath);

		// Alternative to delete that moves the file to the trash (or recycle bin on win) and allows the user to recover it if needed. Returns false if file can't be moved.
		static bool MoveToTrash(const FilePath& aFilePath);

		// Moves file or directory to a specific path. Note: This should not be the directory you want to put it in but the actual filename.
		static bool MoveTo(const FilePath& aFilePath, const FilePath& aDestinationPath);

		// Copies file or directory to a specific path. Note: This should not be the directory you want to put it in but the actual filename.
		static bool CopyTo(const FilePath& aFilePath, const FilePath& aTargetLocation);

		// Moves file or directory into a specified directory, retaining it's same name an extension
		static bool MoveInto(const FilePath& aFilePath, const FilePath& aDestinationDirectory);

		// Copies file or directory into a specified directory, retaining it's same name an extension
		static bool CopyInto(const FilePath& aFilePath, const FilePath& aDestinationDirectory);

		// Searches directory for files, or other directories matching the wildcard criteria.
		static DynamicArray<FilePath> Find(const FilePath& aDirectoryToSearch, const String& aWildCardPattern = "*", bool aSearchRecursively = true, bool aFindFiles = true, bool aFindDirectories = false, bool aIgnoreHiddenFiles = false);

		// Returns true if file path has any subdirectories
		static bool HasSubdirectories(const FilePath& aFilePath);

		// Reads the contents of the file as if it were plain text.
		static String LoadFileAsString(const FilePath& aFilePath);

		// Reads the contents of the file as if it were plain text. Creates a new string at every line break.
		static DynamicArray<String> LoadFileAsStringArray(const FilePath& aFilePath);

		// Checks the file byte-by-byte and returns true if both are identical. False if not identical, or couldn't be compared.
		static bool CompareFiles(const FilePath& aFilePath, const FilePath& aOtherFile);

		// Will open a finder (mac), explorer (windows), or equivalent window displaying this file path.
		static void OpenExplorer(const FilePath& aAtFilePath);

		// True if the file path is located on a CD or DVD drive
		static bool IsOnCDRomDrive(const FilePath& aFilePath);

		// True is located on a hard disk. False if a network drive, but will still be true for removable hard drives.
		static bool IsOnHardDisk(const FilePath& aFilePath);

		// Checks if it's on a drive deemed "removable" such as a USB, CD-rom, or network drive.
		static const bool IsOnRemovableDrive(const FilePath& aFilePath);

	};
}
