#include "hwr.h"

#include "abstractrecognizer.h"

#include "recognizerfactory.h"

HWR::HWR(QWidget *parent)
	: QWidget(parent)
	, m_index(0)
{
	ui.setupUi(this);

	
	QString modelPath = QString("%1\\%2").arg(qApp->applicationDirPath()).arg("handwriting-zh_CN.model");
	m_options[AbstractRecognizer::OPTION_KEY_MODEL_PATH] = modelPath;
	m_options[AbstractRecognizer::OPTION_KEY_CANVAS_HEIGHT] = QString::number(400);
	m_options[AbstractRecognizer::OPTION_KEY_CANVAS_WIDTH] = QString::number(400); ;

	m_hwArea = new HWRCanvas(ui.widget_hwArea);

	m_recognizer = RecognizerFactory::Get()->getRecognizer((RecognizerFactory::HWR_TYPE)m_index, m_options);
	m_hwArea->setRecognizer(m_recognizer);
	//m_recognizer->init(m_options);
	connect(m_recognizer, SIGNAL(recognizeResult(QStringList)), this, SLOT(onRecognizeResult(QStringList)));


	connect(ui.pushButton_clear, SIGNAL(clicked()), this, SLOT(onClear()));

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
