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

	public:
		ObjectInspectorWidget(QWidget* aParent);
	protected:
		DynamicArray<IPropertyWidget*> propertyWidgets;
		
		void CreatePropertyWidgets(WorldObject* aObject, DynamicArray<Vanguard::String> aIgnoreProperties = DynamicArray<Vanguard::String>());
		void UpdatePropertyWidgets();
	};
}