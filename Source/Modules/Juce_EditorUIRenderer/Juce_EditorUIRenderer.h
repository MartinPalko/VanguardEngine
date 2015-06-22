#pragma once
#include <thread>

#include "Juce_EditorUIRenderer_Common.h"
#include "IModule.h"

#include "EditorMainWindow.h"
#include "EditorApplication.h"

namespace Juce_EditorUIRenderer
{
	class Juce_EditorUIRenderer : public IModule
	{
		virtual String GetModuleName() { return "Juce_EditorUIRenderer"; }
		virtual String GetModuleType() { return "EditorUIRenderer"; }

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
			Core::GetInstance()->ShutDown(); // Tell core to exit when juce app exits.
			juceAppRunning = false;
		}

		virtual void UnloadModule()
		{
			if (juceAppRunning)
			{
				juce::JUCEApplicationBase::quit(); // Tell juce app to quit, which should let the app thread exit properly.

				// Spin while the juce app shuts down.
				while (juceAppRunning)
				{
					std::this_thread::yield; 
				}
			}
			stdthread->detach();
		}
	};
}