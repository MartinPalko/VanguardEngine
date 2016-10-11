#include "Inspector.h"
#include "EditorEvents.h"
#include "Widgets/Inspector/EntityInspectorWidget.h"
#include "Widgets/Inspector/ComponentInspectorWidget.h"

#include <QScrollArea>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QLayout>
#include <QTimer>

namespace Vanguard
{
	void Inspector::OnEditorEvent(EditorEvent* aEvent)
	{
		if (EntitySelectedEvent* selectedEvent = Type::SafeCast<EntitySelectedEvent>(aEvent))
		{
			SetEntity(selectedEvent->GetEntity());
		}
		else if (aEvent->GetType()->IsA<EntityUnselectedEvent>())
		{
			SetEntity(nullptr);
		}
	}

	Inspector::Inspector(QWidget* parent) : QDockWidget("Inspector", parent)
		, entityInspector(nullptr)
		, componentInspectors()
		, mainWidget(nullptr)
		, mainLayout(nullptr)
	{
		EditorCore::GetInstance()->RegisterEventListener(this);

		scrollArea = new QScrollArea(this);
		scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
		scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
		scrollArea->setWidgetResizable(true);
		setWidget(scrollArea);

		mainWidget = new QWidget(scrollArea);
		scrollArea->setWidget(mainWidget);
		
		updateTimer = new QTimer(this);
		updateTimer->setInterval(200); // How often the inspector updates values
		connect(updateTimer, &QTimer::timeout, this, &Inspector::Update);
		updateTimer->start();

		SetEntity(nullptr);
	}

	Inspector::~Inspector()
	{
		EditorCore::GetInstance()->UnregisterEventListener(this);
	}

	void Inspector::SetEntity(Entity* aEntity)
	{
		// Clear old component inspectors
		if (mainWidget)
		{
			delete mainWidget;
			mainWidget = nullptr;
		}
		componentInspectors.Clear();
		entityInspector = nullptr;

		if (aEntity)
		{
			mainWidget = new QWidget(scrollArea);
			mainWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

			mainLayout = new QVBoxLayout(mainWidget);

			entityInspector = new EntityInspectorWidget(aEntity, mainWidget);
			mainLayout->addWidget(entityInspector);

			for (int i = 0; i < aEntity->GetNumComponents(); i++)
			{
				auto componentInspector = new ComponentInspectorWidget(aEntity->GetComponent(i), mainWidget);
				mainLayout->addWidget(componentInspector);
				componentInspectors.PushBack(componentInspector);
			}

			mainLayout->addStretch(1);
			mainWidget->setLayout(mainLayout);
			scrollArea->setWidget(mainWidget);
		}

		scrollArea->updateGeometry();
	}

	void Inspector::Update()
	{
		if (entityInspector)
		{
			entityInspector->UpdatePropertyWidgets();
		}

		for (ComponentInspectorWidget* componentInspector : componentInspectors)
		{
			componentInspector->UpdatePropertyWidgets();
		}
	}
}