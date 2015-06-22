#include "Editor_UI_Common.h"

namespace Vanguard_Editor_UI
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

			centreWithSize(300, 300);
			setVisible(true);
		}

		virtual void closeButtonPressed() override
		{
			juce::JUCEApplicationBase::quit();
		}
	};
}