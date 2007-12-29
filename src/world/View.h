#ifndef VIEW_H_
#define VIEW_H_

#include <QPainter>

class View
{
public:
	View() {}
	virtual ~View() {}
	
	virtual void paint(QPainter* p)=0;
};

#endif /*VIEW_H_*/
