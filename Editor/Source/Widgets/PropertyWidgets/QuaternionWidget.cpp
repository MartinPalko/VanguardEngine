#include "QuaternionWidget.h"

#include <QHBoxLayout>


QuaternionSpinBox::QuaternionSpinBox(QWidget* aParent) : QDoubleSpinBox(aParent)
{
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	setRange(Vanguard::MinFloat(), Vanguard::MaxFloat());
}

QString QuaternionSpinBox::textFromValue(double value) const
{
	// Returning short strings here stops min and max values making the spinboxes way too wide.
	if (value == Vanguard::MinFloat())
		return "Min";
	else if (value == Vanguard::MaxFloat())
		return "Max";
	else
		return QDoubleSpinBox::textFromValue(value);
}

QuaternionWidget::QuaternionWidget(void* aInstance, Vanguard::Property* aProperty)
	: instance (aInstance)
	, property (aProperty)
{
	setLayout(new QHBoxLayout());

	xSpinBox = new QuaternionSpinBox(this);
	connect(xSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &QuaternionWidget::OnXSpinBoxChanged);
	layout()->addWidget(xSpinBox);

	ySpinBox = new QuaternionSpinBox(this);
	connect(ySpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &QuaternionWidget::OnYSpinBoxChanged);
	layout()->addWidget(ySpinBox);

	zSpinBox = new QuaternionSpinBox(this);
	connect(zSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &QuaternionWidget::OnZSpinBoxChanged);
	layout()->addWidget(zSpinBox);
}

QWidget* QuaternionWidget::GetWidget()
{
	return this;
}

void QuaternionWidget::Update()
{
	Vanguard::EulerAngles propertyValue = property->Get<Vanguard::Quaternion>(instance).ToEuler();

	if (!xSpinBox->hasFocus())
		xSpinBox->setValue(propertyValue.x);

	if (!ySpinBox->hasFocus())
		ySpinBox->setValue(propertyValue.y);

	if (!zSpinBox->hasFocus())
		zSpinBox->setValue(propertyValue.z);
}

void QuaternionWidget::OnXSpinBoxChanged(double aNewValue)
{
	Vanguard::Vector3 propertyValue = property->Get<Vanguard::Vector3>(instance);

	if (aNewValue != propertyValue.x)
	{
		propertyValue.x = aNewValue;
		property->Set<Vanguard::Vector3>(propertyValue, instance);
	}
}

void QuaternionWidget::OnYSpinBoxChanged(double aNewValue)
{
	Vanguard::Vector3 propertyValue = property->Get<Vanguard::Vector3>(instance);

	if (aNewValue != propertyValue.y)
	{
		propertyValue.x = aNewValue;
		property->Set<Vanguard::Vector3>(propertyValue, instance);
	}
}

void QuaternionWidget::OnZSpinBoxChanged(double aNewValue)
{
	Vanguard::Vector3 propertyValue = property->Get<Vanguard::Vector3>(instance);

	if (aNewValue != propertyValue.z)
	{
		propertyValue.z = aNewValue;
		property->Set<Vanguard::Vector3>(propertyValue, instance);
	}
}

bool QuaternionWidgetFactory::SupportedProperty(Vanguard::Property* aProperty)
{
	return aProperty->Is<Vanguard::Quaternion>();
}

IPropertyWidget* QuaternionWidgetFactory::CreatePropertyWidget(void* aInstance, Vanguard::Property* aProperty)
{
	return SupportedProperty(aProperty) ? new QuaternionWidget(aInstance, aProperty) : nullptr;
}
