#pragma once
#include <thread>

#include "Qt_EditorUI_Common.h"
#include "IModule.h"

#include <qapplication.h>
#include "qevent.h"
#include "MainWindow.h"

namespace Qt_EditorUI
{
	class Qt_EditorUI : public IModule
	{
		virtual String GetModuleName() { return "Qt_EditorUI"; }
		virtual String GetModuleType() { return "EditorUIRenderer"; }
		
		static QApplication* QtApplication;

		std::thread* stdthread;
		static bool QtThreadRunning;

		virtual void LoadModule()
		{
			Log::Message("Starting Qt Application Thread","Qt");
			stdthread = new std::thread(QTAppThread);
			QtThreadRunning = true;
		}

		static void QTAppThread()
		{
			ApplicationArguments arguments = Application::GetApplicationArguments();
			QtApplication = new QApplication(arguments.argc, arguments.argv);
			
			MainWindow mainWindow;
			mainWindow.show();

			// TODO: Handle error code on return
			QtApplication->exec();

			Log::Message("Qt App has finished, shutting down core", "Qt");
			QtThreadRunning = false;
			Core::GetInstance()->ShutDown(); // Tell core to exit when qt app exits.
		}

		virtual void UnloadModule()
		{
			if (QtThreadRunning)
			{
				QCloseEvent closeEvent;

				//QtApplication->exit(); // Tell Qt app to exit, which should let the app thread exit properly.
				QtApplication->postEvent(QtApplication, &closeEvent);

				// Spin while the Qt app shuts down.
				Log::Message("Waiting for Qt App to exit", "Qt");
				while (QtThreadRunning)
				{
					std::this_thread::yield();
				}
			}
			stdthread->detach();
		}
	};
}