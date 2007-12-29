#ifndef GAMEUI_H_
#define GAMEUI_H_

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTextEdit>
#include "Game.h"
#include "ui/BlueprintWindow.h"

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
