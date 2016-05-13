#include <QApplication>
#include "qtmmlwidget.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    QtMmlWidget win;
    QString strContent;
    QFile formulaFile(":./res/formula.txt");
    if(!formulaFile.open(QIODevice::ReadOnly))
    {
        exit(0);
    }
    strContent = formulaFile.readAll();
    win.setContent(strContent);
    win.setDrawFrames(false);
    win.show();
    return app.exec();
}
