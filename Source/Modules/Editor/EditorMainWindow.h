#include "Core.h"
#include "juce_gui_basics.h"

namespace Vanguard
{
	class EditorMainWindow : public juce::DocumentWindow
	{
	public:
		EditorMainWindow(String name) : DocumentWindow(name,
			juce::Colours::lightgrey,
			DocumentWindow::allButtons)
		{
			setUsingNativeTitleBar(true);
			//setContentOwned(new juce::MainContentComponent(), true);

			centreWithSize(getWidth(), getHeight());
			setVisible(true);
		}
	};
}