#include "Juce_EditorUIRenderer_Common.h"

namespace Juce_EditorUIRenderer
{
	class EditorMainWindowMenuBarModel : public juce::MenuBarModel
	{
	public:
		virtual juce::StringArray getMenuBarNames() override
		{
			juce::StringArray menuItems;
			menuItems.add("File");
			menuItems.add("Edit");
			menuItems.add("View");
			menuItems.add("Windows");
			return menuItems;
		}

		virtual juce::PopupMenu getMenuForIndex(int topLevelMenuIndex, const juce::String& menuName) override
		{
			return juce::PopupMenu();
		}
		virtual void menuItemSelected(int menuItemID, int topLevelMenuIndex) override
		{

		}
	};
}