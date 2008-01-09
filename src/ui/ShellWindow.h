#ifndef SHELLWINDOW_H_
#define SHELLWINDOW_H_

#include <QWidget>
#include <QDockWidget>
#include <QTabWidget>
#include <QKeyEvent>

class ShellWindow : public QWidget
{
public:
	ShellWindow(QDockWidget* parent=0);
	virtual ~ShellWindow();
	
	virtual QSize sizeHint() const; 
	
private:

	QDockWidget *mParent;
	QTabWidget *mShellTab;
};

#endif /*SHELLWINDOW_H_*/
