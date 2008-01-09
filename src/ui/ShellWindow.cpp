#include "ShellWindow.h"
#include "ShellInterface.h"

#include <QSizePolicy>
#include <QSize>
#include <QTabWidget>

ShellWindow::ShellWindow(QDockWidget* parent): QWidget(parent), mParent(parent)
{
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	
	mParent->setWindowTitle("Adage Terminal Interface");

	mShellTab = new QTabWidget(this);
	mShellTab->addTab(new ShellInterface(this), tr("adgterm"));
}

ShellWindow::~ShellWindow()
{
}

QSize ShellWindow::sizeHint() const
{
	return QSize(430,320);
}
