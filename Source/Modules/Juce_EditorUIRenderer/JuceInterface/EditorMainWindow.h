#include "Juce_EditorUIRenderer_Common.h"
#include "EditorMainWindowMenuBarModel.h"

namespace Juce_EditorUIRenderer
{
	class EditorMainWindow : public juce::DocumentWindow
	{
		EditorMainWindowMenuBarModel* menuBarModel;


	public:
		EditorMainWindow(String name) : DocumentWindow(name,
			juce::Colours::darkgrey,
			DocumentWindow::allButtons)
		{
			setUsingNativeTitleBar(true);


			menuBarModel = new EditorMainWindowMenuBarModel();
			setMenuBar(menuBarModel);

			centreWithSize( 1024, 768);
			setVisible(true);
		}

		~EditorMainWindow()
		{
			setMenuBar(nullptr);
			delete menuBarModel;
		}

		virtual void closeButtonPressed() override
		{
			juce::JUCEApplicationBase::quit();
		}
	};
}