#pragma once
#include "Editor_Common.h"
#include "EditorCore.h"

#include <QDockWidget>

class QScrollArea;
class QLineEdit;
class QCheckBox;
class QVBoxLayout;

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

		EntityInspectorWidget* entityInspector;
		DynamicArray<ComponentInspectorWidget*> componentInspectors;

		// Implement IEditorEventListener
		virtual void OnEditorEvent(EditorEvent* aEvent) override;
	public:
		Inspector(QWidget* parent);
		~Inspector();
		
		void SetEntity(Entity* aEntity);
	};
}