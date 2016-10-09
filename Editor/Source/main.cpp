#include "Editor_Common.h"
#include "EditorCore.h"
#include "EditorDirectories.h"
#include "StyleManager.h"
#include "Windows/ProjectSelector.h"

#include <QApplication>

int main(int argc, char **argv)
{
	int returnVal = 0;

	// Create the QApplication
	QApplication qApplication(argc, argv);

	StyleManager::GetInstance()->LoadStyle();

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