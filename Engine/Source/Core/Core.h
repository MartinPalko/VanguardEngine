#pragma once

#include "Interfaces/ICore.h"
#include "Core_Common.h"

#include "Config/Config.h"
#include "World.h"
#include "Type.h"
#include "Application/Application.h"
#include "AsyncIO.h"
#include "Log.h"
#include "Project.h"
#include "Directories.h"

#include "Entity/Actor.h"
#include "Entity/Component.h"
#include "Entity/Entity.h"
#include "Entity/Transform.h"

#include "Jobs/JobManager.h"
#include "Jobs/Job.h"
#include "Jobs/Frame.h"

#include "Renderer/Camera.h"
#include "Renderer/IRenderer.h"
#include "Renderer/RenderView.h"

namespace Vanguard
{
	class QuickProfiler
	{
		Timespan startTime;
		String message;
		bool ended;

	public:
		QuickProfiler(String aMessage)
		{
			startTime = Timespan::GetElapsedSystemTime();
			message = aMessage;
			ended = false;
		}

		void End()
		{
			Timespan endTime = Timespan::GetElapsedSystemTime();
			Timespan profileTime = endTime - startTime;
			Log::Message(message + String::FromFloat(profileTime.InSeconds() * 1000.0) + "ms");
			ended = true;
		}

		~QuickProfiler()
		{
			if (!ended)
				End();
		}
	};

	enum class CoreState : uint8
	{
		NotInitialized = 0,
		Initializing,
		Initialized,
		Running,
		PendingShutdown,
		StartingShutdown,
		ShuttingDown,
		ShutDown
	};

	class ModuleManager;

	class CORE_API Core : public ICore
	{
	private:
		static BooleanConfigVar clearTempDirectoryOnShutdown;
		static BooleanConfigVar showConsoleOnStart;

		static Core* instance;
		String mainThreadID;

		JobManager* jobManager;

		class Project* loadedProject;
		class ModuleManager* moduleManager;
		DynamicArray<World*> worlds;
		class DynamicArray<IRenderer*> renderers;
		IRenderer* primaryRenderer;

		CoreState state = CoreState::NotInitialized;

		void AddWorld(World* world);
		void DestroyWorld(World* aWorld);

	public:
		Core();
		virtual ~Core() {};

		static Core* GetInstance();
		String GetMainThreadID() { return mainThreadID; }
		JobManager* GetJobManager();

		// Implementation of ICore
		void Initialize(int aArgC, char** aArgV, const char* aProjectName = "") override;
		void Run() override;
		void ShutDown() override;
		void LoadModule(const char* aModuleName) override;

		inline CoreState GetState(){ return state; }

		class Project* GetLoadedProject(){ return loadedProject; }

		void RegisterRenderer(IRenderer* aRenderer);
		void UnregisterRenderer(IRenderer* aRenderer);
		IRenderer* GetPrimaryRenderer();
	};
}

#if VANGUARD_DEBUG
#define ASSERT_MAIN_THREAD \
if (Vanguard::Core::GetInstance()->GetMainThreadID() != Thread::CurrentThreadID()) Log::Exception("This code must execute on the main thread!");
#else
#define ASSERT_MAIN_THREAD
#endif
