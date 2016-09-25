#pragma once
#include "Editor_Common.h"

#include <QDockWidget>

namespace Vanguard
{
	class Inspector : public QDockWidget
	{
		Q_OBJECT;

	public:
		Inspector(QWidget* parent);
	};
}