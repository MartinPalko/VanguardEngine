#include "Editor_Common.h"
#include "EditorCore.h"
#include "EditorDirectories.h"

#include "Widgets/ProjectSelector.h"

#include <QApplication>
#include <QFile>

int main(int argc, char **argv)
{
	int returnVal = 0;

	// Create the QApplication
	QApplication qApplication(argc, argv);

	{
		Vanguard::FilePath styleSheetPath = Vanguard::EditorDirectories::GetStyleDirectory().GetRelative("EditorStyle.css");
		QFile styleSheetFile(styleSheetPath.GetFullPathName().GetCharPointer());
		styleSheetFile.open(QFile::ReadOnly);
		QString styleSheet = QLatin1String(styleSheetFile.readAll());
		qApplication.setStyleSheet(styleSheet);
	}

	QString project = "";
	int projectSelectorReturn;

	{
		Vanguard::ProjectSelector projectSelector(project);
		projectSelectorReturn = projectSelector.exec();
	}

	if (projectSelectorReturn && project != "")
	{
		// Create, initialize, and run EditorCore.
		Vanguard::EditorCore EditorCore(&qApplication);
		EditorCore.Initialize(argc, argv, project.toLatin1());
		EditorCore.Run();
	}

	return returnVal;
}