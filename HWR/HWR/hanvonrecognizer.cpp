#include "hanvonrecognizer.h"

#include <QJsonDocument>
#include <QJsonObject>

HanvonRecognizer::HanvonRecognizer(QObject *parent)
	: AbstractRecognizer(parent)
{

}

HanvonRecognizer::~HanvonRecognizer()
{

}



bool HanvonRecognizer::init(QHash<QString, QString> options)
{
	AbstractRecognizer::init(options);

	return true;
}

QStringList HanvonRecognizer::recognize(STROKES strokes)
{
	QByteArray data = pack(strokes);

	sendRequest(data);

	QStringList result;
	return result;
}


QByteArray HanvonRecognizer::pack(STROKES strokes)
{
	QJsonDocument doc;

	QJsonObject obj;

	obj["uid"] = "127.0.0.1";
	obj["lang"] = "chns";


	QStringList data;
	for each (STROKE stroke in strokes)
	{
		for (int i = 0; i < stroke[0].size() - 1; i++)
		{
			data << QString::number(stroke[0].at(i)) << QString::number(stroke[1].at(i));
		}
		data << "-1" << "0";
	}

	data << "-1" << "-1";
	obj["data"] = data.join(',');

	doc.setObject(obj);

	QByteArray jsonData = doc.toJson(QJsonDocument::Compact);

	qDebug() << jsonData;
	return jsonData;
}


void HanvonRecognizer::sendRequest(QByteArray data)
{
	QNetworkRequest request;

	QString key = "2e831bbc-3eac-4d03-860d-48b9760b6bd0";

	QString url = QString("http://api.hanvon.com/rt/ws/v1/hand/line?key=%1&code=d4b92957-78ed-4c52-a004-ac3928b054b5").arg(key);
	request.setUrl(QUrl(url));

	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");

	QNetworkReply *reply = m_nam.post(request, data);


	connect(reply, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
	connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
		this, SLOT(slotError(QNetworkReply::NetworkError)));
#ifndef QT_NO_SSL
	connect(reply, SIGNAL(sslErrors(QList<QSslError>)),
		this, SLOT(slotSslErrors(QList<QSslError>)));
#endif
}


void HanvonRecognizer::slotReadyRead()
{
	QNetworkReply* reply = (QNetworkReply*)sender();

	QByteArray body = QByteArray::fromBase64(reply->readAll());

	//qDebug() << "result=" << QString::fromUtf8(body);

	QJsonDocument doc = QJsonDocument::fromJson(body);

	if (doc.isObject())
	{
		QJsonObject parsed = doc.object();

		QStringList resultList;
		if (parsed["code"].toInt() == 0)
		{
			QString result = parsed["result"].toString();

			QStringList codeList;
			codeList = result.split(',');

			QString zi;
			for each (QString code in codeList)
			{
				if (code == "0")
				{
					resultList << zi;
					zi = "";
				}
				else
				{
					zi += QChar(code.toInt());
				}
			}
		}

		emit recognizeResult(resultList);
	}
}
void HanvonRecognizer::slotError(QNetworkReply::NetworkError e)
{
	qDebug() << e;
}
void HanvonRecognizer::slotSslErrors(QList<QSslError> e)
{
	qDebug() << e;
}
