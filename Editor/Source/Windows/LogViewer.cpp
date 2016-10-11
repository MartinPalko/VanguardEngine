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

		// Add messages that happened before we were created.
		for (int i = 0; i < Log::GetNumCachedEntries(); i++)
		{
			OnMessageLogged(Log::GetCachedEntry(i));
		}

		Log::RegisterListener(this);
	}

	LogViewer::~LogViewer()
	{
		Log::UnregisterListener(this);
	}

	void LogViewer::OnMessageLogged(LogEntry aMessage)
	{
		QString messageHeader;
		QString messageFooter;

		switch (aMessage.GetWarningLevel())
		{
		case LogEntryErrorLevel::Warning:
			messageHeader = "<font color=\"#ffff00\">"; // Yellow
			messageFooter = "</font>";
			break;
		case LogEntryErrorLevel::Error:
		case LogEntryErrorLevel::Exception:
			messageHeader = "<font color=\"#ff0000\">"; // Red
			messageFooter = "</font>";
			break;
		default:
		case LogEntryErrorLevel::Message:
			messageHeader = ""; // None
			messageFooter = "";
			break;
		}

		textWidget->moveCursor(QTextCursor::MoveOperation::End);
		textWidget->insertHtml(messageHeader + aMessage.GetFormattedLogEntry().GetCharPointer() + messageFooter + "<br>");

		if (autoScroll)
		{
			textWidget->verticalScrollBar()->setValue(textWidget->verticalScrollBar()->maximum());
		}
	}
}