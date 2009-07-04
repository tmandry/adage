#include <QAction>
#include <QVBoxLayout>
#include <QSizePolicy>
#include <QCheckBox>
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

	mToolCombo = new QComboBox(this);
	mToolCombo->insertItem(Blueprint::dormantPortal, QIcon(), "Dormant Portal");
	mToolCombo->insertItem(Blueprint::portal, QIcon(), "Active Portal");
	mToolCombo->insertItem(Blueprint::trap, QIcon(), "Ghost Trap");
	mToolCombo->insertItem(Blueprint::nav, QIcon(), "Navigate!");
	mToolCombo->setCurrentIndex(-1);
	mToolbar->addSeparator();
	mToolbar->addWidget(mToolCombo);

	QCheckBox* showNavmesh = new QCheckBox("&Navmesh", this);
	mToolbar->addWidget(showNavmesh);

	mStatusBar = new QStatusBar;
	mZoomLbl = new QLabel(QString::number(mBlueprint->zoom(), 'f', 2) + "x");
	mStatusBar->addPermanentWidget(mZoomLbl);

	QVBoxLayout* layout = new QVBoxLayout;
	layout->addWidget(mToolbar);
	layout->addWidget(mBlueprint);
	layout->addWidget(mStatusBar);

	setLayout(layout);

	connect(mBlueprint, SIGNAL(zoomChanged(float)), this, SLOT(zoomChanged(float)));
	connect(mToolCombo, SIGNAL(activated(int)), mBlueprint, SLOT(setTool(int)));
	connect(showNavmesh, SIGNAL(stateChanged(int)), mBlueprint, SLOT(setShowNavmesh(int)));
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
