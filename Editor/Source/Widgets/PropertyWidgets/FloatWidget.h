#pragma once

#include "Editor_Common.h"
#include "IPropertyWidget.h"

#include "Editor_Common.h"
#include "IPropertyWidget.h"

#include <QWidget>

class QDoubleSpinBox;

class FloatWidget : public QWidget, public IPropertyWidget
{
	friend class Vector3WidgetFactory;

	Q_OBJECT;

	void* instance;
	Vanguard::Property* property;

	QDoubleSpinBox* spinBox;

public:
	FloatWidget(void* aInstance, Vanguard::Property* aProperty);

	// Implement IPropertyWidget
	virtual QWidget* GetWidget() override;
	virtual void Update() override;

	private slots:
	void OnSpinBoxChanged(double aNewValue);
};

class FloatWidgetFactory : public IPropertyWidgetFactory
{
	// Implement IPropertyWidgetFactory
	virtual bool SupportedProperty(Vanguard::Property* aProperty) override;
	virtual IPropertyWidget* CreatePropertyWidget(void* aInstance, Vanguard::Property* aProperty) override;
};
