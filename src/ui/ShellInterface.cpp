#include <QWidget>
#include <QLabel>
#include <QKeyEvent>
#include <QVBoxLayout>

#include "ShellInterface.h"

ShellInterface::ShellInterface(QWidget *parent) : QWidget(parent)
{
	mInput = new QLabel("Hello, World");
	
	mLayout = new QVBoxLayout;
	mLayout->addWidget(mInput);
	setLayout(mLayout);
	
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
