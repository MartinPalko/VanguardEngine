#pragma once

#include "Editor_Common.h"

class QWidget;

struct IPropertyWidget
{
	virtual QWidget* GetWidget() = 0;
	virtual void Update() = 0;

	virtual ~IPropertyWidget() {}
};

struct IPropertyWidgetFactory
{
	virtual bool SupportedProperty(Vanguard::Property* aProperty) = 0;
	virtual IPropertyWidget* CreatePropertyWidget(void* aInstance, Vanguard::Property* aProperty) = 0;

	virtual ~IPropertyWidgetFactory() {}
};

