#include <QApplication>
#include <QFile>
#include "GameUI.h"
#include "math/rand.h"
#include "world/Entities.h"

int main(int argc, char **argv)
{
	QApplication a(argc, argv);

	QFile style("adage.style");
	style.open(QIODevice::ReadOnly | QIODevice::Text);
	a.setStyleSheet(style.readAll());
	style.close();

	Math::initRand();

	registerEntities();

	GameUI* ui = new GameUI();
	ui->show();

	a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
	return a.exec();
}
