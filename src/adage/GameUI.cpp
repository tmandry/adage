#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QDockWidget>
#include <QFile>
#include "GameUI.h"
#include "Game.h"
#include "BlueprintWindow.h"
#include "ShellWindow.h"
#include "ui/CommWindow.h"
#include "ShellInterface.h"
#include "world/GhostBustersHQ.h"
#include "map/Map.h"
#include "world/GhostTrap.h"
#include "actors/Person.h"
#include "actors/Ghost.h"
#include "actors/GhostBuster.h"
#include "actors/Navigator.h"
#include "math/rand.h"
#include <iostream>

GameUI::GameUI()
{
	resize(1000, 800);
	setWindowTitle("Adage");

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
	QFile* file = new QFile("data/map.map");
	bool result = file->open(QIODevice::ReadOnly | QIODevice::Text);
	assert(result); //TODO error handling
	map->load(file);

	/*QFile* outFile = new QFile("map-out.map");
	result = outFile->open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
	assert(result);
	map->save(outFile);*/

	new GhostBustersHQ(mGame->world());

	for (int i=0; i<60; ++i) //ghost food
		new Person(Math::Point(Math::randFloat(-75,75),Math::randFloat(-80,90)), mGame->world());
	for (int i=0; i<60; ++i)
		new Person(Math::Point(Math::randFloat(-75,75),Math::randFloat(190,260)), mGame->world());
	for (int i=0; i<20; ++i)
		new Ghost(Math::Point(Math::randFloat(-100,100),Math::randFloat(-100,100)), mGame->world());
	for (int i=0; i<20; ++i)
		new Ghost(Math::Point(Math::randFloat(-100,100),Math::randFloat(180,380)), mGame->world());
	for (int i=0; i<4; ++i)
		new GhostBuster(Math::Point(Math::randFloat(-75,75),Math::randFloat(-90,90)), mGame->world());
	for (int i=0; i<4; ++i)
		new GhostBuster(Math::Point(Math::randFloat(-75,75),Math::randFloat(190,270)), mGame->world());

	Navigator* navvy = new Navigator(mGame->world());
	mGame->setNavvy(navvy);

	mBlueprint = new BlueprintWindow(mGame);
	setCentralWidget(mBlueprint);

	connect(mGame, SIGNAL(worldUpdated()), mBlueprint->blueprint(), SLOT(repaint()));
}

GameUI::~GameUI()
{
	delete mGame;
}
