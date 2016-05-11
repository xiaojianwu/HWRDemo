#ifndef RECOGNIZERFACTORY_H
#define RECOGNIZERFACTORY_H

#include <QObject>


#include "abstractrecognizer.h"

#include <QHash>


class RecognizerFactory : public QObject
{
	Q_OBJECT
public:
	~RecognizerFactory() { instance = nullptr; }

	static RecognizerFactory* Get() 
	{
		if (instance == nullptr)
			instance = new RecognizerFactory;
		return instance;
	}


	enum HWR_TYPE
	{
		HWR_TYPE_ZINNA = 0,
		HWR_TYPE_GOOGLE_INPUT_TOOLS, 
		HWR_TYPE_HANVON, // ººÍõ
	};

	AbstractRecognizer* getRecognizer(HWR_TYPE type);


private:
	static RecognizerFactory* instance;
	RecognizerFactory();

private:
	

	QHash<int, AbstractRecognizer*> m_hwrs;
};

#endif // RECOGNIZERFACTORY_H
