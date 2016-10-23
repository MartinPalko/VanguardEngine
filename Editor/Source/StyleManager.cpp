#include "Editor_Common.h"
#include "EditorDirectories.h"

#include "StyleManager.h"

#include <QApplication>
#include <QString>
#include <QFile>
#include <QDir>
#include <QFont>

#include "sass.h"

QString LoadStyleSheet(Vanguard::FilePath aFilePath, bool aSass)
{
	if (!Vanguard::FileSystem::FileExists(aFilePath))
		return "";

	QString output;

	if (aSass)
	{
		Sass_File_Context* context = sass_make_file_context(aFilePath.GetFullPathName().GetCharPointer());
		Sass_Options* options = sass_file_context_get_options(context);
		sass_option_set_precision(options, 1);
		sass_option_set_source_comments(options, true);

		sass_file_context_set_options(context, options);

		Sass_Context* sass_context = sass_file_context_get_context(context);

		Sass_Compiler* compiler = sass_make_file_compiler(context);
		sass_compiler_parse(compiler);
		sass_compiler_execute(compiler);

		output = sass_context_get_output_string(sass_context);

		// Retrieve errors during compilation
		int status = sass_context_get_error_status(sass_context);
		const char* error = sass_context_get_error_text(sass_context);

		if (status)
		{
			LOG_ERROR(error, "SCSS");
		}

		// Release memory dedicated to the C compiler
		sass_delete_compiler(compiler);
		sass_delete_file_context(context);
	}
	else
	{
		QFile styleSheetFile(aFilePath.GetFullPathName().GetCharPointer());
		styleSheetFile.open(QFile::ReadOnly);
		output = QLatin1String(styleSheetFile.readAll());
	}

	// Parse and replace all local url references with absolute paths.
	QString directory = aFilePath.GetParentDirectory().GetFullPathName().GetCharPointer();
	directory.replace("\\", "/");
	output.replace("url(./", "url(" + directory + "/", Qt::CaseInsensitive);

	return output;
}

StyleManager* StyleManager::GetInstance()
{
	if (!QApplication::instance())
	{
		LOG_EXCEPTION("A QApplication must exist before using the StyleManager", "StyleManager");
	}

	static StyleManager instance;
	return &instance;
}

void StyleManager::LoadStyle()
{
	Vanguard::FilePath styleDirectory = Vanguard::EditorDirectories::GetStyleDirectory();
	QDir::addSearchPath("icons", styleDirectory.GetRelative("Icons").GetFullPathName().GetCharPointer());

	Vanguard::FilePath styleSheetPath = styleDirectory.GetRelative("EditorStyle.scss");
	((QApplication*)QApplication::instance())->setStyleSheet(LoadStyleSheet(styleSheetPath, true));

	QFont newFont("Verdana", 9, 50);
	QApplication::setFont(newFont);
}
