#include "recognizerfactory.h"


#include "zinnarecognizer.h"

RecognizerFactory* RecognizerFactory::instance = nullptr;
RecognizerFactory::RecognizerFactory()
{

}


AbstractRecognizer* RecognizerFactory::getRecognizer(HWR_TYPE type)
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
			break;
		default:
			break;
		}

		m_hwrs[type] = hwr;

		return hwr;
		
	}
	
}