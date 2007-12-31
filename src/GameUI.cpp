#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QDockWidget>
#include "GameUI.h"
#include "Game.h"
#include "ui/BlueprintWindow.h"
#include "ui/ShellWindow.h"
#include "ui/CommWindow.h"
#include "world/Building.h"

GameUI::GameUI()
{
	mGame = new Game();
	new Building(mGame->world());
	
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
