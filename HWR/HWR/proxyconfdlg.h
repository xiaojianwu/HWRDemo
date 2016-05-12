#ifndef PROXYCONFDLG_H
#define PROXYCONFDLG_H

#include <QDialog>
#include "ui_proxyconfdlg.h"

#include <QCloseEvent>

class ProxyConfDlg : public QDialog
{
	Q_OBJECT

public:
	ProxyConfDlg(QWidget *parent = 0);
	~ProxyConfDlg();

protected:
	void closeEvent(QCloseEvent *e);

signals:
	void sigProxyConf(int proxyType, QString proxyIp, int proxyPort);

private:
	Ui::ProxyConfDlg ui;
};

#endif // PROXYCONFDLG_H
