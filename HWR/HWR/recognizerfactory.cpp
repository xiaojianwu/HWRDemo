#include "recognizerfactory.h"


#include "zinnarecognizer.h"
#include "googlerecognizer.h"

#include "hanvonrecognizer.h"

#include "hcirecognizer.h"

RecognizerFactory* RecognizerFactory::instance = nullptr;
RecognizerFactory::RecognizerFactory() : m_lastRecognizer(nullptr)
{
	connect(&m_workerThread, SIGNAL(finished()), this, SLOT(onThreadStarted()));
}


AbstractRecognizer* RecognizerFactory::getRecognizer(HWR_TYPE type, QHash<QString, QString> options)
{
	if (m_hwrs.contains(type))
	{
		return m_hwrs[type];
	}
	else
	{
		AbstractRecognizer* hwr = nullptr;
		switch (type)
		{
		case RecognizerFactory::HWR_TYPE_ZINNA:
			hwr = new ZinnaRecognizer(this);
			break;
		case RecognizerFactory::HWR_TYPE_GOOGLE_INPUT_TOOLS:
		{
			hwr = new GoogleRecognizer();
			hwr->moveToThread(&m_workerThread);
		}

		break;
		case RecognizerFactory::HWR_TYPE_HANVON:
		{
			hwr = new HanvonRecognizer();
			hwr->moveToThread(&m_workerThread);
		}

		break;
		case RecognizerFactory::HWR_TYPE_HCI:
		{
			hwr = new HCIRecognizer();
			hwr->moveToThread(&m_workerThread);
		}

		break;
		default:
			break;
		}

		m_hwrs[type] = hwr;


		if (m_workerThread.isRunning())
		{
			hwr->init(options);
		}
		else
		{
			m_lastOptions = options;
			m_lastRecognizer = hwr;
			m_workerThread.start();
		}

		return hwr;
	}
}

void RecognizerFactory::onThreadStarted()
{
	m_lastRecognizer->init(m_lastOptions);
}
