#pragma once
#include "Editor_Common.h"

#include <QDockWidget>

class QScrollArea;
class QLineEdit;
class QCheckBox;
class QVBoxLayout;

struct IPropertyWidget;

namespace Vanguard
{
	class ObjectInspectorWidget : public QWidget
	{
		Q_OBJECT;

	protected:
		DynamicArray<IPropertyWidget*> propertyWidgets;

	public:
		ObjectInspectorWidget(QWidget* aParent);
		void CreatePropertyWidgets(WorldObject* aObject, DynamicArray<Vanguard::String> aIgnoreProperties = DynamicArray<Vanguard::String>());
		virtual void UpdatePropertyWidgets();
	};
}