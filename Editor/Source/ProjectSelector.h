#include "Editor_Common.h"

#include <QDialog>

class QListWidget;
class QPushButton;

namespace Vanguard
{
	class ProjectSelector : public QDialog
	{
		// TODO: Need to get MOC working.
		//Q_OBJECT;

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