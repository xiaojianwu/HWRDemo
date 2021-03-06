#ifndef GOOGLERECOGNIZER_H
#define GOOGLERECOGNIZER_H

#include "abstractrecognizer.h"


#include <QNetworkAccessManager>
#include <QNetworkReply>

class GoogleRecognizer : public AbstractRecognizer
{
	Q_OBJECT

public:
	GoogleRecognizer(QObject *parent = nullptr);
	~GoogleRecognizer();


private slots:
	void onInit(QHash<QString, QString> options);
	void onRecognize(STROKES strokes);

private:

	QByteArray pack(STROKES strokes);

	void sendRequest(QByteArray data);

	private slots:
	void slotReadyRead();
	void slotError(QNetworkReply::NetworkError);
	void slotSslErrors(QList<QSslError>);


private:
	QNetworkAccessManager* m_nam;

};

#endif // GOOGLERECOGNIZER_H
