#ifndef EDITORBLUEPRINT_H_
#define EDITORBLUEPRINT_H_

#include "ui/Blueprint.h"
#include "Pointer.h"

class EditorGame;
class Entity;

class EditorBlueprint: public Blueprint
{
	Q_OBJECT
public:
	enum ToolType {
		pan,
		move
	};

	EditorBlueprint(EditorGame* game, QWidget* parent=0);
	virtual ~EditorBlueprint();

protected:
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void mouseMoveEvent(QMouseEvent* event);
	virtual void mouseReleaseEvent(QMouseEvent* event);

public slots:
	void setTool(int tool);

signals:
	void dropEntity(Math::Point pos);

private:
	ToolType mTool;
	bool mPanning;
	bool mMoving;
	Pointer<Entity> mMoveEnt;
};

#endif /* EDITORBLUEPRINT_H_ */
