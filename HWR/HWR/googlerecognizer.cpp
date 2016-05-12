#include "googlerecognizer.h"

#include <QJsonDocument>

#include <QJsonObject>
#include <QJsonArray>

#include <QDebug>

#include <QNetworkProxy>
#include <QNetworkRequest>



GoogleRecognizer::GoogleRecognizer(QObject *parent)
	: AbstractRecognizer(parent)
{
	qRegisterMetaType<QHash<QString, QString>>("QHash<QString, QString>");
	qRegisterMetaType<STROKES>("STROKES");
	connect(this, SIGNAL(sigInit(QHash<QString, QString>)), this, SLOT(onInit(QHash<QString, QString>)));
	connect(this, SIGNAL(sigRecognize(STROKES)), this, SLOT(onRecognize(STROKES)));
}

GoogleRecognizer::~GoogleRecognizer()
{

}


void GoogleRecognizer::onInit(QHash<QString, QString> options)
{
	m_nam = new QNetworkAccessManager;
	QNetworkProxy proxy;
	proxy.setType(QNetworkProxy::Socks5Proxy);
	proxy.setHostName("127.0.0.1");
	proxy.setPort(1080);

	m_nam->setProxy(proxy);
}

void GoogleRecognizer::onRecognize(STROKES strokes)
{
	QByteArray data = pack(strokes);

	sendRequest(data);
}


QByteArray GoogleRecognizer::pack(STROKES strokes)
{
	QJsonDocument doc;

	QJsonObject obj;

	obj["options"] = "enable_pre_space";

	QJsonArray objRequests;
	QJsonObject objRequest;

	QJsonObject objWritingGuide;

	objWritingGuide["writing_area_width"] = m_canvasWidth;
	objWritingGuide["writing_area_height"] = m_canvasHeight;

	objRequest["writing_guide"] = objWritingGuide;

	QJsonArray trace;
	QJsonArray endStroke;
	for each (STROKE stroke in strokes)
	{
		QJsonArray w;
		QJsonArray xPoints = pointsToJsonArray(stroke[0]);
		QJsonArray yPoints = pointsToJsonArray(stroke[1]);

		QJsonArray endPoint;

		w.append(xPoints);
		w.append(yPoints);
		w.append(endPoint);

		trace.append(w);
	}
	trace.append(endStroke);


	objRequest["ink"] = trace;
	objRequest["language"] = "zh_CN";

	objRequests.append(objRequest);

	obj["requests"] = objRequests;

	doc.setObject(obj);

	QByteArray jsonData = doc.toJson(QJsonDocument::Compact);

	//qDebug() << jsonData;
	return jsonData;
}


void GoogleRecognizer::sendRequest(QByteArray data)
{
	QNetworkRequest request;
	request.setUrl(QUrl("https://www.google.com/inputtools/request?ime=handwriting&app=mobilesearch&cs=1&oe=UTF-8"));
	//request.setRawHeader("User-Agent", "MyOwnBrowser 1.0");

	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

	QNetworkReply *reply = m_nam->post(request, data);


	connect(reply, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
	connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
		this, SLOT(slotError(QNetworkReply::NetworkError)));
#ifndef QT_NO_SSL
	connect(reply, SIGNAL(sslErrors(QList<QSslError>)),
		this, SLOT(slotSslErrors(QList<QSslError>)));
#endif
}


void GoogleRecognizer::slotReadyRead()
{
	QNetworkReply* reply = (QNetworkReply*)sender();

	QByteArray body = reply->readAll();

	//qDebug() << "result=" << QString::fromUtf8(body);

	QJsonDocument doc = QJsonDocument::fromJson(body);

	if (doc.isArray())
	{
		QJsonArray parsed = doc.array();
		QJsonArray recognizeList = parsed.at(1).toArray().at(0).toArray().at(1).toArray();

		QStringList resultList;
		for (size_t i = 0; i < recognizeList.size(); i++)
		{
			resultList << recognizeList[i].toString();
		}

		//qDebug() << "result list=" << resultList;

		emit recognizeResult(resultList);
	}
}
void GoogleRecognizer::slotError(QNetworkReply::NetworkError e)
{
	qDebug() << e;
}
void GoogleRecognizer::slotSslErrors(QList<QSslError> e)
{
	qDebug() << e;
}
