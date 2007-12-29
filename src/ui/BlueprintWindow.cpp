#include <QAction>
#include <QVBoxLayout>
#include <QSizePolicy>
#include <QString>
#include "ui/BlueprintWindow.h"

BlueprintWindow::BlueprintWindow(Game* game, QWidget* parent):
	QWidget(parent),
	mBlueprint(new Blueprint(game))
{
	setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
	
	//TODO: make these icons instead of ugly text buttons!
	mToolbar = new QToolBar(tr("Blueprint actions"), this);
	mToolbar->addAction(tr("Home"), mBlueprint, SLOT(goHome()));
	mToolbar->addSeparator();
	mToolbar->addAction(tr("Zoom In"), this, SLOT(zoomIn()));
	mToolbar->addAction(tr("Zoom Out"), this, SLOT(zoomOut()));
	
	mStatusBar = new QStatusBar;
	mZoomLbl = new QLabel(QString::number(mBlueprint->zoom(), 'f', 2) + "x");
	mStatusBar->addPermanentWidget(mZoomLbl);
	
	QVBoxLayout* layout = new QVBoxLayout;
	layout->addWidget(mToolbar);
	layout->addWidget(mBlueprint);
	layout->addWidget(mStatusBar);
	
	setLayout(layout);
	
	connect(mBlueprint, SIGNAL(zoomChanged(float)), this, SLOT(zoomChanged(float)));
}

BlueprintWindow::~BlueprintWindow()
{
}

QSize BlueprintWindow::sizeHint() const
{
	return QSize(500, 640);
}

void BlueprintWindow::zoomIn()
{
	mBlueprint->setZoom(mBlueprint->zoom() + zoomInterval);
}

void BlueprintWindow::zoomOut()
{
	mBlueprint->setZoom(mBlueprint->zoom() - zoomInterval);
}

void BlueprintWindow::zoomChanged(float zoom)
{
	mZoomLbl->setText( QString::number(zoom, 'f', 2) + "x" );
}
