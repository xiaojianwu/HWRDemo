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
	void onStartHWR();

	void onTypeChanged(int index);

private:
	Ui::HWRClass ui;

	int m_index;
};

#endif // HWR_H
