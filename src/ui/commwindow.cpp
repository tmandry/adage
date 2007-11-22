#include "commwindow.h"

#include <QSize>
#include <QSizePolicy>

CommWindow::CommWindow(QWidget* parent): QWidget(parent)
{
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

CommWindow::~CommWindow()
{
}

QSize CommWindow::sizeHint() const
{
	return QSize(400, 300);
}
