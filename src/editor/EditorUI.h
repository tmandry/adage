#ifndef EDITORUI_H_
#define EDITORUI_H_

#include <QMainWindow>
#include <QAction>
#include <QFile>
#include "EditorGame.h"
#include "ui_editor.h"

class EditorUI : public QMainWindow, private Ui::Editor
{
	Q_OBJECT
public:
	EditorUI();
	virtual ~EditorUI();

	EditorGame* mGame;
	QFile* mFile;

private slots:
	void New();
	void open();
	void save();
	void saveAs();

private:
	bool openFile(QString filename);
};

#endif /* EDITORUI_H_ */
