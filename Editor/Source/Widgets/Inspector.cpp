#include "Inspector.h"
#include "EditorEvents.h"

namespace Vanguard
{
	void Inspector::OnEditorEvent(EditorEvent* aEvent)
	{
		if (EntitySelectedEvent* selectedEvent = Type::SafeCast<EntitySelectedEvent>(aEvent))
		{
			SetEntity(selectedEvent->GetEntity());
		}
	}

	Inspector::Inspector(QWidget* parent) : QDockWidget("Inspector", parent)
		, entity(nullptr)
	{
		EditorCore::GetInstance()->RegisterEventListener(this);
	}

	Inspector::~Inspector()
	{
		EditorCore::GetInstance()->UnregisterEventListener(this);
	}

	void Inspector::SetEntity(Entity * aEntity)
	{
		setWindowTitle(("Inspector: " + aEntity->GetType()->GetTypeName()).GetCharPointer());
	}
}