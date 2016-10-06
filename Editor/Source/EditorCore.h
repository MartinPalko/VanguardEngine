#pragma once
#include "Editor_Common.h"
#include "Interfaces/IModule.h"

#include <QObject>
#include <QAbstractNativeEventFilter>

class QApplication;

namespace Vanguard
{
	class EditorMainWindow;
	class EditorWorld;
	class EditorEvent;

	struct IEditorEventListener
	{
		virtual void OnEditorEvent(EditorEvent* aEvent) = 0;
	};

	class EditorCore : public QObject, public Core, public QAbstractNativeEventFilter
	{
		Q_OBJECT;

		// Overide QObject
		virtual bool eventFilter(QObject* watched, QEvent* event) override;

		// Implement QAbstractNativeEventFilter
		virtual bool nativeEventFilter(const QByteArray &eventType, void *message, long *result) override;

		QApplication* qApplication;
		EditorMainWindow* mainWindow;
		EditorWorld* editorWorld;
		DynamicArray<IEditorEventListener*> editorEventListeners;
		Entity* selectedEntity;

	public:
		EditorCore(QApplication* aQApplication);

		// Override Core
		virtual void Initialize(int aArgC, char** aArgV, const char* aProjectName) override;
		virtual void Run() override;
		virtual void ShutDown() override;
		virtual void BroadcastEvent(Event* aEvent) override;
		virtual void ProcessEvents(bool aIncludeNativeEvents) override;

	public:
		static EditorCore* GetInstance() { return static_cast<EditorCore*>(Core::GetInstance()); }

		World* GetEditorWorld();

		DynamicArray<World*> GetWorlds();

		void RegisterEventListener(IEditorEventListener* aListener);
		void UnregisterEventListener(IEditorEventListener* aListener);

		void SelectEntity(Entity* aEntity);
		void ClearSelection();
		Entity* GetSelectedEntity();
	};
}