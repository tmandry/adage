#include <QWidget>
#include <QLabel>
#include <QKeyEvent>
#include <QVBoxLayout>

#include "ShellInterface.h"

#include <iostream>

ShellInterface::ShellInterface(QWidget *parent) : QWidget(parent)
{
	mInput = new QLabel;
	mParent = parent;

	mInput->setWordWrap(true);
	
	mLayout = new QVBoxLayout;
	mLayout->addWidget(mInput);
	setLayout(mLayout);

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
	QString text = mInput->text();

	switch(event->key())
	{
		case Qt::Key_Backspace:
			text.remove(text.size() - 1, 1);
		break;

		default:
			text += event->text();
	}
	mInput->setText(text);
}
