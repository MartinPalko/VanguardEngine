#include "EntityInspectorWidget.h"
#include "EditorEvents.h"
#include "EditorCore.h"

#include <QScrollArea>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QLayout>

namespace Vanguard
{
	EntityInspectorWidget::EntityInspectorWidget(Entity* aEntity, QWidget* aParent) : ObjectInspectorWidget(aParent)
		, entity(aEntity)
	{
		setLayout(new QVBoxLayout());
		layout()->setSpacing(5);

		auto nameEditLayout = new QHBoxLayout();

		entityEnabledCheckbox = new QCheckBox(this);
		connect(entityEnabledCheckbox, &QCheckBox::stateChanged, this, &EntityInspectorWidget::OnEnabledCheckboxChanged);
		nameEditLayout->addWidget(entityEnabledCheckbox);

		entityNameLineEdit = new QLineEdit(this);
		connect(entityNameLineEdit, &QLineEdit::textChanged, this, &EntityInspectorWidget::OnNameLineEditChanged);
		nameEditLayout->addWidget(entityNameLineEdit);

		layout()->addItem(nameEditLayout);

		DynamicArray<Vanguard::String> ignoredProperties;
		ignoredProperties.PushBack("enabled");

		CreatePropertyWidgets(aEntity, ignoredProperties);

		entityNameLineEdit->setText(aEntity->GetName().GetCharPointer());
		entityEnabledCheckbox->setChecked(aEntity->Enabled());
	}

	void EntityInspectorWidget::UpdatePropertyWidgets()
	{
		if (!entityNameLineEdit->hasFocus())
			entityNameLineEdit->setText(entity->GetName().GetCharPointer());

		entityEnabledCheckbox->setChecked(entity->Enabled());

		ObjectInspectorWidget::UpdatePropertyWidgets();
	}

	void EntityInspectorWidget::OnNameLineEditChanged()
	{
		if (entity && entityNameLineEdit->text() != entity->GetName())
		{
			entity->SetName(ToVanguardString(entityNameLineEdit->text()));
		}
	}

	void EntityInspectorWidget::OnEnabledCheckboxChanged()
	{
		if (entity && entityEnabledCheckbox->isChecked() != entity->Enabled())
		{
			entity->SetEnabled(entityEnabledCheckbox->isChecked());
		}
	}

}