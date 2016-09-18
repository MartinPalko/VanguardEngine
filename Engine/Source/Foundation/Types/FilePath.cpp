#include "juce_core.h"
#include "FilePath.h"
#include "VanguardString.h"

namespace Vanguard
{
	// ----------- Constructors -----------

	FilePath::FilePath(const String& aAbsolutePath)
	{
		file = new juce::File(aAbsolutePath.GetCharPointer());
	}

	FilePath::FilePath()
	{
		file = new juce::File("");
	}

	FilePath::FilePath(const FilePath& aOther)
	{
		file = new juce::File(aOther.file->getFullPathName().getCharPointer());
	}

	FilePath& FilePath::operator = (const FilePath& aOther)
	{
		*file = juce::File(aOther.file->getFullPathName().getCharPointer());
		return *this;
	}

	FilePath& FilePath::operator = (FilePath&& aOther)
	{
		if (this != &aOther)
		{
			delete file;
			file = aOther.file;
			aOther.file = nullptr;
		}
		return *this;
	}

	FilePath::~FilePath()
	{
		delete file;
	}

	// ----------- Methods -----------

	FilePath::operator bool() const
	{
		return file->getFullPathName() != "";
	}

	const String FilePath::GetFullPathName() const
	{
		return String(file->getFullPathName().getCharPointer());
	}

	String FilePath::GetFilename() const
	{
		return String(file->getFileName().getCharPointer());
	}

	String FilePath::GetFilenameWithoutExtension() const
	{
		return String(file->getFileNameWithoutExtension().getCharPointer());
	}

	String FilePath::GetFileExtension() const
	{
		return String(file->getFileExtension().getCharPointer());
	}

	bool FilePath::HasFileExtension(const String& aExtension) const
	{
		return file->hasFileExtension(aExtension.GetCharPointer());
	}

	FilePath FilePath::GetParentDirectory() const
	{
		return String(file->getParentDirectory().getFullPathName().getCharPointer());
	}

	FilePath FilePath::GetRelative(const String& aRelativePath) const
	{
		return String(file->getChildFile(aRelativePath.GetCharPointer()).getFullPathName().getCharPointer());
	}

	FilePath FilePath::Append(const String& aRelativePath) const
	{
		return FilePath(GetFullPathName() + aRelativePath);
	}
}