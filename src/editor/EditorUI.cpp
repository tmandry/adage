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
#include "world/EntityFactory.h"
#include "world/Entities.h"
#include "world/FactoryManager.h"

EditorUI::EditorUI()
	:	mFile(0)
{
	setupUi(this);
	actNew->setIcon(style()->standardIcon(QStyle::SP_FileIcon));
	actOpen->setIcon(style()->standardIcon(QStyle::SP_DirOpenIcon));
	actSave->setIcon(style()->standardIcon(QStyle::SP_DriveFDIcon));

	CommWindow *comm = new CommWindow(CommDock);
	CommDock->setWidget(comm);
	CommDock->setVisible(false);

	mGame = new EditorGame(comm);
	mBp = new EditorBlueprint(mGame, this);
	setCentralWidget(mBp);
	connect(mBp, SIGNAL(zoomChanged(float)), this, SLOT(zoomChanged(float)));
	connect(mBp, SIGNAL(dropEntity(Math::Point)), this, SLOT(dropEntity(Math::Point)));
	connect(mGame, SIGNAL(worldUpdated()), mBp, SLOT(repaint()));

	toolSelect = new QButtonGroup(this);
	toolSelect->addButton((QAbstractButton*)toolBar->widgetForAction(actPanTool), EditorBlueprint::pan);
	toolSelect->addButton((QAbstractButton*)toolBar->widgetForAction(actMoveTool), EditorBlueprint::move);
	connect(actMoveTool, SIGNAL(triggered()), this, SLOT(updateTool()));

	entityBox = new QComboBox(this);
	entityBox->addItem("");
	entityBox->addItems(FactoryManager::instance()->types());
	toolBar->addSeparator();
	toolBar->addWidget(entityBox);

	zoomLabel = new QLabel;
	statusbar->addPermanentWidget(zoomLabel);
	zoomChanged(mBp->zoom());


	connect(actNew, SIGNAL(triggered()), this, SLOT(New()));
	connect(actOpen, SIGNAL(triggered()), this, SLOT(open()));
	connect(actSave, SIGNAL(triggered()), this, SLOT(save()));
	connect(actSaveAs, SIGNAL(triggered()), this, SLOT(saveAs()));
	connect(actShowNavmesh, SIGNAL(triggered(bool)), mBp, SLOT(setShowNavmesh(bool)));
	connect(actPopulate, SIGNAL(triggered()), this, SLOT(populate()));
	connect(actReset, SIGNAL(triggered()), this, SLOT(reset()));
	connect(actStartStop, SIGNAL(triggered()), this, SLOT(startStop()));
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
		CommDock->setVisible(true);
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


	EntityFactory::Properties props;
	props["pos"] = QVariant::fromValue(Math::Point(50,50));

	EntityFactory* factory = new Person::Factory();
	factory->buildEntity(props, mGame->world());
	delete factory;
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

void EditorUI::zoomChanged(float zoom)
{
	zoomLabel->setText( QString::number(zoom, 'f', 2) + "x" );
}

void EditorUI::updateTool()
{
	mBp->setTool(toolSelect->checkedId());
}

void EditorUI::dropEntity(Math::Point pos)
{
	if (entityBox->currentText().isEmpty()) return;

	EntityFactory* f = FactoryManager::instance()->getFactory(entityBox->currentText());
	EntityFactory::Properties params;
	params["pos"] = QVariant::fromValue<Math::Point>(pos);
	f->buildEntity(params, mGame->world());

	mBp->update();
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
