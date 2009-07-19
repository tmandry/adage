#ifndef EDITORBLUEPRINT_H_
#define EDITORBLUEPRINT_H_

#include <QPainterPath>
#include <QList>
#include "ui/Blueprint.h"
#include "Pointer.h"
#include "math/Point.h"

class EditorGame;
class Entity;

class EditorBlueprint: public Blueprint
{
	Q_OBJECT
public:
	enum ToolType {
		pan,
		move,
		build
	};

	EditorBlueprint(EditorGame* game, QWidget* parent=0);
	virtual ~EditorBlueprint();

protected:
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void mouseMoveEvent(QMouseEvent* event);
	virtual void mouseReleaseEvent(QMouseEvent* event);

	virtual void keyPressEvent(QKeyEvent* event);
	virtual void keyReleaseEvent(QKeyEvent* event);

	virtual void paintEvent(QPaintEvent* event);

public slots:
	void setTool(int tool);

signals:
	void dropEntity(Math::Point pos);

private:
	void doBuild();

	ToolType mTool;
	bool mPanning;
	bool mMoving;
	Pointer<Entity> mMoveEnt;
	bool mBuilding;
	bool mCtrlPressed;
	Math::Point mBuildFromPoint;
	Math::Point mBuildNextPoint; //where the mouse happens to be
	QPainterPath* mBuildPath;
	QList<Math::Point> mBuildPoints;
};

#endif /* EDITORBLUEPRINT_H_ */
