#include <QWidget>
#include <QLabel>
#include <QKeyEvent>

#include "ShellInterface.h"

#include <iostream>

ShellInterface::ShellInterface()//QWidget *parent) : mParent(parent)
{
	mInput = new QLabel("Hello, World", this);
	setFocusPolicy(Qt::WheelFocus);
}

ShellInterface::~ShellInterface()
{
}

void ShellInterface::keyPressEvent(QKeyEvent *event)
{
	QString data = mInput->text();

	data.append(event->key());
	mInput->setText(data);
}
