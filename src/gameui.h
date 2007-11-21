#ifndef GAMEUI_H_
#define GAMEUI_H_

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTextEdit>
#include "game.h"
#include "ui/blueprint.h"

class GameUI : public QMainWindow
{
	Q_OBJECT
public:
	GameUI();
	~GameUI();
	
private:
	Game* mGame;
	Blueprint* mBlueprint;

	QVBoxLayout* mLayout;
	QPushButton* mButton;
	QPushButton* mBtnClose;
};

#endif /*GAMEUI_H_*/
