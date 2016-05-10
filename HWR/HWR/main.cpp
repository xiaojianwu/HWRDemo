

#include <QtGui>
#include <QApplication>

#include "hwr.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

	HWR w;
	w.show();


    return a.exec();
}
