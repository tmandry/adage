#include "View.h"
#include <QPainter>

void CircleView::paint(QPainter* p)
{
	p->save();

	p->setPen(QPen(mColor, mThickness));
	if (mFill) p->setBrush(QBrush(mColor));
	p->drawEllipse(mPos, mRadius, mRadius);

	p->restore();
}
