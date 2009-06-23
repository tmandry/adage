#ifndef COMMWINDOW_H_
#define COMMWINDOW_H_

#include <string>
#include <QTextEdit>

class CommWindow : public QTextEdit
{
	Q_OBJECT
public:
	CommWindow(QWidget* parent=0);
	virtual ~CommWindow();

	virtual QSize sizeHint() const;

public slots:
	void log(QString text);

private:
	QString mText;
};

#endif /*COMMWINDOW_H_*/
