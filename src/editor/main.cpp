#include <QApplication>
#include <QFile>
#include "EditorUI.h"
#include "math/rand.h"

int main(int argc, char **argv)
{
	QApplication a(argc, argv);

	QFile style("adage.style");
	style.open(QIODevice::ReadOnly | QIODevice::Text);
	a.setStyleSheet(style.readAll());
	style.close();

	Math::initRand();

	EditorUI* ui = new EditorUI();
	ui->show();

	a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
	return a.exec();
}