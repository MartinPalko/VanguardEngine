#include "EditorCore.h"

#include "EditorWorld.h"
#include "Widgets/EditorMainWindow.h"
#include "Widgets/PropertyWidgets/IPropertyWidget.h"
#include "Widgets/PropertyWidgets/BoolWidget.h"
#include "Widgets/PropertyWidgets/FloatWidget.h"
#include "Widgets/PropertyWidgets/IntWidget.h"
#include "Widgets/PropertyWidgets/QuaternionWidget.h"
#include "Widgets/PropertyWidgets/Vector3Widget.h"
#include "EditorEvents.h"


#include <QApplication>
#include <QMainWindow>

namespace Vanguard
{
	bool EditorCore::eventFilter(QObject* watched, QEvent* event)
	{
		if (watched == mainWindow && event->type() == QEvent::Close && GetState() == CoreState::Running)
		{
			Quit();
			return true;
		}
		return false;
	}

	bool EditorCore::nativeEventFilter(const QByteArray& eventType, void* message, long* result)
	{
		// Forward native events processed by Qt to the Core Application
		Application::DispatchNativeEvent((NativeEvent)message);
		return false;
	}

	EditorCore::EditorCore(QApplication* aQApplication) : Core()
		, qApplication(aQApplication)
		, selectedEntity(nullptr)
	{}

	void EditorCore::Initialize(int aArgC, char** aArgV, const char* aProjectName)
	{
		// Register built-in property widgets
		RegisterPropertyWidgetFactory(new BoolWidgetFactory());
		RegisterPropertyWidgetFactory(new FloatWidgetFactory());
		//RegisterPropertyWidgetFactory(new IntWidgetFactory());
		//RegisterPropertyWidgetFactory(new QuaternionWidgetFactory());
		RegisterPropertyWidgetFactory(new Vector3WidgetFactory());

		Core::Initialize(aArgC, aArgV, aProjectName);

		editorWorld = new EditorWorld();
		AddWorld(editorWorld);

		mainWindow = new EditorMainWindow();
		mainWindow->installEventFilter(this);

		QApplication::instance()->installNativeEventFilter(this);
		QApplication::instance()->installEventFilter(this);
	}

	void EditorCore::Run()
	{
		mainWindow->show();

		Core::Run();
	}

	void EditorCore::ShutDown()
	{
		mainWindow->removeEventFilter(this);

		if (mainWindow->isVisible())
			mainWindow->close();

		delete mainWindow;
		mainWindow = nullptr;

		Core::ShutDown();
		
		QApplication::instance()->removeNativeEventFilter(this);
		QApplication::instance()->removeEventFilter(this);
	}

	void EditorCore::BroadcastEvent(Event* aEvent)
	{
		ASSERT_MAIN_THREAD;
		DEBUG_LOG("Event: " + aEvent->GetType()->GetTypeName());

		if(EditorEvent* editorEvent = Type::SafeCast<EditorEvent>(aEvent))
		{
			for (auto listener : editorEventListeners)
			{
				listener->OnEditorEvent(editorEvent);
			}
		}
		else
		{
			Core::BroadcastEvent(aEvent);
		}
	}

	void EditorCore::ProcessEvents(bool aIncludeNativeEvents)
	{
		if (aIncludeNativeEvents && qApplication->hasPendingEvents())
			qApplication->processEvents();	

		Core::ProcessEvents(aIncludeNativeEvents);
	}

	World* EditorCore::GetEditorWorld()
	{
		return editorWorld;
	}

	DynamicArray<World*> EditorCore::GetWorlds()
	{
		return worlds;
	}

	void EditorCore::RegisterEventListener(IEditorEventListener* aListener)
	{
		editorEventListeners.PushBack(aListener);
	}

	void EditorCore::UnregisterEventListener(IEditorEventListener* aListener)
	{
		editorEventListeners.Remove(aListener);
	}

	void EditorCore::SelectEntity(Entity* aEntity)
	{
		if (selectedEntity)
			ClearSelection();

		selectedEntity = aEntity;
		PostEvent(new EntitySelectedEvent(selectedEntity));
	}

	void EditorCore::ClearSelection()
	{
		PostEvent(new EntityUnselectedEvent(selectedEntity));
		selectedEntity = nullptr;
	}

	Entity* EditorCore::GetSelectedEntity()
	{
		return selectedEntity;
	}

	void EditorCore::RegisterPropertyWidgetFactory(IPropertyWidgetFactory* aFactory)
	{
		propertyWidgetFactories.PushBack(aFactory);
	}

	void EditorCore::UnregisterPropertyWidgetFactory(IPropertyWidgetFactory* aFactory)
	{
		propertyWidgetFactories.Remove(aFactory);
	}

	DynamicArray<IPropertyWidgetFactory*> EditorCore::GetPropertyWidgetFactories()
	{
		return propertyWidgetFactories;
	}
}
