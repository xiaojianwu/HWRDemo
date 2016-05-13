#ifndef MMLWIDGET_H
#define MMLWIDGET_H

#include <QtWidgets/QWidget>
#include "ui_mmlwidget.h"

class mmlwidget : public QWidget
{
	Q_OBJECT

public:
	mmlwidget(QWidget *parent = 0);
	~mmlwidget();

private:
	Ui::mmlwidgetClass ui;
};

#endif // MMLWIDGET_H
