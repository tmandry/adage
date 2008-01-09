#include "ShellWindow.h"
#include "ShellInterface.h"

#include <QSizePolicy>
#include <QSize>
#include <QTabWidget>
#include <QKeyEvent>

#include <iostream>

ShellWindow::ShellWindow(QDockWidget* parent): QWidget(parent), mParent(parent)
{
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	
	mParent->setWindowTitle("Adage Terminal Interface");

	mShellTab = new QTabWidget(this);
	mShellTab->addTab(new ShellInterface(this), tr("adgterm"));

	setFocusProxy(mShellTab);
	setFocusPolicy(Qt::WheelFocus);
}

ShellWindow::~ShellWindow()
{
}

QSize ShellWindow::sizeHint() const
{
	return QSize(430,320);
}
