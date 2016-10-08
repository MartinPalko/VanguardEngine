#pragma once

#include "Editor_Common.h"
#include "IPropertyWidget.h"

#include "Editor_Common.h"
#include "IPropertyWidget.h"

#include <QWidget>

class QSpinBox;

class IntWidget : public QWidget, public IPropertyWidget
{
	Q_OBJECT;

	void* instance;
	Vanguard::Property* property;

	QSpinBox* spinBox;

public:
	IntWidget(void* aInstance, Vanguard::Property* aProperty);

	// Implement IPropertyWidget
	virtual QWidget* GetWidget() override;
	virtual void Update() override;

	private slots:
	void OnSpinBoxChanged(int aNewValue);
};

class IntWidgetFactory : public IPropertyWidgetFactory
{
	// Implement IPropertyWidgetFactory
	virtual bool SupportedProperty(Vanguard::Property* aProperty) override;
	virtual IPropertyWidget* CreatePropertyWidget(void* aInstance, Vanguard::Property* aProperty) override;
};
