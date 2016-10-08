#include "Vector3Widget.h"

#include <QHBoxLayout>
#include <QDoubleSpinBox>

Vector3Widget::Vector3Widget(void* aInstance, Vanguard::Property* aProperty)
	: instance (aInstance)
	, property (aProperty)
{
	setLayout(new QHBoxLayout());

	xSpinBox = new QDoubleSpinBox(this);
	xSpinBox->setRange(Vanguard::LowestFloat(), Vanguard::MaxFloat());
	connect(xSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &Vector3Widget::OnSpinBoxChanged);
	layout()->addWidget(xSpinBox);

	ySpinBox = new QDoubleSpinBox(this);
	ySpinBox->setRange(Vanguard::LowestFloat(), Vanguard::MaxFloat());
	connect(ySpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &Vector3Widget::OnSpinBoxChanged);
	layout()->addWidget(ySpinBox);

	zSpinBox = new QDoubleSpinBox(this);
	zSpinBox->setRange(Vanguard::LowestFloat(), Vanguard::MaxFloat());
	connect(zSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &Vector3Widget::OnSpinBoxChanged);
	layout()->addWidget(zSpinBox);
}

QWidget* Vector3Widget::GetWidget()
{
	return this;
}

void Vector3Widget::Update()
{
	Vanguard::Vector3 propertyValue = property->Get<Vanguard::Vector3>(instance);

	xSpinBox->setValue(propertyValue.x);
	ySpinBox->setValue(propertyValue.y);
	zSpinBox->setValue(propertyValue.z);
}

void Vector3Widget::OnSpinBoxChanged(double d)
{
	Vanguard::Vector3 spinBoxValues = Vanguard::Vector3(xSpinBox->value(), ySpinBox->value(), zSpinBox->value());
	Vanguard::Vector3 propertyValue = property->Get<Vanguard::Vector3>(instance);

	if (spinBoxValues != propertyValue)
	{
		property->Set<Vanguard::Vector3>(spinBoxValues, instance);
	}
}

bool Vector3WidgetFactory::SupportedProperty(Vanguard::Property* aProperty)
{
	return aProperty->Is<Vanguard::Vector3>();
}

IPropertyWidget* Vector3WidgetFactory::CreatePropertyWidget(void* aInstance, Vanguard::Property* aProperty)
{
	return SupportedProperty(aProperty) ? new Vector3Widget(aInstance, aProperty) : nullptr;
}
