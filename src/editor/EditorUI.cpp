#include <QtGui>
#include "EditorUI.h"
#include "EditorBlueprint.h"
#include "map/Map.h"
#include "world/GhostBustersHQ.h"
#include "actors/Person.h"
#include "actors/Ghost.h"
#include "actors/GhostBuster.h"
#include "math/rand.h"
#include "ui/CommWindow.h"

EditorUI::EditorUI()
	:	mFile(0)
{
	setupUi(this);

	CommWindow *comm = new CommWindow(CommDock);
	CommDock->setWidget(comm);

	mGame = new EditorGame(comm);
	mBp = new EditorBlueprint(mGame, this);
	setCentralWidget(mBp);

	toolSelect = new QButtonGroup(this);

	QPushButton* bPan = new QPushButton("Pan");
	bPan->setCheckable(true);
	bPan->setChecked(true);
	toolBar->insertWidget(actShowNavmesh, bPan);
	toolSelect->addButton(bPan, EditorBlueprint::pan);

	QPushButton* bMove = new QPushButton("Move");
	bMove->setCheckable(true);
	toolBar->insertWidget(actShowNavmesh, bMove);
	toolSelect->addButton(bMove, EditorBlueprint::move);

	toolBar->insertSeparator(actShowNavmesh);

	connect(actNew, SIGNAL(triggered()), this, SLOT(New()));
	connect(actOpen, SIGNAL(triggered()), this, SLOT(open()));
	connect(actSave, SIGNAL(triggered()), this, SLOT(save()));
	connect(actSaveAs, SIGNAL(triggered()), this, SLOT(saveAs()));
	connect(actShowNavmesh, SIGNAL(triggered(bool)), mBp, SLOT(setShowNavmesh(bool)));
	connect(actPopulate, SIGNAL(triggered()), this, SLOT(populate()));
	connect(actReset, SIGNAL(triggered()), this, SLOT(reset()));
	connect(actStartStop, SIGNAL(triggered()), this, SLOT(startStop()));
	connect(toolSelect, SIGNAL(buttonClicked(int)), mBp, SLOT(setTool(int)));
	connect(mGame, SIGNAL(worldUpdated()), mBp, SLOT(repaint()));
}

EditorUI::~EditorUI()
{
	delete mGame;
}

void EditorUI::populate()
{
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

	mBp->repaint();
}

void EditorUI::reset()
{
	mGame->resetWorld();

	if (mFile) {
		mFile->reset();
		Map map(mGame->world());
		map.load(mFile);
	}

	mBp->repaint();
}

void EditorUI::startStop()
{
	if (mGame->updating()) {
		mGame->stop();
		actStartStop->setText("Start");
	} else {
		mGame->start();
		actStartStop->setText("Stop");
	}

	mBp->repaint();
}

void EditorUI::New()
{
	if (mFile) {
		mFile->close();
		delete mFile;
	}

	mGame->resetWorld();
}

void EditorUI::open()
{
	QString filename =
		QFileDialog::getOpenFileName(this, tr("Open Map File"), QDir::currentPath(), tr("Map files (*.map)"));
	if (filename.isEmpty()) return;

	QFile* file = new QFile(filename);
	if (!file->open(QFile::ReadOnly | QFile::Text)) {
		QMessageBox::warning(this, tr("Adage Map Editor"),
				tr("Cannot open file %1: %2").arg(filename).arg(file->errorString())
		);
		return;
	}

	if (mFile) {
		mFile->close();
		delete mFile;
	}
	mFile = file;

	mGame->resetWorld();
	Map map(mGame->world());
	map.load(mFile);
}

void EditorUI::save()
{
	if (!mFile) {
		saveAs();
		return;
	}

	mFile->close();
	if (!mFile->open(QFile::ReadWrite | QFile::Truncate | QFile::Text)) {
		QMessageBox::warning(this, tr("Adage Map Editor"),
				tr("Cannot open file: %1").arg(mFile->errorString())
		);
		return;
	}

	Map map(mGame->world());
	map.save(mFile);
}

void EditorUI::saveAs()
{
	QString filename =
		QFileDialog::getSaveFileName(this, tr("Save Map As..."), QDir::currentPath(), tr("Map files (*.map)"));
	if (filename.isEmpty()) return;

	QFile* file = new QFile(filename);
	if (!file->open(QFile::WriteOnly | QFile::Truncate | QFile::Text)) {
		QMessageBox::warning(this, tr("Adage Map Editor"),
				tr("Cannot open file %1: %2").arg(filename).arg(file->errorString())
		);
		return;
	}

	if (mFile) {
		mFile->close();
		delete mFile;
	}
	mFile = file;

	Map map(mGame->world());
	map.save(mFile);
}

/*bool EditorUI::openFile(QString filename, QFlags<Qt::OpenModeFlag> flags)
{
	if (filename.isEmpty()) return false;

	QFile* file = new QFile(filename);
	if (!file->open(flags | QFile::Text)) {
		QMessageBox::warning(this, tr("Adage Map Editor"),
				tr("Cannot open file %1: %2").arg(filename).arg(file->errorString())
		);
		return false;
	}

	if (mFile) {
		mFile->close();
		delete mFile;
	}
	mFile = file;

	return true;
}*/
