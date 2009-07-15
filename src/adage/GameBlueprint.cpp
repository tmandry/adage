#include <cmath>
#include <utility>
#include <QWidget>
#include <QPainter>
#include <QSize>
#include <QSizePolicy>
#include "GameBlueprint.h"
#include "world/GameBase.h"
#include "world/Wall.h"
#include "math/real.h"
#include "world/GhostPortal.h"
#include "world/GhostTrap.h"
#include "nav/NavSystem.h"
#include "BlueprintWindow.h"

void GameBlueprint::setTool(int tool)
{
	mTool = (ToolType)tool;
}

void GameBlueprint::mouseReleaseEvent(QMouseEvent* event)
{
	switch (event->button()) {
	case Qt::RightButton:
		placeTool(Math::Point(-panning() + (-QPointF(width(),height()) / 2.0 + event->pos()) / scale()));
		break;
	default:
		Blueprint::mouseReleaseEvent(event);
	}
}

void GameBlueprint::placeTool(Math::Point pos)
{
	switch (mTool) {
	case dormantPortal:
		new DormantGhostPortal(mGame->world(), pos);
		break;
	case portal:
		new GhostPortal(mGame->world(), pos);
		break;
	case trap:
		new GhostTrap(mGame->world(), pos);
		break;
	case nav:
		mGame->navigate(pos);
	}
}

void GameBlueprint::goHome()
{
	setPanning(QPointF(0,0));
	update();
}
