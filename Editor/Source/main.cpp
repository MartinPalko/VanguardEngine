#include "Editor_Common.h"
#include "EditorCore.h"

#include "ProjectSelector.h"

#include <QApplication>


int main(int argc, char **argv)
{
	int returnVal = 0;

	// Create the QApplication
	QApplication qApplication(argc, argv);

	QString project = "";

	// Scoped so ProjectSelector is allocated on the stack, and deleted right after it's done.
	{
		Vanguard::ProjectSelector projectSelector(project);
		projectSelector.exec();
	}

	if (project != "")
	{
		// Create, initialize, and run EditorCore.
		Vanguard::EditorCore EditorCore;
		EditorCore.Initialize(argc, argv, project.toLatin1());
		EditorCore.Run();
	}

	return returnVal;
}