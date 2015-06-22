#include "Editor_UI_Common.h"

namespace Vanguard_Editor_UI
{
	class EditorApplication : public juce::JUCEApplication
	{
		virtual const juce::String getApplicationName() override { return "Vanguard Editor"; }
		virtual const juce::String getApplicationVersion() override { return "1.0"; }

		EditorMainWindow* editorWindow = nullptr;

		virtual void initialise(const juce::String& commandLineParameters) override
		{
			editorWindow = new EditorMainWindow("Editor Window");
		};
		virtual void shutdown() override
		{
			delete editorWindow; 
		};

	};
}