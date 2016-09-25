#pragma once
#include "Editor_Common.h"

#include <QWidget>

namespace Vanguard
{
	class Viewport : public QWidget
	{
		Q_OBJECT;

	public:
		Viewport(QWidget* parent);
	};
}