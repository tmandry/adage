#include "EditorBlueprint.h"
#include "EditorGame.h"
#include "world/Entity.h"
#include "world/View.h"
#include "world/World.h"

EditorBlueprint::EditorBlueprint(EditorGame* game, QWidget* parent)
	:	Blueprint(game, parent),
		mTool(pan)
{
}

EditorBlueprint::~EditorBlueprint()
{
}

void EditorBlueprint::mousePressEvent(QMouseEvent* event)
{
	switch(mTool) {
	case pan:
		mPanning = true;
		Blueprint::mousePressEvent(event);
		break;
	case move:
		Math::Point pos = screenToWorld(event->pos());

		Pointer<Entity> target;
		EntityList<Entity> entities = game()->world()->findEntities<Entity>();
		for (unsigned int i = 0; i < entities.size(); ++i) {
			if (entities[i]->view() && entities[i]->movable() && entities[i]->view()->clickHit(pos)) {
				target = entities[i];
				break;
			}
		}

		if (target) {
			mMoving = true;
			mMoveEnt = target;
		}
	}
}

void EditorBlueprint::mouseMoveEvent(QMouseEvent* event)
{
	if (mPanning) {
		Blueprint::mouseMoveEvent(event);
	} else if (mMoving) {
		mMoveEnt->setPos(screenToWorld(event->pos()));
		update();
	}
}

void EditorBlueprint::mouseReleaseEvent(QMouseEvent* event)
{
	if (mPanning) Blueprint::mouseReleaseEvent(event);
	mPanning = mMoving = false;
}

void EditorBlueprint::setTool(int tool)
{
	mTool = (ToolType)tool;
}
