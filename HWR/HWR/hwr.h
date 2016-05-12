#ifndef HWR_H
#define HWR_H

#include <QtWidgets/QWidget>
#include "ui_hwr.h"

#include "HWRCanvas.h"

#include "proxyconfdlg.h"

class HWR : public QWidget
{
	Q_OBJECT

public:
	HWR(QWidget *parent = 0);
	~HWR();

private slots:
	void onRecognizeResult(QStringList list);
	void onTypeChanged(int index);
	void onRecognize();
	void onClear();
	void onProxy();
	void onProxyConf(int proxyType, QString proxyIp, int proxyPort);

private:
	Ui::HWRClass ui;

	int m_index;

	HWRCanvas *m_hwArea;

	AbstractRecognizer* m_recognizer;

	QStringList m_resultList;

	QHash<QString, QString> m_options;

	ProxyConfDlg m_proxyConfDlg;


	int m_proxyType;
	QString m_proxyIp;
	int m_proxyPort;
};

#endif // HWR_H
