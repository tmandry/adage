#include "EditorBlueprint.h"
#include "EditorGame.h"
#include "world/Entity.h"
#include "world/Building.h"
#include "world/View.h"
#include "world/World.h"

EditorBlueprint::EditorBlueprint(EditorGame* game, QWidget* parent)
	:	Blueprint(game, parent),
		mTool(pan),
		mPanning(false),
		mMoving(false),
		mBuilding(false),
		mCtrlPressed(false),
		mBuildPath(0)
{
	setFocusPolicy(Qt::ClickFocus);
	setMouseTracking(true);
}

EditorBlueprint::~EditorBlueprint()
{
}

void EditorBlueprint::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton) {
		switch(mTool) {
		case pan:
			mPanning = true;
			Blueprint::mousePressEvent(event);
			break;
		case move: {
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
			break;
		} default:
			break;
		}

		if (mTool == build && !mCtrlPressed) {
			mPanning = true;
			Blueprint::mousePressEvent(event);
		}
	} else if (event->button() == Qt::RightButton) {
		emit dropEntity(screenToWorld(event->pos()));
	}
}

void EditorBlueprint::mouseMoveEvent(QMouseEvent* event)
{
	if (mPanning) {
		Blueprint::mouseMoveEvent(event);
	} else if (mMoving) {
		mMoveEnt->setPos(screenToWorld(event->pos()));
		update();
	} else if (mBuilding) {
		mBuildNextPoint = screenToWorld(event->pos());
		update();
	}
}

void EditorBlueprint::mouseReleaseEvent(QMouseEvent* event)
{
	if (mPanning) Blueprint::mouseReleaseEvent(event);
	mPanning = mMoving = false;

	if (mTool = build) {
		if (mCtrlPressed) {
			if (!mBuilding) { //first point
				mBuildFromPoint = screenToWorld(event->pos());
				mBuildNextPoint = mBuildFromPoint;

				if (mBuildPath) delete mBuildPath;
				mBuildPath = new QPainterPath(mBuildFromPoint);

				mBuildPoints.clear();
				mBuildPoints << mBuildFromPoint;

				mBuilding = true;
			} else { //if we're already building (a point was already clicked) build a line from the last point to this
				Math::Point point = screenToWorld(event->pos());

				mBuildPath->lineTo(point);
				mBuildPoints << point;
				mBuildFromPoint = point;
				mBuildNextPoint = point;
			}
			update();
		}
	}
}

void EditorBlueprint::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Control) mCtrlPressed = true;
	else Blueprint::keyPressEvent(event);
}

void EditorBlueprint::keyReleaseEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Control) {
		mCtrlPressed = false;
		if (mBuilding) {
			doBuild();
			mBuilding = false;
			update();
		}
	}
	else Blueprint::keyReleaseEvent(event);
}

void EditorBlueprint::paintEvent(QPaintEvent* event)
{
	Blueprint::paintEvent(event);

	if (mBuilding) {
		QPainter* p = createPainter();

		p->setPen(QPen(Qt::red, 0.5));
		p->drawPath(*mBuildPath);
		p->drawLine(mBuildFromPoint, mBuildNextPoint);

		delete p;
	}
}

void EditorBlueprint::setTool(int tool)
{
	mTool = (ToolType)tool;
}

void EditorBlueprint::doBuild()
{
	if (mBuildPoints.size() < 2) return;

	Building* building = new Building(game()->world());
	building->createWalls(mBuildPoints);
}
