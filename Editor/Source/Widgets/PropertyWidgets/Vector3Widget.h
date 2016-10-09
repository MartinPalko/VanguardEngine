#pragma once

#include "Editor_Common.h"
#include "IPropertyWidget.h"

#include <QWidget>

#include <QWidget>
#include <QDoubleSpinBox>

class Vector3SpinBox : public QDoubleSpinBox
{
	Q_OBJECT;

public:
	Vector3SpinBox(QWidget* aParent);
	virtual QString textFromValue(double value) const override;
};

class Vector3Widget : public QWidget, public IPropertyWidget
{

	Q_OBJECT;

	void* instance;
	Vanguard::Property* property;

	Vector3SpinBox* xSpinBox;
	Vector3SpinBox* ySpinBox;
	Vector3SpinBox* zSpinBox;

public:
	Vector3Widget(void* aInstance, Vanguard::Property* aProperty);

	// Implement IPropertyWidget
	virtual QWidget* GetWidget() override;
	virtual void Update() override;

private slots:
	void OnXSpinBoxChanged(double aNewValue);
	void OnYSpinBoxChanged(double aNewValue);
	void OnZSpinBoxChanged(double aNewValue);
};

class Vector3WidgetFactory : public IPropertyWidgetFactory
{
	// Implement IPropertyWidgetFactory
	virtual bool SupportedProperty(Vanguard::Property* aProperty) override;
	virtual IPropertyWidget* CreatePropertyWidget(void* aInstance, Vanguard::Property* aProperty) override;
};
