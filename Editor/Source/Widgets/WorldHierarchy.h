#pragma once
#include "Editor_Common.h"

#include <QDockWidget>

namespace Vanguard
{
	class WorldHierarchy : public QDockWidget
	{
		Q_OBJECT;

	public:
		WorldHierarchy(QWidget* parent);
	};
}