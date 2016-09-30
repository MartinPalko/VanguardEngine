#pragma once
#include "Editor_Common.h"

#include <QDockWidget>

class QTextEdit;

namespace Vanguard
{
	class LogViewer : public QDockWidget, ILogListener
	{
		Q_OBJECT;

		QTextEdit* textWidget;
		bool autoScroll;

	public:
		LogViewer(QWidget* parent);
		~LogViewer();

		// Implement ILogListener
		void OnMessageLogged(LogEntry aMessage) override;
	};
}