#pragma once
#include "Editor_Common.h"
#include "EditorCore.h"

#include <QDockWidget>

class QScrollArea;
class QLineEdit;
class QCheckBox;
class QVBoxLayout;
class QTimer;

namespace Vanguard
{
	class EntityInspectorWidget;
	class ComponentInspectorWidget;

	class Inspector : public QDockWidget, public IEditorEventListener
	{
		Q_OBJECT;

		QScrollArea* scrollArea;
		QWidget* mainWidget;
		QVBoxLayout* mainLayout;
		QTimer* updateTimer;

		EntityInspectorWidget* entityInspector;
		DynamicArray<ComponentInspectorWidget*> componentInspectors;

		// Implement IEditorEventListener
		virtual void OnEditorEvent(EditorEvent* aEvent) override;

	public:
		Inspector(QWidget* parent);
		~Inspector();
		
	public slots:
		void SetEntity(Entity* aEntity);
		virtual void Update();
	};
}