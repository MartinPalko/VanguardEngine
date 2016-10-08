#pragma once
#include "Editor_Common.h"

#include <QDialog>

class QListWidget;
class QPushButton;

namespace Vanguard
{
	class ProjectSelector : public QDialog
	{
		Q_OBJECT;

	private:
		QString& outSelectedProject;

		QListWidget* projectList;
		QPushButton* loadButton;

	public:
		ProjectSelector(QString& aOutSelectedProject);

	private:
		void OnFinished(int result);
	};
}