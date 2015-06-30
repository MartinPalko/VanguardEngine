#pragma once

#include <QMainWindow>

class MainWindow : public QMainWindow
{
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
	
	QMenuBar* menuBar;

	QDockWidget* dock1;
	QDockWidget* dock2;
	QDockWidget* dock3;

};
