#include "Vector3Widget.h"

#include <QHBoxLayout>
#include <QDoubleSpinBox>

Vector3Widget::Vector3Widget(void* aInstance, Vanguard::Property* aProperty)
	: instance (aInstance)
	, property (aProperty)
{
	setLayout(new QHBoxLayout());

	xSpinBox = new QDoubleSpinBox(this);
	xSpinBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);	
	xSpinBox->setRange(Vanguard::MinFloat(), Vanguard::MaxFloat());
	connect(xSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &Vector3Widget::OnXSpinBoxChanged);
	layout()->addWidget(xSpinBox);

	ySpinBox = new QDoubleSpinBox(this);
	ySpinBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	ySpinBox->setRange(Vanguard::MinFloat(), Vanguard::MaxFloat());
	ySpinBox->setMinimumWidth(20);
	connect(ySpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &Vector3Widget::OnYSpinBoxChanged);
	layout()->addWidget(ySpinBox);

	zSpinBox = new QDoubleSpinBox(this);
	zSpinBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	zSpinBox->setRange(Vanguard::MinFloat(), Vanguard::MaxFloat());
	zSpinBox->setMinimumWidth(20);
	connect(zSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &Vector3Widget::OnZSpinBoxChanged);
	layout()->addWidget(zSpinBox);
}

QWidget* Vector3Widget::GetWidget()
{
	return this;
}

void Vector3Widget::Update()
{
	Vanguard::Vector3 propertyValue = property->Get<Vanguard::Vector3>(instance);

	if (!xSpinBox->hasFocus())
		xSpinBox->setValue(propertyValue.x);

	if (!ySpinBox->hasFocus())
		ySpinBox->setValue(propertyValue.y);

	if (!zSpinBox->hasFocus())
		zSpinBox->setValue(propertyValue.z);
}

void Vector3Widget::OnXSpinBoxChanged(double aNewValue)
{
	Vanguard::Vector3 propertyValue = property->Get<Vanguard::Vector3>(instance);
	
	if (aNewValue != propertyValue.x)
	{
		propertyValue.x = aNewValue;
		property->Set<Vanguard::Vector3>(propertyValue, instance);
	}
}

void Vector3Widget::OnYSpinBoxChanged(double aNewValue)
{
	Vanguard::Vector3 propertyValue = property->Get<Vanguard::Vector3>(instance);

	if (aNewValue != propertyValue.y)
	{
		propertyValue.y = aNewValue;
		property->Set<Vanguard::Vector3>(propertyValue, instance);
	}
}

void Vector3Widget::OnZSpinBoxChanged(double aNewValue)
{
	Vanguard::Vector3 propertyValue = property->Get<Vanguard::Vector3>(instance);

	if (aNewValue != propertyValue.z)
	{
		propertyValue.z = aNewValue;
		property->Set<Vanguard::Vector3>(propertyValue, instance);
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
