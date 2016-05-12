#include "proxyconfdlg.h"

ProxyConfDlg::ProxyConfDlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
}

ProxyConfDlg::~ProxyConfDlg()
{

}

void ProxyConfDlg::closeEvent(QCloseEvent *e)
{
	emit sigProxyConf(ui.comboBox->currentIndex(), ui.lineEdit->text(), ui.lineEdit_2->text().toInt());

	hide();

	e->ignore();
}
