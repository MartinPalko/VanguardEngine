#include <thread>

#include "Editor_UI_Common.h"
#include "IModule.h"

#include "EditorMainWindow.h"
#include "EditorApplication.h"



namespace Vanguard_Editor_UI
{
	class Editor_UI : public IModule
	{
		virtual String GetModuleName() { return "Editor_UI"; }
		virtual String GetModuleType() { return "Editor"; }

		std::thread* stdthread;

		static bool juceAppRunning;
		
		virtual void LoadModule()
		{
			// Start a new thread, which will run the juce app.
			stdthread = new std::thread(JuceAppThread);
			juceAppRunning = true;
		}

		static void JuceAppThread()
		{			
			juce::JUCEApplicationBase::createInstance = []()->juce::JUCEApplicationBase*{return new EditorApplication(); };
			juce::JUCEApplicationBase::main();
			Core::GetInstance()->Exit(); // Tell core to exit when juce app exits.
			juceAppRunning = false;
		}

		virtual void UnloadModule()
		{
			juce::JUCEApplicationBase::quit(); // Tell juce app to quit, which should let the app thread exit properly.

			// Spin while the juce app shuts down.
			while (juceAppRunning){}

			stdthread->detach();
		}
	};
}