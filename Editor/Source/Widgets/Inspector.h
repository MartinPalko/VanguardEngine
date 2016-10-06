#pragma once
#include "Editor_Common.h"
#include "EditorCore.h"

#include <QDockWidget>

namespace Vanguard
{
	class Inspector : public QDockWidget , public IEditorEventListener
	{
		Q_OBJECT;

		Entity* entity;

		// Implement IEditorEventListener
		virtual void OnEditorEvent(EditorEvent* aEvent) override;
	public:
		Inspector(QWidget* parent);
		~Inspector();
		
		void SetEntity(Entity* aEntity);
	};
}