#include "View.h"
#include "world/Entity.h"
#include <QPainter>

void CircleView::paint(QPainter* p)
{
	p->save();

	p->setPen(QPen(mColor, mThickness));
	if (mFill) p->setBrush(QBrush(mColor));
	p->drawEllipse(mParent->pos(), mRadius, mRadius);

	p->restore();
}

bool CircleView::clickHit(Math::Point point) const
{
	return (distanceSq(point, mParent->pos()) < mRadius*mRadius);
}
