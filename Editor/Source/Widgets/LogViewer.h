#pragma once
#include "Editor_Common.h"

#include <QDockWidget>

namespace Vanguard
{
	class LogViewer : public QDockWidget
	{
		Q_OBJECT;

	public:
		LogViewer(QWidget* parent);
	};
}