#include "hwr.h"

#include "abstractrecognizer.h"

#include "recognizerfactory.h"

#include <QNetworkProxy>

HWR::HWR(QWidget *parent)
	: QWidget(parent)
	, m_index(0)
{
	ui.setupUi(this);

	m_proxyType = QNetworkProxy::HttpProxy;
	m_proxyIp = "192.168.16.232";
	m_proxyPort = 8080;
	
	QString modelPath = QString("%1\\%2").arg(qApp->applicationDirPath()).arg("handwriting-zh_CN.model");
	m_options[AbstractRecognizer::OPTION_KEY_MODEL_PATH] = modelPath;
	m_options[AbstractRecognizer::OPTION_KEY_CANVAS_HEIGHT] = QString::number(400);
	m_options[AbstractRecognizer::OPTION_KEY_CANVAS_WIDTH] = QString::number(400);

	m_options[AbstractRecognizer::OPTION_KEY_PROXY_TYPE] = QString::number(m_proxyType);
	m_options[AbstractRecognizer::OPTION_KEY_PROXY_IP] = m_proxyIp;
	m_options[AbstractRecognizer::OPTION_KEY_PROXY_PORT] = QString::number(m_proxyPort);

	m_hwArea = new HWRCanvas(ui.widget_hwArea);

	m_recognizer = RecognizerFactory::Get()->getRecognizer((RecognizerFactory::HWR_TYPE)m_index, m_options);
	m_hwArea->setRecognizer(m_recognizer);
	//m_recognizer->init(m_options);
	connect(m_recognizer, SIGNAL(recognizeResult(QStringList)), this, SLOT(onRecognizeResult(QStringList)));

	connect(ui.pushButton_recognize, SIGNAL(clicked()), this, SLOT(onRecognize()));
	
	connect(ui.pushButton_clear, SIGNAL(clicked()), this, SLOT(onClear()));
	connect(ui.pushButton_proxy, SIGNAL(clicked()), this, SLOT(onProxy()));

	connect(&m_proxyConfDlg, SIGNAL(sigProxyConf(int, QString, int)), this, SLOT(onProxyConf(int, QString, int)));

	

	connect(ui.comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onTypeChanged(int)));
}

HWR::~HWR()
{

}


void HWR::onClear()
{
	m_hwArea->clear();
	ui.listWidget->clear();
}

void HWR::onProxy()
{
	m_proxyConfDlg.show();
}

void HWR::onProxyConf(int proxyType, QString proxyIp, int proxyPort)
{
	m_proxyIp = proxyIp;
	m_proxyPort = proxyPort;

	if (proxyType == 0)
	{
		m_proxyType = QNetworkProxy::HttpProxy;
	}
	else
	{
		m_proxyType = QNetworkProxy::Socks5Proxy;
	}

	m_options[AbstractRecognizer::OPTION_KEY_PROXY_TYPE] = QString::number(m_proxyType);
	m_options[AbstractRecognizer::OPTION_KEY_PROXY_IP] = m_proxyIp;
	m_options[AbstractRecognizer::OPTION_KEY_PROXY_PORT] = QString::number(m_proxyPort);
}


void HWR::onTypeChanged(int index)
{
	m_index = index;

	disconnect(m_recognizer, 0);

	m_recognizer = RecognizerFactory::Get()->getRecognizer((RecognizerFactory::HWR_TYPE)m_index, m_options);
	m_hwArea->setRecognizer(m_recognizer);
	//m_recognizer->init(m_options);

	connect(m_recognizer, SIGNAL(recognizeResult(QStringList)), this, SLOT(onRecognizeResult(QStringList)));

	
}

void HWR::onRecognizeResult(QStringList list)
{
	ui.listWidget->clear();
	m_resultList = list;

	ui.listWidget->addItems(m_resultList);
}

void HWR::onRecognize()
{
	m_hwArea->recognize();
}
