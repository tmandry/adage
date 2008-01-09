#ifndef SHELLINTERFACE_H_
#define SHELLINTERFACE_H_

#include <QWidget>
#include <QLabel>
#include <QKeyEvent>

class QVBoxLayout;

class ShellInterface : protected QLabel
{
	Q_OBJECT

public:
	ShellInterface(QWidget* parent=0);
	virtual ~ShellInterface();

private:
	void keyPressEvent(QKeyEvent *event);
};

#endif	/*SHELLINTERFACE_H_*/
