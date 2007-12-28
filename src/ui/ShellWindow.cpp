#include "shellwindow.h"

#include <QSizePolicy>
#include <QSize>

ShellWindow::ShellWindow(QDockWidget* parent): QWidget(parent), mParent(parent)
{
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	
	mParent->setWindowTitle("Shell - tyler@gyro:~");
}

ShellWindow::~ShellWindow()
{
}

QSize ShellWindow::sizeHint() const
{
	return QSize(430,320);
}
