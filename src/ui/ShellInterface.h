#ifndef SHELLINTERFACE_H_
#define SHELLINTERFACE_H_

#include <QWidget>
#include <QLabel>
#include <QKeyEvent>

class ShellInterface : public QWidget
{
	Q_OBJECT

public:
	//ShellInterface(QWidget* parent=0);
	ShellInterface();
	virtual ~ShellInterface();

private:

	void keyPressEvent(QKeyEvent *event);

	//QWidget* mParent;
	QLabel *mInput;
};

#endif	/*SHELLINTERFACE_H_*/
