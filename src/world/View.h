#ifndef VIEW_H_
#define VIEW_H_

#include <QColor> //designed to be separate from QtGui but that can happen later
#include "math/Point.h"
class QPainter;

class View
{
public:
	View() {}
	virtual ~View() {}

	virtual void paint(QPainter* p)=0;
};

class CircleView : public View {
public:
	CircleView(Math::Point pos, double radius, QColor color, double thickness = 0.1, bool fill = false)
		: mPos(pos), mRadius(radius), mColor(color), mThickness(thickness), mFill(fill) {}

	void paint(QPainter* p);

	void setRadius(double radius) { mRadius = radius; }
	double radius() const { return mRadius; }

private:
	Math::Point mPos;
	double mRadius;
	QColor mColor;
	double mThickness;
	bool mFill;
};

#endif /*VIEW_H_*/
