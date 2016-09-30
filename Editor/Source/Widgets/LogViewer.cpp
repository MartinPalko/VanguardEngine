#include "LogViewer.h"

#include <QTextEdit>
#include <QScrollBar>

namespace Vanguard
{
	LogViewer::LogViewer(QWidget* parent) : QDockWidget("Log", parent)
		, autoScroll(true)
	{
		textWidget = new QTextEdit(this);
		textWidget->setReadOnly(true);
		
		setWidget(textWidget);

		Log::RegisterListener(this);
	}

	LogViewer::~LogViewer()
	{
		Log::UnregisterListener(this);
	}

	void LogViewer::OnMessageLogged(LogEntry aMessage)
	{
		switch (aMessage.GetWarningLevel())
		{
		case LogEntryErrorLevel::Warning:
			textWidget->setTextColor(QColor(255, 220, 0));
			break;
		case LogEntryErrorLevel::Error:
		case LogEntryErrorLevel::Exception:
			textWidget->setTextColor(QColor(255, 0, 0));
			break;
		default:
		case LogEntryErrorLevel::Message:
			textWidget->setTextColor(QColor(0, 0, 0));
			break;
		}

		textWidget->append(aMessage.GetFormattedLogEntry().GetCharPointer());

		if (autoScroll)
		{
			textWidget->verticalScrollBar()->setValue(textWidget->verticalScrollBar()->maximum());
		}
	}
}