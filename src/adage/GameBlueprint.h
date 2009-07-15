#ifndef GAMEBLUEPRINT_H_
#define GAMEBLUEPRINT_H_

#include "ui/Blueprint.h"
#include "Game.h"

class GameBlueprint : public Blueprint
{
	Q_OBJECT

public:
	enum ToolType {
		dormantPortal,
		portal,
		trap,
		nav
	};

	GameBlueprint(Game* game, QWidget* parent=0): Blueprint(game, parent), mGame(game) {}
	virtual ~GameBlueprint() {}

public slots:
	void setTool(int tool);
	//eventually will be used for returning the view to the main point of interest in the mission
	void goHome();

protected:
	void mouseReleaseEvent(QMouseEvent* event);

private:
	void placeTool(Math::Point pos);

	Game* mGame;

	ToolType mTool;
};

#endif /*GAMEBLUEPRINT_H_*/
