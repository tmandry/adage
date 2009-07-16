#ifndef EDITORUI_H_
#define EDITORUI_H_

#include <QMainWindow>
#include <QAction>
#include <QFile>
#include <QButtonGroup>
#include "EditorGame.h"
#include "ui_editor.h"
#include "EditorBlueprint.h"

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

private:
	//bool openFile(QString filename, QFlags<Qt::OpenModeFlag> flags);

	QButtonGroup* toolSelect;

	EditorGame* mGame;
	EditorBlueprint* mBp;
	QFile* mFile;
};

#endif /* EDITORUI_H_ */
