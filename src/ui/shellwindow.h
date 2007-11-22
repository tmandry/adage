#ifndef SHELLWINDOW_H_
#define SHELLWINDOW_H_

#include <QWidget>
#include <QDockWidget>

//TODO: title, new shell button, shell tab bar (?)
//use QDockWidgets for tabs?

class ShellWindow : public QWidget
{
public:
	ShellWindow(QDockWidget* parent=0);
	virtual ~ShellWindow();
	
	virtual QSize sizeHint() const; 
	
private:
	QDockWidget *mParent;
};

#endif /*SHELLWINDOW_H_*/
