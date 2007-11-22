#ifndef COMMWINDOW_H_
#define COMMWINDOW_H_

#include <QWidget>

class CommWindow : public QWidget
{
public:
	CommWindow(QWidget* parent=0);
	virtual ~CommWindow();
	
	virtual QSize sizeHint() const;
};

#endif /*COMMWINDOW_H_*/
