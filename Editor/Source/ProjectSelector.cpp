#include "ProjectSelector.h"

#include <QListWidget>
#include <QBoxLayout>
#include <QPushButton>

namespace Vanguard
{
	ProjectSelector::ProjectSelector(QString& aOutSelectedProject) : QDialog()
		, outSelectedProject(aOutSelectedProject)
	{
		// Find projects
		DynamicArray<FilePath> projectDirectories = FileSystem::GetSubdirectories(Directories::GetVanguardProjectsDirectory());

		// Construct UI
		setWindowTitle("Select Project");

		auto mainLayout = new QVBoxLayout(this);

		projectList = new QListWidget(this);
		mainLayout->addWidget(projectList);
		for (FilePath projectDirectory : projectDirectories)
		{
			projectList->addItem(projectDirectory.GetFilename().GetCharPointer());
		}

		loadButton = new QPushButton("Load", this);
		mainLayout->addWidget(loadButton);

		connect(loadButton, &QPushButton::clicked, this, &QDialog::accept);
		connect(projectList, &QListWidget::itemDoubleClicked, this, &QDialog::accept);
		connect(this, &QDialog::finished, this, &ProjectSelector::OnFinished);
	}

	void ProjectSelector::OnFinished(int result)
	{
		outSelectedProject = projectList->currentItem()->text();
	}
}
