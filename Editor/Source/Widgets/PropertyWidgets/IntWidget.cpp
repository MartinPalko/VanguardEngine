#include "IntWidget.h"

#include <QHBoxLayout>
#include <QSpinBox>

IntWidget::IntWidget(void* aInstance, Vanguard::Property* aProperty)
	: instance (aInstance)
	, property (aProperty)
{
	setLayout(new QHBoxLayout());
	layout()->setMargin(0);

	spinBox = new QSpinBox(this);
	spinBox->setRange(Vanguard::MinInt32(), Vanguard::MaxInt32());
	connect(spinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &IntWidget::OnSpinBoxChanged);
	layout()->addWidget(spinBox);
}

QWidget* IntWidget::GetWidget()
{
	return this;
}

void IntWidget::Update()
{
	if (!spinBox->hasFocus())
	{
		int propertyValue = property->Get<int>(instance);
		spinBox->setValue(propertyValue);
	}
}

void IntWidget::OnSpinBoxChanged(int aNewValue)
{
	int spinboxValue = aNewValue;
	int propertyValue = property->Get<int>(instance);

	if (spinboxValue != propertyValue)
	{
		property->Set<int>(spinboxValue, instance);
	}
}

bool IntWidgetFactory::SupportedProperty(Vanguard::Property* aProperty)
{
	return aProperty->Is<int>();
}

IPropertyWidget* IntWidgetFactory::CreatePropertyWidget(void* aInstance, Vanguard::Property* aProperty)
{
	return SupportedProperty(aProperty) ? new IntWidget(aInstance, aProperty) : nullptr;
}
