#include <QtGui>
#include "EditorUI.h"
#include "ui/Blueprint.h"
#include "map/Map.h"
#include "map/MapHandler.h"
#include "map/MapGenerator.h"

EditorUI::EditorUI()
	:	mFile(0)
{
	mGame = new EditorGame();

	setupUi(this);

	Blueprint* bp = new Blueprint(mGame, this);
	setCentralWidget(bp);

	connect(actNew, SIGNAL(triggered()), this, SLOT(New()));
	connect(actOpen, SIGNAL(triggered()), this, SLOT(open()));
	connect(actSave, SIGNAL(triggered()), this, SLOT(save()));
	connect(actSaveAs, SIGNAL(triggered()), this, SLOT(saveAs()));
}

EditorUI::~EditorUI()
{
	delete mGame;
}

void EditorUI::New()
{
	if (mFile) {
		mFile->close();
		delete mFile;
	}

	delete mGame;
	mGame = new EditorGame();
}

void EditorUI::open()
{
	QString filename =
		QFileDialog::getOpenFileName(this, tr("Open Map File"), QDir::currentPath(), tr("Map files (*.map)"));

	if (!openFile(filename)) return;

	delete mGame;
	mGame = new EditorGame();
	Map map(mGame->world());
	map.load(mFile);
}

void EditorUI::save()
{
	if (!mFile) {
		saveAs();
		return;
	}

	Map map(mGame->world());
	map.save(mFile);
}

void EditorUI::saveAs()
{
	QString filename =
		QFileDialog::getSaveFileName(this, tr("Save Map As..."), QDir::currentPath(), tr("Map files (*.map)"));

	if (!openFile(filename)) return;

	Map map(mGame->world());
	map.save(mFile);
}

bool EditorUI::openFile(QString filename)
{
	if (filename.isEmpty()) return false;

	QFile* file = new QFile(filename);
	if (!file->open(QFile::ReadWrite | QFile::Text)) {
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
}
