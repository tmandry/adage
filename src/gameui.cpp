#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QDockWidget>
#include "gameui.h"
#include "game.h"
#include "ui/blueprintwindow.h"
#include "ui/shellwindow.h"
#include "ui/commwindow.h"
#include "world/wall.h"

GameUI::GameUI()
{
	mGame = new Game();
	mGame->map().addWall( Wall(0,0, 100,200) );
	mGame->map().addWall( Wall(100,200, 200,200) );
	mGame->map().addWall( Wall(200,200, 320,240) );
	
	resize(930, 640);
	setWindowTitle("Adage");
	
	/*mLayout = new QVBoxLayout(this);
	
	QHBoxLayout* horizLayout = new QHBoxLayout(); 
	
	mButton = new QPushButton("Push me");
	horizLayout->addWidget(mButton, 0, Qt::AlignLeft);
	
	mBtnClose = new QPushButton("Close");
	horizLayout->addWidget(mBtnClose, 0, Qt::AlignRight);
	
	mLayout->addLayout(horizLayout);*/
	
	mBlueprint = new BlueprintWindow(mGame);
	setCentralWidget(mBlueprint);
	
	QDockWidget *dock = new QDockWidget(tr("Shell"), this);
	ShellWindow *shell = new ShellWindow(dock);
	dock->setWidget(shell);
	addDockWidget(Qt::RightDockWidgetArea, dock);
	
	dock = new QDockWidget(tr("Comm"), this);
	CommWindow *comm = new CommWindow(dock);
	dock->setWidget(comm);
	addDockWidget(Qt::RightDockWidgetArea, dock);
		
	//connect(mBtnClose, SIGNAL(clicked()), this, SLOT(close()));
}

GameUI::~GameUI()
{
}
