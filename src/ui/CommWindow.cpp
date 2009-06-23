#include "CommWindow.h"

#include <QSize>
#include <QSizePolicy>
#include <QScrollBar>
#include <QString>

CommWindow::CommWindow(QWidget* parent): QTextEdit(parent)
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

void CommWindow::log(QString text)
{
	mText += text + "<br>\n";
	setHtml(mText);
	verticalScrollBar()->setValue(verticalScrollBar()->maximum());
}
