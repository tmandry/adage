#include <QWidget>
#include <QLabel>
#include <QKeyEvent>
#include <QVBoxLayout>

#include "ShellInterface.h"

#include <iostream>

ShellInterface::ShellInterface(QWidget *parent) : QLabel(parent)
{
	setWordWrap(true);

	setFocusPolicy(Qt::WheelFocus);
}

ShellInterface::~ShellInterface()
{
}

QSize ShellInterface::sizeHint() const
{
	return QSize(200,200);
}

void ShellInterface::keyPressEvent(QKeyEvent *event)
{
	QString text( this->text() );

	switch(event->key())
	{
		case Qt::Key_Backspace:
			text.remove(text.size() - 1, 1);
		break;

		default:
			text += event->text();
	}
	setText(text);
}
