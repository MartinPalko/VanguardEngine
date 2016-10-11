#include "BoolWidget.h"

#include <QHBoxLayout>
#include <QCheckBox>

BoolWidget::BoolWidget(void* aInstance, Vanguard::Property* aProperty)
	: instance (aInstance)
	, property (aProperty)
{
	setLayout(new QHBoxLayout());
	layout()->setMargin(0);

	checkBox = new QCheckBox(this);
	connect(checkBox, &QCheckBox::stateChanged, this, &BoolWidget::OnCheckboxChanged);
	layout()->addWidget(checkBox);
}

QWidget* BoolWidget::GetWidget()
{
	return this;
}

void BoolWidget::Update()
{
	bool propertyValue = property->Get<bool>(instance);
	checkBox->setChecked(propertyValue);
}

void BoolWidget::OnCheckboxChanged()
{
	bool propertyValue = property->Get<bool>(instance);

	if (checkBox->isChecked() != propertyValue)
	{
		property->Set<bool>(propertyValue, instance);
	}
}

bool BoolWidgetFactory::SupportedProperty(Vanguard::Property* aProperty)
{
	return aProperty->Is<bool>();
}

IPropertyWidget* BoolWidgetFactory::CreatePropertyWidget(void* aInstance, Vanguard::Property* aProperty)
{
	return SupportedProperty(aProperty) ? new BoolWidget(aInstance, aProperty) : nullptr;
}