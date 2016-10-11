#include "ComponentInspectorWidget.h"
#include "EditorEvents.h"

#include <QScrollArea>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QLayout>
#include <QPushButton>

namespace Vanguard
{
	ComponentInspectorWidget::ComponentInspectorWidget(Component* aComponent, QWidget* aParent) : ObjectInspectorWidget(aParent)
		, component(aComponent)
	{
		setLayout(new QVBoxLayout());
		layout()->setSpacing(5);
		
		componentTitlebarLayout = new QHBoxLayout();
		layout()->addItem(componentTitlebarLayout);

		componentTitle = new QPushButton(ToQString(aComponent->GetType()->GetTypeName()), this);
		componentTitlebarLayout->addWidget(componentTitle);
		
		CreatePropertyWidgets(aComponent);
	}

}