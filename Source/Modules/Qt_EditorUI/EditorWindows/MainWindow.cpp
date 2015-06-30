#include "MainWindow.h"

#include "qdockwidget.h"
#include "qmenubar.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
	setObjectName("EditorWindow");
	setWindowTitle("Vanguard Editor");
	resize(800, 600);

	menuBar = new QMenuBar(this);
	QMenu* fileMenu = menuBar->addMenu("File");
	menuBar->addMenu("Edit");
	menuBar->addMenu("Windows");
	menuBar->addMenu("Help");

	fileMenu->addAction("Open");
	setMenuBar(menuBar);

	dock1 = new QDockWidget(this);
	dock1->setWindowTitle("Dock1");
	dock1->setLayoutDirection(Qt::LayoutDirection::LayoutDirectionAuto);
	dock1->autoFillBackground();

	dock2 = new QDockWidget(this);
	dock2->setWindowTitle("Dock2");

	dock3 = new QDockWidget(this);
	dock3->setWindowTitle("Dock3");
}

MainWindow::~MainWindow()
{
	
}
