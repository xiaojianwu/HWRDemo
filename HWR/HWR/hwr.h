#ifndef HWR_H
#define HWR_H

#include <QtWidgets/QWidget>
#include "ui_hwr.h"

class HWR : public QWidget
{
	Q_OBJECT

public:
	HWR(QWidget *parent = 0);
	~HWR();

private slots:
void onOne();
void onTwo();

private:
	Ui::HWRClass ui;
};

#endif // HWR_H
