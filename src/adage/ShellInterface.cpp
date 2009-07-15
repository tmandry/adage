#include <QWidget>
#include <QLabel>
#include <QKeyEvent>
#include <QVBoxLayout>

#include "ShellInterface.h"

#include <iostream>

ShellInterface::ShellInterface(QWidget *parent) : QLabel(parent)
{
	setWordWrap(true);
	setAlignment(Qt::AlignLeft | Qt::AlignTop);

	setFocusPolicy(Qt::WheelFocus);
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

ShellInterface::~ShellInterface()
{
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
