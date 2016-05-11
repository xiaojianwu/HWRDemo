#include "googlerecognizer.h"

#include <QJsonDocument>

#include <QJsonObject>
#include <QJsonArray>

#include <QDebug>


GoogleRecognizer::GoogleRecognizer(QObject *parent)
	: AbstractRecognizer(parent)
{

}

GoogleRecognizer::~GoogleRecognizer()
{

}


bool GoogleRecognizer::init(QHash<QString, QString> options)
{

	AbstractRecognizer::init(options);

	return true;
}

QStringList GoogleRecognizer::recognize(STROKES strokes)
{
	QJsonDocument doc;

	QJsonObject obj;

	obj["options"] = "enable_pre_space";

	QJsonObject objRequests;

	QJsonObject objWritingGuide;

	objWritingGuide["writing_area_width"] = m_canvasWidth;
	objWritingGuide["writing_area_height"] = m_canvasHeight;

	objRequests["writing_guide"] = objWritingGuide;

	QJsonArray trace;
	for each (STROKE stroke in strokes)
	{
		QJsonArray w;
		QJsonArray xPoints = pointsToJsonArray(stroke[0]);
		QJsonArray yPoints = pointsToJsonArray(stroke[1]);

		w.append(xPoints);
		w.append(yPoints);

		trace.append(w);
	}


	objRequests["ink"] = trace;
	objRequests["language"] = "zh_CN";

	obj["requests"] = objRequests;

	doc.setObject(obj);

	QString jsonData = doc.toJson(QJsonDocument::Compact);

	qDebug() << jsonData;
	
	QStringList result;
	return result;
}


QString GoogleRecognizer::pack()
{
	return "";
}
