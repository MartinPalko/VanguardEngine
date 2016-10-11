#pragma once
#include "Editor_Common.h"
#include "ObjectInspectorWidget.h"

class QScrollArea;
class QLineEdit;
class QCheckBox;
class QVBoxLayout;

struct IPropertyWidget;

namespace Vanguard
{
	class EntityInspectorWidget : public ObjectInspectorWidget
	{
		Q_OBJECT;

	public:
		EntityInspectorWidget(Entity* aEntity, QWidget* aParent);

		// Override ObjectInspectorWidget
		virtual void UpdatePropertyWidgets();

	private:
		Entity* entity;

		QLineEdit* entityNameLineEdit;
		QCheckBox* entityEnabledCheckbox;
		DynamicArray<IPropertyWidget*> propertyWidgets;

	private slots:
		void OnNameLineEditChanged();
		void OnEnabledCheckboxChanged();
	};
}