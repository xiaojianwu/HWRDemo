#ifndef HWR_H
#define HWR_H

#include <QtWidgets/QWidget>
#include "ui_hwr.h"

#include "HWRCanvas.h"

class HWR : public QWidget
{
	Q_OBJECT

public:
	HWR(QWidget *parent = 0);
	~HWR();

	private slots:
	void onClear();

	void onTypeChanged(int index);

private slots:
	void onRecognizeResult(QStringList list);

private:
	Ui::HWRClass ui;

	int m_index;

	HWRCanvas *m_hwArea;

	AbstractRecognizer* m_recognizer;

	QStringList m_resultList;

	QHash<QString, QString> m_options;
};

#endif // HWR_H
