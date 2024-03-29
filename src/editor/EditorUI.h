#ifndef EDITORUI_H_
#define EDITORUI_H_

#include <QMainWindow>
#include <QAction>
#include <QFile>
#include <QButtonGroup>
#include <QComboBox>
#include "EditorGame.h"
#include "ui_editor.h"
#include "EditorBlueprint.h"

class QLabel;

class EditorUI : public QMainWindow, private Ui::Editor
{
	Q_OBJECT
public:

	EditorUI();
	virtual ~EditorUI();

public slots:
	void populate();
	void reset();
	void startStop();

private slots:
	void New();
	void open();
	void save();
	void saveAs();

	void zoomChanged(float zoom);
	void updateTool();
	void dropEntity(Math::Point pos);
	void generateNavmesh();

private:
	//bool openFile(QString filename, QFlags<Qt::OpenModeFlag> flags);

	QButtonGroup* toolSelect;
	QLabel* zoomLabel;
	QComboBox* entityBox;

	EditorGame* mGame;
	EditorBlueprint* mBp;
	QFile* mFile;
};

#endif /* EDITORUI_H_ */
