#include "ShellWindow.h"
#include "ShellInterface.h"

#include <QSizePolicy>
#include <QSize>
#include <QTabWidget>
#include <QKeyEvent>

#include <iostream>

ShellWindow::ShellWindow(QDockWidget* p): QTabWidget(p)
{
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	
	setWindowTitle("Adage Terminal Interface");

	addTab(new ShellInterface(this), tr("adgterm"));

	setFocusPolicy(Qt::WheelFocus);
}

ShellWindow::~ShellWindow()
{
}

QSize ShellWindow::sizeHint() const
{
	return QSize(430,320);
}
