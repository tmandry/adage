#ifndef GAMEUI_H_
#define GAMEUI_H_

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTextEdit>
#include "game.h"
#include "ui/blueprintwindow.h"

class GameUI : public QMainWindow
{
public:
	GameUI();
	~GameUI();
	
private:
	Game* mGame;
	BlueprintWindow* mBlueprint;

	QVBoxLayout* mLayout;
	QPushButton* mButton;
	QPushButton* mBtnClose;
};

#endif /*GAMEUI_H_*/
