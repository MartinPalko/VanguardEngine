#pragma once
#include "Editor_Common.h"
#include "ObjectInspectorWidget.h"

class QScrollArea;
class QLineEdit;
class QCheckBox;
class QVBoxLayout;
class QHBoxLayout;
class QPushButton;

namespace Vanguard
{
	class ComponentInspectorWidget : public ObjectInspectorWidget
	{
		Q_OBJECT;

	public:
		ComponentInspectorWidget(Component* aComponent, QWidget* aParent);

	private:
		Component* component;

		QHBoxLayout* componentTitlebarLayout;

		QPushButton* componentTitle;

	};
}