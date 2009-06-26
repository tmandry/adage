#ifndef BLUEPRINTWINDOW_H_
#define BLUEPRINTWINDOW_H_

#include <QWidget>
#include <QToolBar>
#include <QSize>
#include <QStatusBar>
#include <QLabel>
#include <QComboBox>
#include "Game.h"
#include "ui/Blueprint.h"

class BlueprintWindow : public QWidget
{
	Q_OBJECT

public:
	BlueprintWindow(Game* game, QWidget* parent=0);
	virtual ~BlueprintWindow();

	Blueprint* blueprint() const { return mBlueprint; }

	virtual QSize sizeHint() const;

public slots:
	void zoomIn();
	void zoomOut();
	void zoomChanged(float zoom);

private:
	static const float zoomInterval = 0.3;

	QToolBar* mToolbar;
	Blueprint* mBlueprint;
	QStatusBar* mStatusBar;
	QLabel* mZoomLbl;
	QComboBox* mToolCombo;
};

#endif /*BLUEPRINTWINDOW_H_*/
