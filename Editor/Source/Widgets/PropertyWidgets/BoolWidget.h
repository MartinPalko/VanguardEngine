#pragma once

#include "Editor_Common.h"
#include "IPropertyWidget.h"

#include <QWidget>

class QCheckBox;

class BoolWidget : public QWidget, public IPropertyWidget
{
	friend class Vector3WidgetFactory;

	Q_OBJECT;

	void* instance;
	Vanguard::Property* property;

	QCheckBox* checkBox;

public:
	BoolWidget(void* aInstance, Vanguard::Property* aProperty);

	// Implement IPropertyWidget
	virtual QWidget* GetWidget() override;
	virtual void Update() override;

	private slots:
	void OnCheckboxChanged();
};

class BoolWidgetFactory : public IPropertyWidgetFactory
{
	// Implement IPropertyWidgetFactory
	virtual bool SupportedProperty(Vanguard::Property* aProperty) override;
	virtual IPropertyWidget* CreatePropertyWidget(void* aInstance, Vanguard::Property* aProperty) override;
};
