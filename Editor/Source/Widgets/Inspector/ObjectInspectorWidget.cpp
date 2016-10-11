#include "ObjectInspectorWidget.h"
#include "EditorEvents.h"
#include "EditorCore.h"
#include "Widgets/PropertyWidgets/IPropertyWidget.h"

#include <QScrollArea>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QLayout>
#include <QRegExp>

namespace Vanguard
{
	ObjectInspectorWidget::ObjectInspectorWidget(QWidget* aParent) : QWidget(aParent)
	{
	}

	static QString FormatName(QString s)
	{
		s[0] = s.at(0).toUpper();
		return s;
	}

	void ObjectInspectorWidget::CreatePropertyWidgets(WorldObject* aObject, DynamicArray<Vanguard::String> aIgnoreProperties)
	{
		DynamicArray<IPropertyWidgetFactory*> factories = EditorCore::GetInstance()->GetPropertyWidgetFactories();

		for (Vanguard::Property* property : aObject->GetType()->GetProperties())
		{
			if (aIgnoreProperties.Contains(property->GetName()))
				continue;

			// Find appropriate factory
			IPropertyWidgetFactory* foundFactory = nullptr;
			for (IPropertyWidgetFactory* factory : factories)
			{
				if (factory->SupportedProperty(property))
				{
					foundFactory = factory;
					break;
				}
			}

			// Create widget
			if (foundFactory)
			{
				IPropertyWidget* propertyWidget = foundFactory->CreatePropertyWidget(aObject, property);
				auto propertyLayout = new QHBoxLayout(this);
				propertyLayout->setMargin(0);
				propertyLayout->setSpacing(0);

				QString propertyDisplayName = FormatName(ToQString(property->GetName()));

				QLabel* label = new QLabel(propertyDisplayName, this);
				propertyLayout->addWidget(label);
				propertyWidgets.PushBack(propertyWidget);
				propertyWidget->GetWidget()->setParent(this);

				propertyLayout->addWidget(propertyWidget->GetWidget());
				layout()->addItem(propertyLayout);
			}
		}

		updateGeometry();
		UpdatePropertyWidgets();
	}

	void ObjectInspectorWidget::UpdatePropertyWidgets()
	{
		for (IPropertyWidget* propertyWidget : propertyWidgets)
		{
			propertyWidget->Update();
		}
	}

}