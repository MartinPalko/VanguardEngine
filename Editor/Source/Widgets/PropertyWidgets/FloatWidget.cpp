#include "FloatWidget.h"
#include "Vector3Widget.h"

#include <QHBoxLayout>
#include <QDoubleSpinBox>

FloatWidget::FloatWidget(void* aInstance, Vanguard::Property* aProperty)
	: instance (aInstance)
	, property (aProperty)
{
	setLayout(new QHBoxLayout());

	spinBox = new QDoubleSpinBox(this);
	spinBox->setRange(Vanguard::LowestFloat(), Vanguard::MaxFloat());
	connect(spinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &FloatWidget::OnSpinBoxChanged);
	layout()->addWidget(spinBox);
}

QWidget* FloatWidget::GetWidget()
{
	return this;
}

void FloatWidget::Update()
{
	float propertyValue = property->Get<float>(instance);

	spinBox->setValue(propertyValue);
}

void FloatWidget::OnSpinBoxChanged(double aNewValue)
{
	float spinboxValue = aNewValue;
	float propertyValue = property->Get<float>(instance);

	if (spinboxValue != propertyValue)
	{
		property->Set<float>(spinboxValue, instance);
	}
}

bool FloatWidgetFactory::SupportedProperty(Vanguard::Property* aProperty)
{
	return aProperty->Is<float>();
}

IPropertyWidget* FloatWidgetFactory::CreatePropertyWidget(void* aInstance, Vanguard::Property* aProperty)
{
	return SupportedProperty(aProperty) ? new FloatWidget(aInstance, aProperty) : nullptr;
}
