#pragma once

#include "Editor_Common.h"
#include "IPropertyWidget.h"

#include <QWidget>
#include <QDoubleSpinBox>

class QuaternionSpinBox : public QDoubleSpinBox
{
	Q_OBJECT;

public:
	QuaternionSpinBox(QWidget* aParent);
	virtual QString textFromValue(double value) const override;
};

class QuaternionWidget : public QWidget, public IPropertyWidget
{
	Q_OBJECT;

	void* instance;
	Vanguard::Property* property;

	QuaternionSpinBox* xSpinBox;
	QuaternionSpinBox* ySpinBox;
	QuaternionSpinBox* zSpinBox;

public:
	QuaternionWidget(void* aInstance, Vanguard::Property* aProperty);

	// Implement IPropertyWidget
	virtual QWidget* GetWidget() override;
	virtual void Update() override;

	private slots:
	void OnXSpinBoxChanged(double aNewValue);
	void OnYSpinBoxChanged(double aNewValue);
	void OnZSpinBoxChanged(double aNewValue);
};

class QuaternionWidgetFactory : public IPropertyWidgetFactory
{
	// Implement IPropertyWidgetFactory
	virtual bool SupportedProperty(Vanguard::Property* aProperty) override;
	virtual IPropertyWidget* CreatePropertyWidget(void* aInstance, Vanguard::Property* aProperty) override;
};