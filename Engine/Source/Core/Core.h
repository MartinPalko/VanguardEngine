#pragma once

#include "Interfaces/ICore.h"
#include "Core_Common.h"

#include "Config/Config.h"
#include "World.h"
#include "Type.h"
#include "Application/Application.h"
#include "AsyncIO.h"
#include "Log.h"
#include "Profiler.h"
#include "Project.h"
#include "ResourceManager.h"
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
			LOG_MESSAGE(message + String::FromFloat(profileTime.InSeconds() * 1000.0) + "ms", "Quick Profiler");
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
		size_t mainThreadID;

		JobManager* jobManager;
		Profiler* profiler;
		ResourceManager* resourceManager;
		Project* loadedProject;
		ModuleManager* moduleManager;
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
		size_t GetMainThreadID() { return mainThreadID; }
		JobManager* GetJobManager();
		Profiler* GetProfiler();
		ResourceManager* GetResourceManager();

		// Implementation of ICore
		virtual void Initialize(int aArgC, char** aArgV, const char* aProjectName = "") override;
		virtual void Run() override;
		virtual void ShutDown() override;
		virtual void LoadModule(const char* aModuleName) override;

		inline CoreState GetState(){ return state; }

		class Project* GetLoadedProject(){ return loadedProject; }

		void RegisterRenderer(IRenderer* aRenderer);
		void UnregisterRenderer(IRenderer* aRenderer);
		IRenderer* GetPrimaryRenderer();
	};
}

#if VANGUARD_DEBUG
#define ASSERT_MAIN_THREAD \
if (Vanguard::Core::GetInstance()->GetMainThreadID() != Thread::CurrentThreadID()) LOG_EXCEPTION("This code must execute on the main thread!", "Core");
#else
#define ASSERT_MAIN_THREAD
#endif
