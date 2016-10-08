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

		auto nameEditLayout = new QHBoxLayout();	
		auto nameLabel = new QLabel("Name", this);
		nameEditLayout->addWidget(nameLabel);
		entityNameLineEdit = new QLineEdit(this);
		connect(entityNameLineEdit, &QLineEdit::textChanged, this, &EntityInspectorWidget::OnNameLineEditChanged);
		nameEditLayout->addWidget(entityNameLineEdit);
		layout()->addItem(nameEditLayout);

		entityEnabledCheckbox = new QCheckBox("Enabled", this);
		connect(entityEnabledCheckbox, &QCheckBox::stateChanged, this, &EntityInspectorWidget::OnEnabledCheckboxChanged);
		layout()->addWidget(entityEnabledCheckbox);

		entityNameLineEdit->setText(aEntity->GetName().GetCharPointer());
		entityEnabledCheckbox->setChecked(aEntity->Enabled());

		CreatePropertyWidgets(aEntity);
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