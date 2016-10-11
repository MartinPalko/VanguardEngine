#include "Vector3Widget.h"

#include <QHBoxLayout>

Vector3SpinBox::Vector3SpinBox(QWidget* aParent) : QDoubleSpinBox(aParent)
{
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	setRange(Vanguard::MinFloat(), Vanguard::MaxFloat());
}

QString Vector3SpinBox::textFromValue(double value) const
{
	// Returning short strings here stops min and max values making the spinboxes way too wide.
	if (value == Vanguard::MinFloat())
		return "Min";
	else if (value == Vanguard::MaxFloat())
		return "Max";
	else
		return QDoubleSpinBox::textFromValue(value);
}

Vector3Widget::Vector3Widget(void* aInstance, Vanguard::Property* aProperty)
	: instance (aInstance)
	, property (aProperty)
{
	setLayout(new QHBoxLayout());
	layout()->setSpacing(0);
	layout()->setMargin(0);

	xSpinBox = new Vector3SpinBox(this);
	connect(xSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &Vector3Widget::OnXSpinBoxChanged);
	layout()->addWidget(xSpinBox);

	ySpinBox = new Vector3SpinBox(this);
	connect(ySpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &Vector3Widget::OnYSpinBoxChanged);
	layout()->addWidget(ySpinBox);

	zSpinBox = new Vector3SpinBox(this);
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
