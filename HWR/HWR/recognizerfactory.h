#ifndef RECOGNIZERFACTORY_H
#define RECOGNIZERFACTORY_H

#include <QObject>


#include "abstractrecognizer.h"

#include <QHash>

#include <QThread>


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
		HWR_TYPE_HANVON, // ∫∫Õı
		HWR_TYPE_HCI, // ¡È‘∆
	};

	AbstractRecognizer* getRecognizer(HWR_TYPE type, QHash<QString, QString> options);


private:
	static RecognizerFactory* instance;
	RecognizerFactory();

private slots:
	void onThreadStarted();

private:
	

	QHash<int, AbstractRecognizer*> m_hwrs;

	AbstractRecognizer* m_lastRecognizer;
	QHash<QString, QString> m_lastOptions;

	QThread m_workerThread;
};

#endif // RECOGNIZERFACTORY_H
