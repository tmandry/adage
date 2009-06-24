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
#include "ui/ShellInterface.h"
#include "world/GhostBustersHQ.h"
#include "world/Map.h"
#include "actors/Person.h"
#include "actors/Ghost.h"
#include "actors/GhostBuster.h"
#include "math/rand.h"
#include <iostream>

GameUI::GameUI()
{
	resize(930, 640);
	setWindowTitle("Adage");

	/*mLayout = new QVBoxLayout(this);

	QHBoxLayout* horizLayout = new QHBoxLayout();

	mButton = new QPushButton("Push me");
	horizLayout->addWidget(mButton, 0, Qt::AlignLeft);

	mBtnClose = new QPushButton("Close");
	horizLayout->addWidget(mBtnClose, 0, Qt::AlignRight);

	mLayout->addLayout(horizLayout);*/

	QDockWidget *dock = new QDockWidget(tr("Shell"), this);
	ShellWindow *shell = new ShellWindow(dock);
	//ShellInterface *shellInt = new ShellInterface(shell);
	dock->setWidget(shell);
	addDockWidget(Qt::RightDockWidgetArea, dock);

	dock = new QDockWidget(tr("Comm"), this);
	CommWindow *comm = new CommWindow(dock);
	dock->setWidget(comm);
	addDockWidget(Qt::RightDockWidgetArea, dock);

	//connect(mBtnClose, SIGNAL(clicked()), this, SLOT(close()));

	mGame = new Game(comm);
	//connect(mGame, SIGNAL(worldUpdated()), this, SLOT(update()));

	//TEST GAME ENTITIES GO HERE
	Map* map = new Map(mGame->world());
	map->open("building.txt");
	map->load();

	new GhostBustersHQ(mGame->world());

	for (int i=0; i<50; ++i)
		new Person(Math::Point(Math::randFloat(-75,75),Math::randFloat(-80,90)), mGame->world());
	for (int i=0; i<50; ++i)
		new Person(Math::Point(Math::randFloat(-75,75),Math::randFloat(190,260)), mGame->world());
	for (int i=0; i<17; ++i)
		new Ghost(Math::Point(Math::randFloat(-100,100),Math::randFloat(-100,100)), mGame->world());
	for (int i=0; i<17; ++i)
		new Ghost(Math::Point(Math::randFloat(-100,100),Math::randFloat(180,380)), mGame->world());
	for (int i=0; i<4; ++i)
		new GhostBuster(Math::Point(Math::randFloat(-75,75),Math::randFloat(-90,90)), mGame->world());
	for (int i=0; i<4; ++i)
		new GhostBuster(Math::Point(Math::randFloat(-75,75),Math::randFloat(190,270)), mGame->world());


	//
	mBlueprint = new BlueprintWindow(mGame);
	setCentralWidget(mBlueprint);

	connect(mGame, SIGNAL(worldUpdated()), mBlueprint->blueprint(), SLOT(repaint()));
}

GameUI::~GameUI()
{
	delete mGame;
}
