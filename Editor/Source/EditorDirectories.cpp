#include "EditorDirectories.h"


namespace Vanguard
{
	namespace EditorDirectories
	{
		FilePath GetEditorDirectory()
		{
			return Directories::GetVanguardRootDirectory().GetRelative("Editor");
		}

		FilePath GetStyleDirectory()
		{
			return GetEditorDirectory().GetRelative("Style");
		}
	}
}