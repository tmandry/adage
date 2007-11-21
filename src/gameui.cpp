#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTextEdit>
#include "gameui.h"
#include "game.h"
#include "ui/blueprint.h"
#include "world/wall.h"

GameUI::GameUI()
{
	mGame = new Game();
	mGame->map().addWall( Wall(0,0, 100,200) );
	mGame->map().addWall( Wall(100,200, 200,200) );
	mGame->map().addWall( Wall(200,200, 320,240) );
	
	//resize(320, 210);
	setWindowTitle("Adage");
	
	/*mLayout = new QVBoxLayout(this);
	
	QHBoxLayout* horizLayout = new QHBoxLayout(); 
	
	mButton = new QPushButton("Push me");
	horizLayout->addWidget(mButton, 0, Qt::AlignLeft);
	
	mBtnClose = new QPushButton("Close");
	horizLayout->addWidget(mBtnClose, 0, Qt::AlignRight);
	
	mLayout->addLayout(horizLayout);*/
	
	mBlueprint = new Blueprint(mGame);
	setCentralWidget(mBlueprint);
		
	//connect(mBtnClose, SIGNAL(clicked()), this, SLOT(close()));
}

GameUI::~GameUI()
{
}
