#ifndef GOOGLERECOGNIZER_H
#define GOOGLERECOGNIZER_H

#include "abstractrecognizer.h"


#include <QNetworkAccessManager>
#include <QNetworkReply>

class GoogleRecognizer : public AbstractRecognizer
{
	Q_OBJECT

public:
	GoogleRecognizer(QObject *parent);
	~GoogleRecognizer();


	virtual bool init(QHash<QString, QString> options);

	virtual QStringList recognize(STROKES strokes);

private:

	QByteArray pack(STROKES strokes);

	void sendRequest(QByteArray data);

	private slots:
	void slotReadyRead();
	void slotError(QNetworkReply::NetworkError);
	void slotSslErrors(QList<QSslError>);


private:
	QNetworkAccessManager m_nam;

};

#endif // GOOGLERECOGNIZER_H
